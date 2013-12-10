/*
$begin dismod_pde_csv$$
$escape $$
$spell
	pde
	inf
	covariate
	covariates
	omega
	dismod_pde
	csv
	sfun
	optimizer
$$

$section dismod_pde Command Line Program$$
$index program, dismod_pde_csv$$
$index dismod_pde_csv, program$$

$head Syntax$$
$codei%dismod_pde_csv%$$
$icode%parameter.csv% %measure_in.csv% %sfun_in.csv%
%$$
$codei%dismod_pde_csv %parameter.csv%  \
	%measure_in.csv%   %measure_out.csv% \
	%sfun_in.csv%      %sfun_out.csv% \
	%effect_in.csv%    %effect_out.csv% 
%$$

$head Purpose$$
Runs $cref/dismod_pde::optimize/optimize/$$ with inputs
and outputs from and to csv files.

$head CSV Files$$
Each of the $code dismod_pde_csv$$ command line arguments
specify a $cref/dismod_pde csv file/csv/dismod_pde CSV Files/$$.
This dismod_pde_csv uses the following csv files for its input and output:
$table
$cref/parameter.csv/dismod_pde_csv/parameter.csv/$$ $pre  $$ $cnext
	a small set of control parameters
$rnext
$cref/effect_in.csv/dismod_pde_csv/effect_in.csv/$$ $pre  $$ $cnext
	measurement effects, limits, and initial values during fitting
$rnext
$cref/measure_in.csv/dismod_pde_csv/measure_in.csv/$$ $pre  $$ $cnext 
	defines the measurements
$rnext
$cref/sfun_in.csv/dismod_pde_csv/sfun_in.csv/$$ $pre  $$ $cnext 
	stochastic function input values
$rnext
$cref/sfun_out.csv/dismod_pde_csv/sfun_out.csv/$$ $pre  $$ $cnext 
	stochastic function output values
$rnext
$cref/measure_out.csv/dismod_pde_csv/measure_out.csv/$$  $pre  $$ $cnext
	fitting residuals for each measurement 
$tend

$head Infinity$$
$index infinity$$
These data files use the special values $code inf$$ and $code -inf$$ to denote
plus and minus infinity.

$comment -------------------------------------------------------------- $$

$head parameter.csv$$
$index parameter.csv$$
The command line argument $icode parameter.csv$$ specifies
the parameter file.
This file has only two columns, the first column name is $code name$$
and the second column name is $code value$$.
For each name, the following table contains a description of the
corresponding value (that the modeler chooses).
The order of the lines in $icode parameter.csv$$ is not important.
All of the names must appear once and only once.
Example value,
used by $cref get_started.sh$$, are listed below for each parameter.
$table
$bold name$$         $pre $$ $cnext 
	$bold Default$$ $pre $$ $cnext 
	$bold Description$$ 
$rnext
$cref/age_grid/dismod_pde_csv/parameter.csv/age_grid/$$ $pre $$ $cnext
	0 5 20 $pre $$ $cnext
	Age values in the computational grid.
$rnext
$cref/cohort_grid/dismod_pde_csv/parameter.csv/cohort_grid/$$ $pre $$ $cnext
	1940 1960 1980 200 2020 2040 $pre $$ $cnext
	Cohort values in the computational grid.
$rnext
$cref/max_ode_step/optimize/parameter/max_ode_step/$$  $pre $$ $cnext
	1 $pre $$ $cnext
	Maximum step to use in age when solving for 
	$latex S(\cdot)$$ and $latex C(\cdot)$$.
$rnext
$cref/print_level/optimize/parameter/print_level/$$  $pre $$ $cnext
	5 $pre $$ $cnext
	Level of 
	$cref/tracing printed by optimization/ipopt_trace/$$ (0 for no tracing).
$rnext
$cref/max_iteration/optimize/parameter/max_iteration/$$  $pre $$ $cnext
	80 $pre $$ $cnext
	Maximum number of iterations the optimizer should attempt
	before giving up.
$rnext
$cref/
converge_tolerance/optimize/parameter/converge_tolerance/$$  $pre $$ $cnext
	1e-6 $pre $$ $cnext
	Optimizer convergence tolerance.
$rnext
$cref/
debug_output/optimize/parameter/debug_output/$$  $pre $$ $cnext
	0 $pre $$ $cnext
	More debugging output.
$tend 

$subhead age_grid$$
$index grid, age$$
$index age, grid$$
The value corresponding to the name $code age_grid$$
is a vector with elements separated by one or more spaces.
It must be monotone increasing.

$subhead cohort_grid$$
$index grid, cohort$$
$index cohort, grid$$
The value corresponding to the name $code cohort_grid$$
is a vector with elements separated by one or more spaces.
It must be monotone increasing.
The limits of the cohort grid must be large enough to surround the
data in time.


$subhead debug_output$$
In $icode parameter.csv$$ file,
this value must be either $code 0$$ (false) or $code 1$$ (true).
If it is $code 1$$, more debugging output is displayed 
(possibly at the expense of slower execution).

$comment -------------------------------------------------------------- $$

$head measure_in.csv$$
$index measure_in.csv$$
The command line argument $icode measure_in.csv$$ specifies
the measurement file which must contain the following columns:

$table
$bold Name$$ $cnext $bold Value$$ $rnext
$cref/integrand/dismod_pde_csv/measure_in.csv/integrand/$$ $cnext
	The $cref/integrand/optimize/measure_in/integrand/$$
	used to model this measurement.
$rnext
$cref/age_lower, age_upper/
	dismod_pde_csv/measure_in.csv/age_lower, age_upper/$$ $cnext 
	The age limits for this measurement.
$rnext
$cref/time_lower, time_upper/
	dismod_pde_csv/measure_in.csv/time_lower, time_upper/$$ $cnext 
	The time limits for this measurement.
$rnext
$cref/meas_like/dismod_pde_csv/measure_in.csv/meas_like/$$ $cnext
	name of the likelihood for this measurement.
$rnext
$cref/meas_zeta/dismod_pde_csv/measure_in.csv/meas_zeta/$$ $cnext
	offset that is added before log scaling this measurement.
$rnext
$cref/meas_sigma/dismod_pde_csv/measure_in.csv/meas_sigma/$$ $cnext
	standard deviation for this measurement.
$rnext
$cref/meas_value/dismod_pde_csv/measure_in.csv/meas_value/$$ $cnext
	The value for this measurement. 
$rnext
$cref/Covariates/dismod_pde_csv/measure_in.csv/Covariates/$$ $cnext
	Zero or more covariate columns.
$rnext
$cref/Comments/dismod_pde_csv/measure_in.csv/Comments/$$  $cnext
	Zero or more comment columns.
$tend

$table
$bold Name$$ $cnext $bold Value$$ $rnext
$codei%x_%name%$$ $cnext
	value of the specified covariate for this measurement
$tend

$subhead ell$$
We use $latex \ell$$ to denote an index that starts at zero
with the first value for each column.
Since the first line contains the names for each column
the first value for each column is in line number two; 
i.e. $latex \ell = 0$$ corresponds to line number two.

$subhead integrand$$
$index integrand$$
The column with name $code integrand$$ 
must have one of the following values:
$code incidence$$,
$code remission$$.
$code m_excess$$,
$code m_other$$,
$code m_with$$,
$code prevalence$$
$code m_prevalence$$,
$code m_specific$$,
$code m_all$$,
$code m_standard$$,
$code relative_risk$$.
We use $latex i ( \ell )$$ to denote the value for the integrand; see
optimizer $cref/integrand/optimize/measure_in/integrand/$$.

$subhead age_lower, age_upper$$
$index age, limits$$
$index upper, age$$
$index lower, age$$
The column with name $code age_lower$$ ($code age_upper$$) 
contains the lower (upper) limit for integration with respect to age
for this measurement.
The lower age limit must be greater than zero.
We use $latex a_{l, \ell}$$ ($latex a_{u,\ell}$$) to denote the
corresponding limits; see optimizer 
$cref/age_lower, age_upper/optimize/measure_in/age_lower, age_upper/$$.

$subhead time_lower, time_upper$$
$index time, limits$$
$index upper, time$$
$index lower, time$$
The column with name $code time_lower$$ ($code time_upper$$) 
contains the lower (upper) limit for integration with respect to time
for this measurement.
We use $latex t_{l, \ell}$$ ($latex t_{u,\ell}$$) to denote the
corresponding limits; see optimizer
$cref/time_lower, time_upper/optimize/measure_in/time_lower, time_upper/$$.

$subhead meas_like$$
$index likelihood, name$$
$index name, likelihood$$
The column with name $code meas_like$$ 
must have one of the following values:
$code gaussian$$ ($latex h_\ell = 2$$), or 
$code laplace$$ ($latex h_\ell = 1$$).
We use $latex h_\ell$$ to denote which likelihood we are using; 
see optimizer $cref/meas_like/optimize/measure_in/meas_like/$$.

$subhead meas_zeta$$
$index likelihood, zeta$$
$index zeta, likelihood$$
The column with name $code meas_zeta$$ 
contains the offset $latex \zeta_\ell$$
that is added to the measurement, and the model,
before taking the log;
see optimizer $cref/meas_zeta/optimize/measure_in/meas_zeta/$$.

$subhead meas_sigma$$
$index likelihood, sigma$$
$index sigma, likelihood$$
The column with name $code meas_sigma$$ 
contains the standard deviation $latex \sigma_\ell$$ for this measurement;
see optimizer $cref/meas_sigma/optimize/measure_in/meas_sigma/$$.
If $latex \sigma_\ell$$ is $cref/infinity/dismod_pde_csv/Infinity/$$,
the measurement is not included in the fit.
If $latex \sigma_\ell$$ is zero,
The measurement is treated like a constraint; i,e.,
the fit must agree with no residual.

$subhead meas_value$$
The column with name $code meas_value$$ contains the values
for this measurement $latex z_\ell$$;
see optimizer $cref/meas_value/optimize/measure_in/meas_value/$$.

$subhead Covariates$$
$index covariates$$
Columns in the file $icode measure_in.csv$$ that have
names beginning with the two characters $code x_$$
are covariate columns and corresponding measurement effects
can be estimated; see
$cref/effect_in.csv/dismod_pde_csv/effect_in.csv/$$.

$subhead Comments$$
$index Comment$$
The file $icode measure_in.csv$$ may contain
one or more comment columns.
The corresponding column names begin with the characters 
$code c_$$:

$comment -------------------------------------------------------------- $$

$head sfun_in.csv$$
$index sfun_in.csv$$

$subhead Output Template$$
$index sfun_in.csv, template$$
$index template, sfun_in.csv$$
If $icode sfun_in.csv$$ is the last command line argument,
this is an output file and its contents only depend on the values
in $icode parameter.csv$$ and $icode measurement.csv$$.
In this case, $icode sfun_in.csv$$ it is a template for 
the corresponding input file (when it is not the last command line argument).
See the heading Input File directly below.

$subhead Input File$$
$index sfun_in.csv, input$$
$index input, sfun_in.csv$$
If $icode sfun_in.csv$$ is not the last command line argument,
it is an input file with the column names listed below.
You cannot change any of the column names; i.e., 
the names in the first row of the template version of 
$icode sfun_in.csv$$ (see the heading Output Template directly above).
The following values are fixed for each line and should not be changed:
$table
$bold Name$$ 
	$cnext $bold Value$$ 
$rnext
$cref/age/optimize/grid/age/$$   
	$cnext age $latex a$$ corresponding to this line
$rnext
$cref/cohort/optimize/grid/cohort/$$   
	$cnext cohort $latex c$$ corresponding to this line
$rnext
$code time$$  
	$cnext time $latex t = a + c$$ corresponding to this line
$rnext
$cref/function/dismod_pde_csv/sfun_in.csv/function/$$ 
	$cnext stochastic function index $latex q$$ corresponding to this line
$tend
The following values are input for each line 
and should be chosen by the modeler:
$table
$bold Name$$ 
	$cnext $bold Value$$ 
$rnext
$cref/optimize_zeta/optimize/sfun_in/optimize_zeta/$$ 
	$cnext offset added before log scaling of optimization problem
$rnext
$cref/optimize_lower/optimize/sfun_in/optimize_lower/$$ 
	$cnext lower bound for $latex s^q (a, c)$$ 
		during the optimization procedure
$rnext
$cref/optimize_initial/optimize/sfun_in/optimize_initial/$$ 
	$cnext initial value for $latex s^q (a, c)$$ 
		where the optimization procedure begins 
$rnext
$cref/optimize_upper/optimize/sfun_in/optimize_upper/$$ 
	$cnext upper bound for $latex s^q (a, c)$$ 
		during the optimization procedure
$rnext
$cref/prior_like/optimize/sfun_in/prior_like/$$ 
	$cnext direct prior likelihood name for $latex s^q (a, c)$$
$rnext
$cref/prior_zeta/optimize/sfun_in/prior_zeta/$$ 
	$cnext offset added before log in direct prior residual for
	$latex s^q (a, c)$$ 
$rnext
$cref/prior_sigma/optimize/sfun_in/prior_sigma/$$ 
	$cnext standard deviation for direct prior residual for
	$latex s^q (a, c)$$ 
$rnext
$cref/prior_mean/optimize/sfun_in/prior_mean/$$ 
	$cnext direct prior value for 
	$latex s^q (a, c)$$ 
$rnext
$cref/age_like/optimize/sfun_in/age_like/$$ 
	$cnext age direction forward difference likelihood for 
	$latex s^q(a, c)$$
$rnext
$cref/age_zeta/optimize/sfun_in/age_zeta/$$ 
	$cnext added before log for age direction forward differences for 
	$latex s^q (a, c)$$
$rnext
$cref/age_sigma/optimize/sfun_in/age_sigma/$$ 
	$cnext standard deviation for age direction forward differences for 
	$latex s^q (a, c)$$
$rnext
$cref/age_order/optimize/sfun_in/age_order/$$ 
	$cnext order for age direction forward differences for $latex s^q (a, c)$$
$rnext
$cref/cohort_like/optimize/sfun_in/cohort_like/$$ 
	$cnext cohort direction forward difference likelihood for 
	$latex s^q(a, c)$$
$rnext
$cref/cohort_zeta/optimize/sfun_in/cohort_zeta/$$ 
	$cnext added before log for cohort direction forward differences for 
	$latex s^q (a, c)$$
$rnext
$cref/cohort_sigma/optimize/sfun_in/cohort_sigma/$$ 
	$cnext standard deviation for cohort direction forward differences 
	for $latex s^q (a, c)$$
$rnext
$cref/cohort_order/optimize/sfun_in/cohort_order/$$ 
	$cnext order for cohort direction forward differences for 
	$latex s^q (a, c)$$
$rnext
$cref/cross_like/optimize/sfun_in/cross_like/$$ 
	$cnext cross term forward difference likelihood for 
	$latex s^q(a, c)$$
$rnext
$cref/cross_zeta/optimize/sfun_in/cross_zeta/$$ 
	$cnext added before log for cross term forward differences for 
	$latex s^q (a, c)$$
$rnext
$cref/cross_sigma/optimize/sfun_in/cross_sigma/$$ 
	$cnext standard deviation for cross term forward differences 
	for $latex s^q (a, c)$$
$tend

$subhead function$$
$index function, column$$
$index column, function$$
$index field, function$$
The value in the column with name $code function$$ specifies the function 
for the corresponding line 
and has one of the following values :
$code S$$,
$code C$$,
$code iota$$,
$code rho$$,
$code chi$$, or
$code omega$$.
For more details, see the 
$cref/stochastic functions/optimize/Stochastic Functions/$$ 
discussion in the optimizer documentation.

$subhead name$$
$index likelihood, name$$
$index name, likelihood$$
The columns with names
$code prior_like$$,
$code age_like$$, and
$code cohort_like$$,
must value $code gaussian$$ or $code laplace$$.
This specifies the corresponding likelihood for the residuals.

$subhead zeta$$
$index zeta$$
The columns with names
$code prior_zeta$$,
$code age_zeta$$, and
$code cohort_zeta$$,
contains the offset added before taking the log
during the computation of the corresponding residuals.

$subhead sigma$$
$index sigma$$
The columns with names
$code prior_sigma$$,
$code age_sigma$$, and
$code cohort_sigma$$,
contains the standard deviation for the corresponding residuals.

$subhead order$$
$index age, order$$
$index cohort, order$$
$index order, age and cohort$$
The columns with name $code age_order$$ and $code cohort_order$$ 
must have value $code 1$$ or $code 2$$.
These specify the order for the forward differences
in the age and cohort directions.
Note that the functions $code S$$ and $code C$$
can only use order $code 1$$ in the age direction.

$comment -------------------------------------------------------------- $$

$head effect_in.csv$$
$index effect_in.csv$$
The command line argument $icode effect_in.csv$$ specifies
the measurement effect input information .
This file has five columns with the following names:
$code integrand$$, 
$code covariate$$, 
$code lower$$,
$code initial$$, and 
$code upper$$.

$subhead integrand$$
$index integrand$$
The column with name $code integrand$$ 
must have one of the following values:
$code incidence$$,
$code m_all$$,
$code m_excess$$,
$code m_other$$,
$code m_prevalence$$,
$code m_specific$$,
$code relative_risk$$, or
$code remission$$.
We use $latex i ( m )$$ to denote the integrand
for the $th m$$ effect; see optimizer 
$cref/integrand/optimize/effect_in/integrand/$$.

$subhead covariate$$
$index x_, covariate$$
$index covariate, name x_$$
$index name, covariate x_$$
Each value in the $code covariate$$ column
must be one of the 
$cref/covariate/dismod_pde_csv/measure_in.csv/Covariates/$$ names in
the $icode measure_in.csv$$ file
(hence it must begin with the characters $code x_$$).
Only covariate effects that appear in the $icode effect_in.csv$$ file are
included in the fit.
We use $latex n(m)$$ to denote the covariate
for the $th m$$ effect; see optimizer 
$cref/covariate/optimize/effect_in/covariate/$$.

$subhead lower$$
$index covariate, effect lower$$
$index lower, covariate effect$$
$index effect, covariate lower$$
The lower limit 
(constraint on fit) for this covariate effect. 
The value $code -inf$$ can be used to denote minus 
$cref/infinity/dismod_pde_csv/Infinity/$$; i.e.,
no lower limit.

$subhead initial$$
$index covariate, effect initial$$
$index initial, covariate effect$$
$index effect, covariate initial$$
The initial value (where the optimization procedure begins)
for this covariate effect.
For each covariate effect, $icode initial$$ must satisfy
$codei%
	%lower% <= %initial% <= %upper%
%$$

$subhead upper$$
$index covariate, effect upper$$
$index upper, covariate effect$$
$index effect, covariate upper$$
The upper limit 
(constraint on fit) for this covariate effect. 
The value $code inf$$ can be used to denote plus 
$cref/infinity/dismod_pde_csv/Infinity/$$; i.e.,
no lower limit.
It is ok if 
$codei%
	%lower% == %initial% == %upper%
%$$
in which case the corresponding value is used for this covariate effect,
and it is not included in the optimization procedure.


$comment -------------------------------------------------------------- $$


$head sfun_out.csv$$
$index sfun_out.csv$$
If this command line argument is present,
this is an output file that contains information for each point
on the computational grid.
$table
$bold Name$$ 
	$cnext $bold Value$$ 
$rnext
$code age$$   
	$cnext age $latex a$$ for this line of the file
$rnext
$code cohort$$ 
	$cnext cohort $latex c$$ for this line of the file
$rnext
$code time$$  
	$cnext time $latex t = a + c$$ for this line of the file
$rnext
$code function$$ 
	$cnext stochastic function index $latex q$$ for this line of the file,
$rnext
$cref/estimate/optimize/sfun_out/estimate/$$ 
	$cnext final optimal estimate for $latex s^q (a, c)$$ 
$rnext
$cref/prior_residual/optimize/sfun_out/prior_residual/$$ 
	$cnext direct prior residual for $latex s^q (a, c)$$
$rnext
$cref/prior_scaled/optimize/sfun_out/prior_scaled/$$ 
	$cnext scaled direct prior residual for $latex s^q (a, c)$$
$rnext
$cref/age_residual/optimize/sfun_out/age_residual/$$ 
	$cnext age direction residual for $latex s^q (a, c)$$
$rnext
$cref/age_scaled/optimize/sfun_out/age_scaled/$$ 
	$cnext scaled age direction residual for $latex s^q (a, c)$$
$rnext
$cref/cohort_residual/optimize/sfun_out/cohort_residual/$$ 
	$cnext cohort direction residual for $latex s^q (a, c)$$
$rnext
$cref/cohort_scaled/optimize/sfun_out/cohort_scaled/$$ 
	$cnext scaled cohort direction residual for $latex s^q (a, c)$$
$rnext
$cref/cross_residual/optimize/sfun_out/cross_residual/$$ 
	$cnext cross term residual for $latex s^q (a, c)$$
$rnext
$cref/cross_scaled/optimize/sfun_out/cross_scaled/$$ 
	$cnext scaled cross term residual for $latex s^q (a, c)$$
$tend
Note that the output file has special structure in the following way:

$list number$$
Output lines are in groups of six with the same value for $latex (a, c)$$;
i.e., they corresponding to $latex s^q (a, c)$$ 
for $latex q = 0 , \ldots , 5$$.
$lnext
The function index $latex q$$ on each line in a group of six is different
corresponds to one of the following stochastic functions:
$code S$$, $code C$$, $code iota$$, $code rho$$, $code chi$$, or $code omega$$.
$lend


$subhead Scaled Residuals$$
$index scaled, residual$$
$index residual, scaled$$
$list number$$
If a corresponding standard deviation $latex \sigma = 0 $$ or
$latex \sigma = \infty$$, the scaled residual is zero.
$lnext
Residuals that are not defined are set to zero.
For example, the forward difference age residuals at the 
maximum age value.
$lnext
If $latex 0 < \sigma < \infty$$, and it corresponds to a prior residual,
the scaled residual is the residual divided $latex \sigma$$.
$lnext
If $latex 0 < \sigma < \infty$$, and it corresponds to a defined
age (cohort) residual,
the scaled residual is the residual divided $latex \sigma$$
times the square root of the corresponding age (cohort) values.
$lend

$comment -------------------------------------------------------------- $$

$head measure_out.csv$$
$index measure_out.csv$$
If this command line argument is present,
all of the columns present in 
$cref/measure_in.csv/dismod_pde_csv/measure_in.csv/$$ 
are copied to $icode measure_out.csv$$.
In addition, this output file that contains information for each measurement.
$table
$bold Name$$ 
	$cnext $bold Value$$ 
$rnext
$icode age$$   
	$cnext midpoint of the age interval for this measurement
$rnext
$icode cohort$$ 
	$cnext midpoint of the cohort interval for this measurement
$rnext
$icode time$$  
	$cnext midpoint of the time interval for this measurement.
$rnext
$cref/model/optimize/measure_out/model/$$
		$cnext model value for this measurement,
		corresponding to $cref/estimate/optimize/sfun_out/estimate/$$
$rnext
$cref/adjusted/optimize/measure_out/adjusted/$$
		$cnext covariate adjusted value for this measurement
$rnext
$cref/residual/optimize/measure_out/residual/$$
		$cnext residual value for this measurement,
		corresponding to estimate
$rnext
$cref/scaled/optimize/measure_out/scaled/$$
		$cnext scaled residual for this measurement,
		corresponding to the estimate
$tend


$comment -------------------------------------------------------------- $$

$head effect_out.csv$$
$index effect_out.csv$$
If this command line argument is present,
each line of this file contains information for a measurement effect.
This file has three columns named 
$code integrand$$, $code covariate$$ and $code effect$$

$subhead integrand$$
The column named $code integrand$$ specifies the
$cref/integrand/dismod_pde_csv/measure_in.csv/integrand/$$ 
column in the $icode measure_in.csv$$ file for this 
measurement effect.

$subhead covariate$$
The column named $code covariate$$ specifies the
$cref/covariate/dismod_pde_csv/measure_in.csv/Covariates/$$ 
column in the $icode measure_in.csv$$ file for this 
measurement effect.

$subhead effect$$
$index effect, measurement estimate$$
$index estimate, measurement effect$$
$index measurement, effect estimate$$
The value in the column named $code estimate$$ is the 
MAP estimate for this measurement effect.

$head Example$$
The bash script $cref get_started.sh$$ is a simple example and test
of $code dismod_pde_csv$$.

$end
--------------------------------------------------------------------------
*/

