#! /bin/bash -e
# $OMhelpKeyCharacter=@
#
top_srcdir="../.."
top_builddir="$top_srcdir/build"
#
# @begin sfun_grid.sh@@  @newlinech #@@
# @spell
#	writerow
#	sys
#	srcdir
#	builddir
#	bndd
#	grep
#	sfun
#	py
#	csv
#	len
#	omega
#	bnd
#	fi
#	dev
# @@ 
# @section d4_sfun_grid.py: Example and Test@@
# @index d4_sfun_grid.py, example@@
# @index example, d4_sfun_grid.py@@
# @index test, d4_sfun_grid.py@@
#
# @head Purpose@@
# This bash shell script is a simple example and test of
# @code d4_sfun_grid.py@@.
# It exits with status zero if the test passes
# and with status one if the test fails.
#
# @head Abort on Any Error@@
# @codep
set -e
# @@
#
# @head Python Script that Creates An sfun_out.csv@@
# @codep
cat << EOF > create_sfun_out.py
import csv
import sys
writer = csv.writer( open('sfun_out.csv', 'w') )
#
# list of possible values for function
sfun_name = [ 'S', 'C', 'iota', 'rho', 'chi', 'omega' ] 
# list of column names in sfun_out.csv
column_name = [ 'age', 'cohort', 'time', 'function', 'estimate' ]
for major in [ 'prior' , 'age', 'cohort', 'cross' ] :
	for minor in [ 'residual' , 'scaled' ] :
		column_name.append( major + '_' + minor )
n_col  = len(column_name)
#
age        = [ 20, 40, 60 ]
cohort     = [ 1900, 1920, 1940, 1980, 2000 ]
time_start = age[-1] + cohort[0]   # 1960
time_end   = age[0]  + cohort[-1]  # 2020
n_age    = len(age)
n_cohort = len(cohort)
n_sfun   = len(sfun_name)
step   = 1
#
writer.writerow(column_name)
for j in range( n_age ) :
	a     = age[j]
	start = n_cohort - 1
	while a + cohort[start] > time_start :
		start = start - 1
	stop   = 1
	while a + cohort[stop-1] < time_end :
		stop = stop + 1
	for k in range(start, stop, step ) :
		c = cohort[k]
		t = a + c
		for q in range(n_sfun) :
			row = [ a, c, t, sfun_name[q] ]
			for m in range(4, n_col, step) :
				row.append( t + 2. * a + 3. * m  + 4. * q  )
			writer.writerow(row)

#
sys.exit(None)
EOF
$top_builddir/python.sh \
	create_sfun_out.py
# @@
#
# @head Check Time Bounds@@
# @codep
$top_builddir/python.sh \
	$top_builddir/python/d4_sfun_grid.py sfun_out.csv > sfun_grid.bnd
if ! grep 'time_lower_bound *= *1960\.0' sfun_grid.bnd > /dev/null
then
	echo "sfun_grid.sh: time_lower_bound does not check"
	exit 1
fi
if ! grep 'time_upper_bound *= *2020\.0' sfun_grid.bnd > /dev/null
then
	echo "sfun_grid.sh: time_upper_bound does not check"
	exit 1
fi
# @@
#
# @head Create sfun_grid.csv@@
# @codep
time_start='1960'
time_end='2020'
n_time='3'
$top_builddir/python/d4_sfun_grid.py sfun_out.csv sfun_grid.csv \
	$time_start $time_end $n_time
# @@
#
# @head Check sfun_grid.csv@@
# @codep
cat << EOF > check_d4_sfun_grid.py
import csv
import sys
reader = csv.DictReader( open('sfun_grid.csv', 'r') )
#
# list of possible values for function
sfun_index = { 'S' : 0, 'C' : 1, 'iota' : 2, 'rho' : 3, 'chi' : 4, 'omega' : 5 }
# list of column names in sfun_out.csv
column_name = [ 'age', 'cohort', 'time', 'function', 'estimate' ]
for major in [ 'prior' , 'age', 'cohort', 'cross' ] :
	for minor in [ 'residual' , 'scaled' ] :
		column_name.append( major + '_' + minor )
n_col  = len(column_name)
#
for row in reader :
	q  = sfun_index[ row['function'] ]
	a  = float( row['age'] )
	t  = float( row['time'] ) 
	for m in range(4, n_col, 1) :
		name  = column_name[m]
		value = float( row[name] )
		check = t + 2. * a + 3. * m + 4. * q
		assert abs( 1. - value/check) < 1e-10
sys.exit(None)
EOF
$top_builddir/python.sh \
	check_d4_sfun_grid.py
# @@
#
# @head Final Message@@
# If this shell script reaches here, then test has passed.
# @codep
echo "sfun_grid.sh: OK"
# @@
#
# @end
