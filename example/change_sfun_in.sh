#! /bin/bash -e
# $OMhelpKeyCharacter=@
#
top_srcdir="../.."
top_builddir="$top_srcdir/build"
#
# @begin change_sfun_in.sh@@ @newlinech #@@
# @spell
# 	pde
#	srcdir
#	builddir
#	inf
#	sys
#	tr
#	sed
#	rm
#	cpp
#	dismod
#	skipinitialspace
#	sfun
#	py
#	csv
#	covariate
# @@
# @section d4_change_sfun_in.py: Example and Test@@
# @index d4_change_sfun_in.py, example@@
# @index example, d4_change_sfun_in.py@@
# @index test, d4_change_sfun_in.py@@
#
# @head Syntax@@
# @code change_sfun_in.sh@@
#
# @head Purpose@@
# This bash shell script is a simple example and test of
# @code d4_change_sfun_in.py@@.
# It exits with status zero if the test passes
# and with status one if the test fails.
#
# @head Abort on Any Error@@
# @codep
set -e
# @@
#
# @head parameter.csv@@
# The first thing we do is create the
# @cref/parameter.csv/dismod_pde_csv/parameter.csv/@@ file 
# as follows:
# @codep 
cat << EOF > parameter.csv
name,value
age_grid,0 10 20
cohort_grid,1950 1990 2030 
max_ode_step,1
print_level,5
max_iteration,80
converge_tolerance,1e-6
EOF
# @@
# For this example,
# we use large values for @code delta_age@@ and @code delta_cohort@@
# so that the resulting file @code sfun_in.csv@@ is small.
# Note that the first and last lines above are bash command that
# instruct bash to create the file @code parameter.csv@@ with the
# contents between them.
#
# @head measure_in.csv@@
# Next we create a very simple
# @cref/measure_in.csv/dismod_pde_csv/measure_in.csv/@@ file.
# @codep
cat << EOF > measure_in.csv
integrand,age_lower,age_upper,time_lower,time_upper,meas_like,meas_zeta,meas_sigma,meas_value,c_notes
remission,10,20,1990,2000,gaussian,1,1e-5,1e-1,no_comment
EOF
# @@
# The text above is in a form that makes it easier to read in this example,
# but it does not conform to the csv file 
# @cref/specifications/csv/dismod_pde CSV Files/@@.  
# The command @code tr '\n;' ' \n'@@
# replaces all the new-lines by spaces and the semi-colons by new-lines 
# which converts the text to csv format.
# The command @code  sed -e 's/  */ /g' -e '/^ *$/d'@@
# converts multiple contiguous spaces to a single space,
# which is not necessary, but helps make the result readable in an editor.
# It also deletes line that only have spaces in then.
# (This is necessary for this example because the new-line after the last
# semi-colon gets converted to a space.)
#
# @head template.csv@@
# Remove the file 
# @cref/template.csv/dismod_pde_csv/sfun_in.csv/Output Template/@@ 
# (if it exists) and then recreate it.
# @codep
rm -f template.csv
../cpp/dismod_pde_csv \
	parameter.csv measure_in.csv template.csv
# @@
#
# @head Running d4_change_sfun_in.py@@
# @codep
$top_builddir/python/d4_change_sfun_in.py template.csv sfun_in.csv \
\
-comment            'zeta          lower      initial          upper' \
-optimize_S           1.0            0.0         1e-2            1.0 \
\
-comment            'name           zeta        sigma           mean' \
-prior_S         gaussian            1.0          inf            0.0  \
\
-comment            'name           zeta        sigma          order' \
-age_S           gaussian            1.0         2e-2              1  \
-cohort_S         laplace            1.0         3e-2              2  \
-cross_S          laplace            2.0         3e-2
# @@
#
# @subhead Check sfun_in.csv@@
# @codep
cat << EOF > check_sfun_in.py
import csv
import sys
file   = open('sfun_in.csv', 'r')
reader = csv.DictReader(file, skipinitialspace=True)
ok     = True
for row in reader :
	if row['function'] == 'S' : 
		ok = ok and float( row['optimize_lower'] )   == 0.0 
		ok = ok and float( row['optimize_initial'] ) == 0.01
		ok = ok and float( row['optimize_upper'] )   == 1.0 

		ok = ok and row['prior_like']                == 'gaussian'
		ok = ok and float(row['prior_zeta'])         == 1.
		ok = ok and row['prior_sigma']               == 'inf'
		ok = ok and float(row['prior_mean'] )        == 0.

		ok = ok and row['age_like']                  == 'gaussian'
		ok = ok and float(row['age_zeta'] )          == 1.
		ok = ok and float(row['age_sigma'] )         == .02
		ok = ok and int(row['age_order'])            == 1

		ok = ok and row['cohort_like']               == 'laplace'
		ok = ok and float(row['cohort_zeta'] )       == 1.
		ok = ok and float(row['cohort_sigma'] )      == .03
		ok = ok and int(row['cohort_order'])         == 2

		ok = ok and row['cross_like']                == 'laplace'
		ok = ok and float(row['cross_zeta'] )        == 2.0
		ok = ok and float(row['cross_sigma'] )       == .03
	if not ok :
		print row
		sys.exit('Error in d4_change_sfun_in.py')
sys.exit(None)
EOF
$top_builddir/python.sh \
	check_sfun_in.py
# @@
#
# @head Final Message@@
# If this shell script reaches here, then test has passed.
# @codep
echo "change_sfun_in.sh: OK"
# @@
#
# @end
