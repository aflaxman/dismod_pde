#! /bin/bash -e
#
srcdir="../../../example/diabetes" # directory where this file is stored
file_out="measure_in.csv"          # file we are writing out
cv_prevalence=".1"                 # 10% coefficient of variation
zeta_data='1e-6'                   # zeta for all values
age_lower_min="1."                 # lower limit for age
risk_30="5.0"                      # relative risk at age 30
risk_80="1.5"                      # relative rist at age 80
#
# --------------------------------------------------------------------------
# http://www.mortality.org/hmd/AUT/STATS/Deaths_1x1.txt
# http://www.mortality.org/hmd/AUT/STATS/Population.txt
# convert from ascii columns to csv file
for file in deaths population
do
	echo "create $file.csv"
	sed < $srcdir/$file.txt > $file.csv \
		-e '1,2d' -e 's|  *|,|g' -e '/110+,/d'
done
# --------------------------------------------------------------------------
cat << EOF > measure_in.py
import csv
import numpy
import math
# ------------------------------------------------------------------------
# write header for output file
file_out = open('$file_out', 'w')
writer   = csv.writer(file_out)
row_out  = [ 
	'integrand', 
	'age_lower', 
	'age_upper', 
	'time_lower', 
	'time_upper',
	'meas_like',
	'meas_zeta',
	'meas_sigma',
	'meas_value',
	'c_iso3'
]
writer.writerow(row_out)
# ------------------------------------------------------------------------
#
# read in m_all data
age      = range(20, 81, 10)
n_age    = len(age)
age      = numpy.array(age,  float)
#
year     = range(1980, 2011, 10)
n_year   = len(year)
year     = numpy.array(year, float)
for name in [ 'deaths', 'population' ] :
	file_in  = open( name + '.csv');
	reader   = csv.DictReader(file_in)
	data     = numpy.zeros( (n_age, n_year), float)
	for row in reader :
		a      = float( row['Age'] )
		y      = float( row['Year'] )
		a_flag = a == age
		y_flag = y == year
		if any( a_flag ) and any( y_flag ) :
			data[a_flag, y_flag] = float(row['Male'])
	if name == 'deaths' :
		death_data = data
	else :
		pop_data = data
#
# write out all cause data
meas_like  = 'gaussian'
meas_zeta  = $zeta_data
meas_sigma = 0.0
c_iso3     = 'AUT'
for j in range(n_age) :
	for k in range(n_year) :
		r_jk = '%6.5f' % ( death_data[j,k] / pop_data[j,k] )
		integrand  = 'm_all'
		age_lower  = age[j]
		age_upper  = age[j]
		time_lower = year[k]
		time_upper = year[k]
		meas_value = r_jk
		row_out = [ 
			integrand, 
			age_lower, 
			age_upper, 
			time_lower, 
			time_upper,
			meas_like,
			meas_zeta,
			meas_sigma,
			meas_value,
			c_iso3
		]
		writer.writerow(row_out)
# -----------------------------------------------------------------------------
age      = range(30, 81, 10)
n_age    = len(age)
age      = numpy.array(age,  float)
#
integrand  = 'relative_risk'
meas_like  = 'gaussian'
meas_zeta  = $zeta_data
meas_sigma = 0.0
c_iso3     = 'AUT'
risk_30    = $risk_30
risk_80    = $risk_80
for j in range(n_age) :
	age_lower = age[j]
	age_upper = age[j]
	# linear function of age that is risk_30 at age 30 and risk_80 at age 80.
	meas_value = risk_80 + (risk_30-risk_80) * (80.-age[j]) /(80.-30.)
	meas_value = round( meas_value , 3 );
	for k in range(n_year) :
		time_lower = year[k]
		time_upper = year[k]
		row_out = [ 
			integrand, 
			age_lower, 
			age_upper, 
			time_lower, 
			time_upper,
			meas_like,
			meas_zeta,
			meas_sigma,
			meas_value,
			c_iso3
		]
		writer.writerow(row_out)