# include <string>
# include <fstream>
# include <dismod_pde/csv_read.hpp>
# include <dismod_pde/atof_char_vec.hpp>
# include <dismod_pde/create_grid.hpp>
# include <dismod_pde/double2stream.hpp>
# include <dismod_pde/like_name2enum.hpp>
# include <dismod_pde/optimize.hpp>
# include <dismod_pde/read_effect_in.hpp>
# include <dismod_pde/read_measure_in.hpp>
# include <dismod_pde/str_char_vec.hpp>
# include <dismod_pde/write_effect_out.hpp>
# include <dismod_pde/write_measure_out.hpp>
# include <dismod_pde/output.hpp>

namespace { // BEGIN empty namespace



void write_sfun_out(
	const std::string&                                  file     , 
	const dismod_pde::pair2index&                          grid     ,
	const dismod_pde::vector<dismod_pde::Sfun_Output_Struct>& sfun_out )
{	using dismod_pde::vector;
	using dismod_pde::double2stream;
	using std::cerr;
	using std::endl;
	using dismod_pde::output;

	// some temporaries
	size_t i, j, k, q;

	//
	// some constants 
	//
	const size_t         n_grid        = grid.size();
	const vector<double> age           = grid.age();
	const vector<double> cohort        = grid.cohort();
	const vector<std::string> function = dismod_pde::stochastic_enum.name();
	const size_t n_stochastic          = function.size();
	//
	const char* sfun_out_field[] = {
		"age",
		"cohort",
		"time",
		"function",
		"estimate",
		"prior_residual",
		"prior_scaled",
		"age_residual",
		"age_scaled",
		"cohort_residual",
		"cohort_scaled",
		"cross_residual",
		"cross_scaled"
	};
	const size_t n_field = sizeof(sfun_out_field) / sizeof(sfun_out_field[0]);

	// open the file and discard any current contents (truncate)
	std::ofstream fs( file.c_str(), std::ios_base::trunc );

	// write field names on first line of file
	for(j = 0; j < n_field; j++)
	{	fs << sfun_out_field[j];
		if( j != n_field - 1 )
			fs << ",";
	}
	fs << std::endl;

	//
	size_t i_grid;
	for(i_grid = 0; i_grid < n_grid; i_grid++)
	{	grid.unpack(j, k, i_grid);
		double time          = age[j] + cohort[k];
		for(q = 0; q < n_stochastic; q++)
		{	i = i_grid * n_stochastic + q;
			// next field value line in sfun_out.csv
			fs << age[j]        << ",";
			fs << cohort[k]     << ",";
			fs << time          << ",";
			fs << function[q]   << ","; 
			output(fs, sfun_out[i].estimate       ); fs << ",";
			output(fs, sfun_out[i].prior_residual ); fs << ",";
			output(fs, sfun_out[i].prior_scaled   ); fs << ",";
			output(fs, sfun_out[i].age_residual   ); fs << ",";
			output(fs, sfun_out[i].age_scaled     ); fs << ",";
			output(fs, sfun_out[i].cohort_residual); fs << ",";
			output(fs, sfun_out[i].cohort_scaled  ); fs << ",";
			output(fs, sfun_out[i].cross_residual);  fs << ",";
			output(fs, sfun_out[i].cross_scaled  );  fs << endl;
		}
	}
	fs.close();
}


void set_sfun_in(
	const std::string&                              file       ,             
	const dismod_pde::pair2index&                      grid       ,
	dismod_pde::vector<dismod_pde::Sfun_Input_Struct>&    sfun_in    )
{
	using std::cerr;
	using std::endl;

	using dismod_pde::vector;
	using dismod_pde::atof_char_vec;
	using dismod_pde::str_char_vec;
	using dismod_pde::like_name2enum;

	// some temporaries
	size_t i, j, k, q;
	double d;
	std::string str;

	//
	// some constants 
	//
	const size_t         n_grid        = grid.size();
	const vector<double> age           = grid.age();
	const vector<double> cohort        = grid.cohort();
	const vector<std::string> function = dismod_pde::stochastic_enum.name();
	const size_t n_stochastic          = function.size();
	//
	const char* sfun_in_field[] = {
		"age",
		"cohort",
		"time",
		"function",

		"optimize_zeta",
		"optimize_lower",
		"optimize_initial",
		"optimize_upper",

		"prior_like",
		"prior_zeta",
		"prior_sigma",
		"prior_mean",

		"age_like",
		"age_zeta",
		"age_sigma",
		"age_order",

		"cohort_like",
		"cohort_zeta",
		"cohort_sigma",
		"cohort_order",

		"cross_like",
		"cross_zeta",
		"cross_sigma"
	};
	const size_t n_field = sizeof(sfun_in_field) / sizeof(sfun_in_field[0]);

	// sfun_in file field names
	vector<std::string> field(n_field);
	for(i = 0; i < n_field; i++)
		field[i] = sfun_in_field[i];

	// call csv_read
	vector<char>   raw;
	vector< std::pair<size_t,size_t> > sub_vec;
	dismod_pde::csv_read(file, field, raw, sub_vec);
	assert( sub_vec.size() % n_field == 0 );
	if( n_grid * n_stochastic + 1 != sub_vec.size() / n_field )
	{	cerr << endl;
		cerr << "dismod_csv: expected " << n_grid * n_stochastic + 1;
		cerr << " lines in the file " << file;
		cerr << " but found " << sub_vec.size() / n_field;
		cerr << endl;
		cerr << "Try starting from a new sfun_in.csv template file.";
		cerr << endl;
		std::exit(1);
	}

	// now size and fill in sfun_in
	sfun_in.resize(n_grid * n_stochastic);
	size_t i_grid;
	size_t line = 0;
	size_t i_sub_vec;
	bool   ok;
	for(i_grid = 0; i_grid < n_grid; i_grid++) 
	{	grid.unpack(j, k, i_grid);
		for(q = 0; q < n_stochastic; q++)
		{
			// index into data file (data starts at second line)
			++line; 
			// index into sfun_in vector
			i  = i_grid * n_stochastic + q;
			// initialize check flag
			ok = true;
			// initialize index into the sub_vec structure
			i_sub_vec = line * n_field;
			// check age
			d   = atof_char_vec(raw, sub_vec[i_sub_vec++], line, file);
			ok &= fabs(d - age[j]) < 1e-2;
			// check cohort
			d   = atof_char_vec(raw, sub_vec[i_sub_vec++], line, file);
			ok &= fabs(d - cohort[k]) < 1e-2;
			// check time
			d   = atof_char_vec(raw, sub_vec[i_sub_vec++], line, file);
			ok &= fabs(d - (age[j] + cohort[k]) ) < 1e-2;
			// check function
			str = str_char_vec(raw, sub_vec[i_sub_vec++]);
			ok &= str == function[q];
			if( ! ok )
			{	cerr << endl;
				cerr << "dismod_pde_csv: error at line " << line + 1;
				cerr << " in file " << file << endl;
				cerr << "One of the fixed fields had changed or " << endl;
				cerr << "age_grid, cohort_grid, or the measurements.";
				cerr << endl;
				std::exit(1);
			}
			// optimize_zeta
			d   = atof_char_vec(raw, sub_vec[i_sub_vec++], line, file);
			ok &= ! CppAD::isnan(d);
			sfun_in[i].optimize_zeta    = d;
			// optimize_lower
			d   = atof_char_vec(raw, sub_vec[i_sub_vec++], line, file);
			ok &= ! CppAD::isnan(d);
			sfun_in[i].optimize_lower    = d;
			// optimize_initial
			d   = atof_char_vec(raw, sub_vec[i_sub_vec++], line, file);
			ok &= ! CppAD::isnan(d);
			sfun_in[i].optimize_initial    = d;
			// optimize_upper
			d   = atof_char_vec(raw, sub_vec[i_sub_vec++], line, file);
			ok &= ! CppAD::isnan(d);
			sfun_in[i].optimize_upper    = d;

			// prior_like
			str = str_char_vec(raw, sub_vec[i_sub_vec++]);
			sfun_in[i].prior_like        = like_name2enum(str, line, file);
			// prior_zeta
			d   = atof_char_vec(raw, sub_vec[i_sub_vec++], line, file);
			ok &= ! CppAD::isnan(d);
			sfun_in[i].prior_zeta        = d;
			// prior_sigma
			d   = atof_char_vec(raw, sub_vec[i_sub_vec++], line, file);
			ok &= ! CppAD::isnan(d);
			sfun_in[i].prior_sigma       = d;
			// prior_mean
			d   = atof_char_vec(raw, sub_vec[i_sub_vec++], line, file);
			ok &= ! CppAD::isnan(d);
			sfun_in[i].prior_mean        = d;

			// age_like
			str = str_char_vec(raw, sub_vec[i_sub_vec++]);
			sfun_in[i].age_like          = like_name2enum(str, line, file);
			// age_zeta
			d   = atof_char_vec(raw, sub_vec[i_sub_vec++], line, file);
			ok &= ! CppAD::isnan(d);
			sfun_in[i].age_zeta          = d;
			// age_sigma
			d   = atof_char_vec(raw, sub_vec[i_sub_vec++], line, file);
			ok &= ! CppAD::isnan(d);
			sfun_in[i].age_sigma         = d;
			// age_order
			d   = atof_char_vec(raw, sub_vec[i_sub_vec++], line, file);
			ok &= ! CppAD::isnan(d);
			sfun_in[i].age_order         = size_t(d);

			// cohort_like
			str = str_char_vec(raw, sub_vec[i_sub_vec++]);
			sfun_in[i].cohort_like       = like_name2enum(str, line, file);
			// cohort_zeta
			d   = atof_char_vec(raw, sub_vec[i_sub_vec++], line, file);
			ok &= ! CppAD::isnan(d);
			sfun_in[i].cohort_zeta       = d;
			// cohort_sigma
			d   = atof_char_vec(raw, sub_vec[i_sub_vec++], line, file);
			ok &= ! CppAD::isnan(d);
			sfun_in[i].cohort_sigma      = d;
			// cohort_order
			d   = atof_char_vec(raw, sub_vec[i_sub_vec++], line, file);
			ok &= ! CppAD::isnan(d);
			sfun_in[i].cohort_order      = size_t(d);

			// cross_like
			str = str_char_vec(raw, sub_vec[i_sub_vec++]);
			sfun_in[i].cross_like        = like_name2enum(str, line, file);
			// cross_zeta
			d   = atof_char_vec(raw, sub_vec[i_sub_vec++], line, file);
			ok &= ! CppAD::isnan(d);
			sfun_in[i].cross_zeta        = d;
			// cross_sigma
			d   = atof_char_vec(raw, sub_vec[i_sub_vec++], line, file);
			ok &= ! CppAD::isnan(d);
			sfun_in[i].cross_sigma       = d;
			if( ! ok )
			{	cerr << endl;
				cerr << "dismod_pde_csv: error at line " << line + 1;
				cerr << " in file " << file << endl;
				cerr << "age = " << age[j];
				cerr << ", cohort = " <<  cohort[k];
				cerr << ", function = " << function[q] << endl;
				cerr << "One of the data fields has the value nan" << endl;
				cerr << endl;
				std::exit(1);
			}
		}
	}
	return;
}

void write_sfun_in(
	const std::string&            file   , 
	const dismod_pde::pair2index&    grid   )
{	using dismod_pde::vector;
	using std::cerr;
	using std::endl;
	using dismod_pde::output;

	// some temporaries
	size_t j, k, ell, q;

	//
	// some constants 
	//
	const size_t         n_grid        = grid.size();
	const vector<double> age           = grid.age();
	const vector<double> cohort        = grid.cohort();
	const vector<std::string> function = dismod_pde::stochastic_enum.name();
	const size_t n_stochastic          = function.size();
	//
	const char* sfun_in_field[] = {
		"age",
		"cohort",
		"time",
		"function",

		"optimize_zeta",
		"optimize_lower",
		"optimize_initial",
		"optimize_upper",

		"prior_like",
		"prior_zeta",
		"prior_sigma",
		"prior_mean",

		"age_like",
		"age_zeta",
		"age_sigma",
		"age_order",

		"cohort_like",
		"cohort_zeta",
		"cohort_sigma",
		"cohort_order",

		"cross_like",
		"cross_zeta",
		"cross_sigma"
	};
	const size_t n_field = sizeof(sfun_in_field) / sizeof(sfun_in_field[0]);

	// declare file stream
	std::ofstream fs;

	// First check that the file does not yet exist
	fs.open( file.c_str() , std::ios_base::in );
	if( ! fs.fail() )
	{	cerr << "dismod_pde_csv: Attempt to overwrite the existing file" << endl;
		cerr << file << endl;
		std::exit(1);
	}

	// Open the file for append. There should be nothing to append to
	// but this is just to be sure we do not loose work.
	fs.open( file.c_str(), std::ios_base::app );
	if( fs.fail() )
	{	cerr << "dismod_pde_csv: cant open following file for writing:" << endl;
		cerr << file << endl;
		std::exit(1);
	}

	// write field names on first line of file
	for(ell = 0; ell < n_field; ell++)
	{	fs << sfun_in_field[ell];
		if( ell != n_field - 1 )
			fs << ",";
	}
	fs << std::endl;

	// now loop over grid points
	size_t i_grid;
	for(i_grid = 0; i_grid < n_grid; i_grid++)
	{	grid.unpack(j, k, i_grid);
		double time          = age[j] + cohort[k];
		for(q = 0; q < n_stochastic; q++)
		{	// first four fields on this line have data
			fs << age[j]     << ",";
			fs << cohort[k]  << ",";
			fs << time       << ",";
			fs << function[q];
			// rest of fields must be filled in by user
			for(ell = 4; ell < n_field; ell++)
			{	size_t len = std::strlen(sfun_in_field[ell]);
				assert( len > 5 );
				const char* send = sfun_in_field[ell] + len - 5;
				bool likelihood  = std::strcmp(send, "_like") == 0;
				if( likelihood )
					fs << ",like";
				else	fs << ",nan";
			}
			fs << std::endl;
		}
	}
	fs.close();
}
void read_parameter(
	const std::string&                  file         , 
	dismod_pde::Optimize_Parameter_Struct& parameter    )
{
	using std::cerr;
	using std::endl;

	using dismod_pde::vector;
	using dismod_pde::atof_char_vec;
	using dismod_pde::str_char_vec;

	// section of message that is same for all errors
	using std::endl;
	std::ostringstream msg;
	msg << endl << "Error while reading parameter file " << file << endl;

	// some temporary indices
	size_t i, j;

	// measurement file field names
	const size_t n_field   = 2;
	vector<std::string> field(n_field);
	std::string str_name, str_value;
	field[0] = "name";
	field[1] = "value";

	// call csv_read
	vector<char>   raw;
	vector< std::pair<size_t,size_t> > sub_vec;
	dismod_pde::csv_read(file, field, raw, sub_vec);
	assert( sub_vec.size() % n_field == 0 );
	// number of lines in the data file counting header
	size_t n_line  = sub_vec.size() / n_field;

	// parameter names and default values
	size_t n_parameter = 7;
	vector<std::string> parameter_name(n_parameter);
	vector<bool>        parameter_scalar(n_parameter);
	double              scalar_value = 0.0; // set value to avoid warning
	vector<double>      vector_value;
	parameter_name[0] = "age_grid";           parameter_scalar[0] = false;
	parameter_name[1] = "cohort_grid";        parameter_scalar[1] = false;
	parameter_name[2] = "max_ode_step";       parameter_scalar[2] = true;
	parameter_name[3] = "print_level";        parameter_scalar[3] = true;
	parameter_name[4] = "max_iteration";      parameter_scalar[4] = true;
	parameter_name[5] = "converge_tolerance"; parameter_scalar[5] = true;
	parameter_name[6] = "debug_output";       parameter_scalar[6] = true;

	// which parameters have already been set
	vector<bool> parameter_set(n_parameter);
	for(j = 0; j < n_parameter; j++)
		parameter_set[j] = false;
 
	// get the parameter settings
	for(i = 1; i < n_line; i++)
	{	size_t index = i * n_field;
		str_name = str_char_vec(raw, sub_vec[index++] );
		bool valid_name = false;
		for(j = 0; j < n_parameter; j++)
		{	if( str_name == parameter_name[j] )
			{	if( parameter_set[j] )
				{	cerr << endl;
					cerr << "dismod_pde_csv: error at line " << i;
					cerr << " in file " << file << endl;
					cerr << "The parameter " << parameter_name[j];
					cerr << " appears twice" << endl;
					std::exit(1);
				}
				valid_name       = true;
				parameter_set[j] = true;
				if( parameter_scalar[j] ) scalar_value = 
					atof_char_vec(raw, sub_vec[index], i, file );
				else
				{	vector_value.resize(0);
					str_value        = str_char_vec(raw, sub_vec[index]);
					size_t n_char    = str_value.size();
					// skip spaces at begining
					size_t i_char    = 0;
					while(i_char < n_char)
					{	char ch = str_value[i_char++];
						while(ch == ' ' && i_char < n_char)
							ch = str_value[i_char++];
						//
						std::string element;
						while(ch != ' ' )
						{	element.push_back(ch);
							if( i_char < n_char )
								ch = str_value[i_char++];
							else	ch = ' ';
						}
						scalar_value = std::atof( element.c_str() );
						vector_value.push_back( scalar_value );
					}
				}
			}
		}
		if( ! valid_name )
		{	cerr << endl;
			cerr << "dismod_pde_csv: error at line " << i;
			cerr << " in file " << file << endl;
			cerr << "name = " << str_name << " not a valid choice" << endl;
			std::exit(1);
		}
		if( str_name == "age_grid" )
			parameter.age_grid  = vector_value;
		else if( str_name == "cohort_grid" )
			parameter.cohort_grid  = vector_value;
		else if( str_name == "max_ode_step" )
			parameter.max_ode_step  = scalar_value;
		else if( str_name == "print_level" )
			parameter.print_level  = static_cast<size_t>( scalar_value );
		else if( str_name == "max_iteration" )
			parameter.max_iteration  = static_cast<size_t>( scalar_value );
		else if( str_name == "converge_tolerance" )
			parameter.converge_tolerance  = scalar_value;
		else if( str_name == "debug_output" )
			parameter.debug_output  = static_cast<bool>( scalar_value );
		else
			assert( false );
	}

	return;
}

} // END empty namespace
 

