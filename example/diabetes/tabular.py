#! /usr/bin/python
# ----------------------------------------------------------------------------
file_in='measure_in.csv'
# ----------------------------------------------------------------------------
def round_str(s) :
	return str( int( round( float( s ) ) ))
import re
import sys
usage='''
extract_table.py: integrand
creates the output file integrand.tex with the corresponding data where
integrand: is the integrad (either m_all or relative_risk).
'''
argv  = sys.argv
error = len(argv) != 2
if not error :
	error = argv[1] != 'm_all' and argv[1] != 'relative_risk' 
if error :
	print usage
	for i in range( len(argv) - 1 ) :
		print 'argv[', i+1 ,']=', argv[i+1]
	sys.exit(1)
integrand = argv[1]
file_out   = integrand + '.tex'
print 'Create ' + file_out
#
file_ptr     = open(file_in, 'r')
data         = file_ptr.read()
file_ptr.close()
#
line_p   = integrand +',([^,]*),[^,]*,([^,]*),[^,]*,[^,]*,[^,]*,[^,]*,([^,]*),'
line_p   = re.compile(line_p)
iterator = line_p.finditer(data)
age          = []
year         = []
value        = []
for match in iterator :
	age.append( match.group(1) )
	year.append( match.group(2) )
	value.append( match.group(3) )
#
table  = '\\begin{tabular}{|r|r'
n_year = 1
while age[n_year-1] == age[n_year] :
	table  = table + 'r'
	n_year = n_year + 1
n_age  = len( age ) / n_year
#
table  = table + '|}\n'
table  = table + '\\hline\n'
table  = table + '    & \\multicolumn{4}{c|}{Year} \\\\\n'
line   = ' Age'
for index in range(n_year) :
	line = line + '& ' + round_str(year[index])
line   = line + ' \\\\\n'
table  = table + line
table  = table + '\\hline\n'
for index in range( len(value) ) :
	if index % n_year == 0 :
		line = ' ' + round_str( age[index] )
	line = line + '& ' + ( '%g' % float( value[index] ) )
	if index % n_year == (n_year - 1) :
		line  = line + ' \\\\\n'
		table = table + line
table  = table + '\\hline\n'
table  = table + '\\end{tabular}\n'
#
file_ptr = open(file_out, 'w')
file_ptr.write(table)
file_ptr.close()