# -----------------------------------------------------------------------------
# References for prevalence data.
#
# Reference [1]
# National, regional, and global trends in fasting plasma glucose and diabetes 
# prevalence since 1980: ...
# Lancet, Volume 378, Issue 9785, Pages 31-40, 2 July 2011 
#
# Reference [2]
# Secular trends in cardiovascular risk factors: ...
# Journal of Internal Medicine Volume 261, Issue 6, pages 566-576, June 2007
# -----------------------------------------------------------------------------
# Data in Reference [2], Table 3, Glucose (mg / dL), Males 
data_list = [
	{ 'clow' : 1905, 'cup' : 1914 , 'alow' : 70 , 'aup' : 79 , 'G' :  86.7  },
	{ 'clow' : 1915, 'cup' : 1924 , 'alow' : 70 , 'aup' : 79 , 'G' :  96.1  },
	{ 'clow' : 1925, 'cup' : 1934 , 'alow' : 70 , 'aup' : 79 , 'G' : 102.0  },
	{ 'clow' : 1915, 'cup' : 1924 , 'alow' : 60 , 'aup' : 69 , 'G' :  90.2  },
	{ 'clow' : 1925, 'cup' : 1934 , 'alow' : 60 , 'aup' : 69 , 'G' :  95.1  },
	{ 'clow' : 1935, 'cup' : 1944 , 'alow' : 60 , 'aup' : 69 , 'G' : 100.9  },
	{ 'clow' : 1925, 'cup' : 1934 , 'alow' : 50 , 'aup' : 59 , 'G' :  87.3  },
	{ 'clow' : 1935, 'cup' : 1944 , 'alow' : 50 , 'aup' : 59 , 'G' :  94.1  },
	{ 'clow' : 1945, 'cup' : 1954 , 'alow' : 50 , 'aup' : 59 , 'G' :  99.1  },
	{ 'clow' : 1935, 'cup' : 1944 , 'alow' : 40 , 'aup' : 49 , 'G' :  83.8  },
	{ 'clow' : 1945, 'cup' : 1954 , 'alow' : 40 , 'aup' : 49 , 'G' :  90.1  },
	{ 'clow' : 1955, 'cup' : 1964 , 'alow' : 40 , 'aup' : 49 , 'G' :  93.0  },
	{ 'clow' : 1945, 'cup' : 1954 , 'alow' : 30 , 'aup' : 39 , 'G' :  80.5  },
	{ 'clow' : 1955, 'cup' : 1964 , 'alow' : 30 , 'aup' : 39 , 'G' :  85.7  },
	{ 'clow' : 1965, 'cup' : 1974 , 'alow' : 30 , 'aup' : 39 , 'G' :  89.6  },
	{ 'clow' : 1955, 'cup' : 1964 , 'alow' : 20 , 'aup' : 29 , 'G' :  77.9  },
	{ 'clow' : 1965, 'cup' : 1974 , 'alow' : 20 , 'aup' : 29 , 'G' :  82.5  },
	{ 'clow' : 1975, 'cup' : 1984 , 'alow' : 20 , 'aup' : 29 , 'G' :  92.1  }
]
# -----------------------------------------------------------------------------
# In Reference [1], Supplementary webappendix, Webtable 5 we have
# L(P) = - 16.38 + 7.64 * log(F) - 6.46 * H + 0.098 * A + 0.029 * S 
#        + 3.57  * H * log(F) - 0.046 * A * log(F)
# where
# L = logit function; i.e., L(P) = log[ P / (1-P) ]
# P = prevalence of diabetes as a fraction of current population 
# F = fasting plasma glucose in millimoles/liter     = G / 18
# G = fasting plasma glucose in milligrams/deciliter = F * 18
# H = 1 for high income country, 0 otherwise
# S = 1 for female, 0 otherwise
# A = age in years
#
H           = 1.0
S           = 0.0
for data in data_list :
	G = data['G']
	F = G / 18.0 
	A = (data['alow'] + data['aup'] + 1) / 2.
	C = (data['clow'] + data['cup'] + 1) / 2.
	L = - 16.38 + 7.64 * math.log(F) - 6.46 * H + 0.098 * A + 0.029 * S 
	L = L + 3.57  * H * math.log(F) - 0.046 * A * math.log(F)
	P = math.exp(L) / (1.0 + math.exp(L)) 
	T = A + C
	#
	age_lower  = A
	age_upper  = A
	time_lower = T
	time_upper = T
	meas_value = P
	meas_like  = 'gaussian'
	integrand  = 'prevalence'
	ciso3      = 'AUT'
	meas_sigma = math.log( $cv_prevalence + 1. )
	meas_zeta  = $zeta_data
	year       = T
	# ------------------------------------------------------------------
	meas_zeta  = '%.2e' % meas_zeta
	meas_sigma = '%.2e' % meas_sigma
	meas_value = '%.4f' % meas_value
	# -------------------------------------------------------------------
	assert $age_lower_min < age_lower
	#
	row_out = [ 
		integrand, 
		age_lower, 
		age_upper, 
		time_lower, 
		time_upper,
		meas_like,
		meas_zeta,
		meas_sigma,
		meas_value,
		c_iso3
	]
	writer.writerow(row_out)
EOF
# --------------------------------------------------------------------------
echo "creating $file_out"
../../python.sh measure_in.py 
# --------------------------------------------------------------------------