int main(int argc, const char *argv[])
{
	using std::cerr;
	using std::endl;
	using dismod_pde::vector;

	// temporaries
	std::string file_in, file_out;

	const char* usage = "usage:"
	"dismod_pde_csv parameter.csv measure_in.csv sfun_in.csv\n"
	"\n"
	"dismod_pde_csv parameter.csv \\\n"
	"	        measure_in.csv   measure_out.csv\\\n"
	"	        sfun_in.csv      sfun_out.csv\\\n"
	"            effect_in.csv effect_out.csv\n";
	if( argc != 4 && argc != 8 )
	{	cerr << usage << endl;
		return 1;
	}
	// read parameter.csv
	file_in = argv[1];
	dismod_pde::Optimize_Parameter_Struct parameter;
	read_parameter(file_in, parameter);

	// read measure_in.csv
	file_in = argv[2];
	vector<std::string>                   covariate_name;
	vector<dismod_pde::Measure_Input_Struct> measure_in;
	vector<double>                        covariate_value;
	dismod_pde::read_measure_in(
		file_in, covariate_name, measure_in, covariate_value
	);
	const size_t n_measure   = measure_in.size();

	// determine starting and ending time for the data
	double time_lower   = measure_in[0].time_lower; 
	double time_upper   = measure_in[0].time_upper;
	for(size_t ell = 1; ell < n_measure; ell++)
	{	time_lower     = std::min(time_lower, measure_in[ell].time_lower);
		time_upper     = std::max(time_upper, measure_in[ell].time_upper);
	}

	// create grid
	dismod_pde::pair2index grid(
		parameter.age_grid, parameter.cohort_grid, time_lower, time_upper
	);
	const size_t n_grid       = grid.size();
	const size_t n_stochastic = dismod_pde::n_stochastic_enum;
	const size_t n_state      = n_grid * n_stochastic;

	if( argc == 4 )
	{	file_out = argv[3];
		// just create sfun_in.csv template 
		write_sfun_in(file_out, grid);
		return 0;
	}

	// read sfun_in.csv
	file_in = argv[4];
	vector<dismod_pde::Sfun_Input_Struct> sfun_in;
	set_sfun_in(file_in, grid, sfun_in);
	assert( n_state == sfun_in.size() );

	// read effect_in.csv
	file_in = argv[6];
	vector<dismod_pde::Effect_Input_Struct> effect_in;
	dismod_pde::read_effect_in(file_in, covariate_name, effect_in);
	const size_t n_effect    = effect_in.size();

	/*
	optimize the estimate 
	*/
	vector<dismod_pde::Sfun_Output_Struct>    sfun_out(n_state);
	vector<dismod_pde::Measure_Output_Struct> measure_out(n_measure);
	vector<double>                         effect_out(n_effect);
	std::string message;
	message = dismod_pde::optimize(
		parameter,     grid,         covariate_value,
		sfun_in,       sfun_out, 
		measure_in,    measure_out,
		effect_in,     effect_out
	);

	// create measure_out.csv
	file_in  = argv[2];
	file_out = argv[3];
	dismod_pde::write_measure_out(file_in, file_out, measure_in, measure_out);

	// create sfun_out.csv
	file_out = argv[5];
	write_sfun_out(file_out, grid, sfun_out); 

	// create effect_out.csv
	file_out   = argv[7];
	dismod_pde::write_effect_out(file_out, covariate_name, effect_in, effect_out);

	// check that message is as expected
	if( message != "optimize: success")
	{	cerr << endl;
		cerr << "dismod_pde_csv error: optimizer returned following message";
		cerr << endl << message << endl;
		std::exit(1);
	}
	
	return 0;
}
