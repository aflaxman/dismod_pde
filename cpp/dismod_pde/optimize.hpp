# ifndef DISMOD4_OPTIMIZE_HPP
# define DISMOD4_OPTIMIZE_HPP

/*
Notation:
Q:                           number of stochastic functions; i.e., 6
s^q:                         stochastic function with index q
S, C, iota, rho, xhi, omega: stochastic functions q=0,1,...,Q-1
H(r,sigma,h):                Huber loss function for h=1,2
a_j:                         age value for j=0,...,J-1
c_k:                         cohort value for k=0,...,K-1
zeta:                        a log offset value
sigma:                       a standard deviation
o:                           an order for o = 1,2
ell:                         measurement index for ell=0,...,L-1
i(ell):                      integrand index
a_{l,ell} (t_{l,ell})        lower age (time) limit
a_{u,ell} (t_{u,ell})        upper age (time) limit
m:                           covariate effect index for m=0,...,M-1
X_{ell,n}:                   covariate value for ell=0,...,L-1, n=0,...,N-1
n(m):                        covariate column as a function of effect index
i(m):                        integrand index as a function of effect index


$begin optimize$$
$latex \newcommand{\B}[1]{{\bf #1}}$$
$latex \newcommand{\R}[1]{{\rm #1}}$$
$latex \newcommand{\W}[1]{\; #1 \;}$$
$spell
	pde
	mt
	mp
	rr
	isnan
	rescale
	univariate
	Huber
	covariate
	covariates
	sfun
	bool
	indicies
	est
	res
	optimizer
	infeasibility
	Complementarity
	interpolant
	enum
	dismod
	eXcess
	param
	const
	struct
	integrands
	hpp
	std
	omega
$$

$section Optimization of dismod_pde Objective Function$$

$head Syntax$$
$codei%# include "optimize.hpp"
%$$
$icode%message% = dismod_pde::optimize(
	%parameter%,    %grid%,          %covariate_value%
	%sfun_in%,      %sfun_out%, 
	%measure_in%,   %measure_out%,
	%effect_in%     %effect_out%
)%$$

$head Stochastic Functions$$
dismod_pde estimates the following stochastic functions of age and cohort:
$table
$bold Name$$ $cnext 
	$bold Description$$ 
	$cnext $pre  $$ $code Stochastic_Enum$$
	$cnext $pre  $$ $latex q$$
$rnext
$latex S( \cdot )$$ $cnext
	fraction of cohort that are Susceptible to the condition
	$cnext $pre  $$ $code dismod_pde::S_enum$$
	$cnext $pre  $$ 0
$rnext
$latex C( \cdot )$$ $cnext
	fraction of cohort that have the Condition
	$cnext $pre  $$ $code dismod_pde::C_enum$$
	$cnext $pre  $$ 1
$rnext
$latex \iota ( \cdot ) $$ $cnext
	Incidence rate at which condition occurs
	$cnext $pre  $$ $code dismod_pde::iota_enum$$
	$cnext $pre  $$ 2
$rnext
$latex \rho  ( \cdot ) $$ $cnext
	Remission rate at which condition is cured
	$cnext $pre  $$ $code dismod_pde::rho_enum$$
	$cnext $pre  $$ 3
$rnext
$latex \chi  ( \cdot ) $$ $cnext
	eXcess mortality rate at which people die from the condition
	$cnext $pre  $$ $code dismod_pde::chi_enum$$
	$cnext $pre  $$ 4
$rnext
$latex \omega  ( \cdot ) $$ $cnext
	Mortality rate at which people die from other causes
	$cnext $pre  $$ $code dismod_pde::omega_enum$$
	$cnext $pre  $$ 5
$tend
For $latex q = 0 , \ldots , 5$$,
we also use $latex s^q (a, c)$$ for the stochastic functions above; e.g.,
$latex s^3 ( a , c ) = \rho ( a , c )$$.
Note that the $latex q$$ values corresponding to the different functions
may change in future versions of $code dismod_pde::optimize$$.
Hence it is better to use the $code Stochastic_Enum$$ values for the
stochastic function indices.

$head Stochastic_Enum$$
$index Stochastic_Enum$$
$code dismod_pde::Stochastic_Enum$$ is an $code enum$$
type with the values listed above.
Sometimes the corresponding integer value $latex q$$ is used to denote a 
stochastic function in the documentation below.
The vector
$codei%
	const dismod_pde::vector<std::string> dismod_pde::stochastic_enum.name()
%$$
can be used to map from the indices $latex q$$ to the corresponding string; 
e.g., 
$codei%
	dismod_pde::stochastic_enum.name()[ dismod_pde::rho_enum ] == "rho"
%$$
Here and below we use the notation $latex Q$$ for the number
of stochastic functions; i.e.,
$codei%
	%Q% = dismod_pde::n_stochastic_enum
%$$

$head Plus Infinity$$
We use the notation $latex + \infty$$ for the value
$codei%
	double %infinity% = DISMOD4_PLUS_INFINITY
%$$
where $code DISMOD4_PLUS_INFINITY$$ is defined by
$code optimize.hpp$$.


$head Not a Number$$
We use the notation $latex \R{nan}$$ for the value
$codei%
	double %nan% = dismod_pde::nan()
%$$
where $code dismod_pde::nan()$$ is defined by
$code optimize.hpp$$.
For a $code double $$ value $icode x$$,
one should use the function
$codei%
	dismod_pde::isnan(%x%)
%$$
to determine if $icode x$$ is not a number.


$head Likelihood Functions$$

$subhead Huber Loss Function$$
The Laplace $latex H( r, \sigma ,1)$$ and Gaussian $latex H( r, \sigma ,2)$$ 
Huber loss functions are defined as follows:
$latex \[
H( r , \sigma ,1) = \left\{ \begin{array}{ll}
	0 & \R{if} \; \sigma = \infty  \W{\R{or}} r = 0 \W{\R{or}} \R{isnan}(r)
	\\
	\infty & \R{otherwise \; if} \; \sigma = 0
	\\
	\sqrt{2} \; | \; r / \sigma |^1 & \R{otherwise}
\end{array} \right.
\] $$
$latex \[
H( r , \sigma ,2) = \left\{ \begin{array}{ll}
	0 & \R{if} \; \sigma = \infty  \W{\R{or}} r = 0 \W{\R{or}} \R{isnan}(r)
	\\
	\infty & \R{otherwise \; if} \; \sigma = 0 
	\\
	( r / \sigma )^2 / 2 & \R{otherwise}
\end{array} \right.
\] $$
Note that, up to terms that are constant for our application,
these loss functions are the negative-log likelihood for the univariate 


$subhead Likelihood_Enum$$
$index Likelihood_Enum$$
$code dismod_pde::Likelihood_Enum$$ is an $code enum$$ type with the values 
$code dismod_pde::gaussian_enum$$ and $code dismod_pde::laplace_enum$$.
You can use the vector
$codei%
	const dismod_pde::vector<std::string> dismod_pde::likelihood_enum.name()
%$$
to map from the $code enum$$ type to the corresponding string; 
e.g., 
$codei%
	dismod_pde::likelihood_enum.name()[ dismod_pde::gaussian_enum ] == "gaussian"
%$$

$subhead Residual Function$$
The dismod_pde residual scaling function for 
offset $latex \zeta$$,
model value $latex x$$,
and matching value $latex y$$ is defined by
$latex \[
R( y , x , \zeta ) = log( \zeta + y ) - \log( \zeta + x )
\] $$


$head Vector$$
The symbol $code dismod_pde::vector$$ is a 
$href%
	http://www.coin-or.org/CppAD/Doc/simplevector.xml%
	simple vector
%$$ template type.

$comment -------------------------------------------------------------------$$

$head parameter$$
The argument $icode parameter$$ has prototype
$codei%
	const dismod_pde::Optimize_Parameter_Struct& %parameter%
%$$
The following fields of $icode parameter$$ must be set
to the corresponding values:

$subhead max_ode_step$$
The parameter
$codei%
	double %parameter%.max_ode_step
%$$
determines the maximum age integration step to use when computing the functions
$latex \tilde{S}_{j,k} ( a )$$ and 
$latex \tilde{C}_{j,k} ( a )$$ as approximate solutions of an ODE; see
$cref/age_residual/optimize/sfun_out/age_residual/$$ below.
$icode%
	0 < parameter%.max_ode_step%
%$$

$subhead print_level$$
The parameter
$codei%
	size_t %parameter%.print_level
%$$
determines the level of output that is printed by the optimization 
procedure.
$table
level $pre  $$  $cnext Description $rnext
0 $cnext 
	no printing 
$rnext
4 $cnext
	print a summary at the beginning and end of the optimization procedure
$rnext
5 $cnext 
	print a $cref/trace/ipopt_trace/$$ for each iteration of 
	the optimization procedure.
$tend

$subhead max_iteration$$
The parameter
$codei%
	size_t %parameter%.max_iteration
%$$
determines the maximum number of iterations that the optimizer
should attempt before giving up on convergence as specified by 
$icode%parameter%.converge_tolerance%$$.

$subhead converge_tolerance$$
The parameter
$codei%
	double %parameter%.converge_tolerance
%$$
specifies the convergence criteria tolerance as a bound on the final value of
$code Dual infeasibility$$,
$code Constraint violation$$,
$code Complementarity$$,
as printed in the optimizer trace
(see $cref/print_level/optimize/parameter/print_level/$$).

$subhead debug_output$$
The parameter
$codei%
	bool %parameter%.debug_output
%$$
specifies if extra debugging output should be generated
(possibly at the expense of slower execution speed).

$comment -------------------------------------------------------------------$$

$head grid$$
The argument $icode grid$$ has prototype
$codei%
	const dismod_pde::pair2index& %grid%
%$$
It is a $cref/pair2index/$$ object constructed in the following way
$codei%
	dismod_pde::pair2index %grid%(%age%, %cohort%, %time_lower%, %time_upper%)
%$$
where $icode age$$, $icode cohort$$, $icode time_lower$$, 
and $icode time_upper$$ are described directly below:

$subhead G$$
$index G$$
For $latex j = 0 , \ldots , J-1$$ and $latex k = 0 , \ldots , K-1$$, if
$codei%
	%valid% = %grid%.pack(%j%, %k%, %grid_index%)
%$$
is true, then $latex (i,j)$$ is in the computation grid $latex G$$.
We use $latex s_G$$ to denote the stochastic function values
on the computational grid; i.e.
$latex \[
s_G = \left\{ 
	s_{j,k}^q \W{:}  (j, k) \in G \W{\R{and}} q = 0 , \ldots , Q-1 
\right\}
\] $$

$subhead age$$
The argument $cref/age/pair2index/age/$$ 
in the $icode grid$$ constructor specifies the grid of age values.
We use the notation $icode%J% = %age%.size()%$$
and for $latex j = 0 , \ldots , J-1$$,
we sometimes use $latex a_j$$ for the value $icode%age%[%j%]%$$.   
See $cref/age_lower, age_upper/optimize/measure_in/age_lower, age_upper/$$
for extra conditions on $icode age$$.

$subhead cohort$$
The argument $cref/cohort/pair2index/cohort/$$ 
to the $icode grid$$ constructor specifies the grid of cohort values.
We use the notation $icode%K% = %cohort%.size()%$$
and for $latex k = 0 , \ldots , K-1$$,
we sometimes use $latex c_k$$ for the value $icode%cohort%[%k%]%$$.
See $cref/time_lower, time_upper/optimize/measure_in/time_lower, time_upper/$$
for extra conditions on $icode cohort$$.

$subhead time_lower$$
The argument $cref/time_lower/pair2index/time_lower/$$ 
to the $icode grid$$ constructor specifies the lower limit in time 
for the age-cohort grid pairs that are to
be included in the computations and results.

$subhead time_upper$$
The argument $cref/time_upper/pair2index/time_upper/$$ 
to the $icode grid$$ constructor specifies the upper limit in time 
for the age-cohort grid pairs that are to
be included in the computations and results.

$comment -------------------------------------------------------------------$$

$head sfun_in$$
The argument $icode sfun_in$$ has prototype
$codei%
	const dismod_pde::vector<dismod_pde::Sfun_Input_Struct>& %sfun_in%
%$$
The size of this vector is
$codei%
	%sfun_in%.size() == %grid%.size() * %Q%
%$$
For $latex j = 0 , \ldots , J-1$$ and $latex k = 0 , \ldots , K-1$$, if
$codei%
	%valid% = %grid%.pack(%j%, %k%, %grid_index%)
%$$
returns false, the corresponding age-cohort pair 
is not in the computational grid.
Otherwise, for $latex q = 0 , \ldots , Q-1$$, the information stored in
$codei%
	double %sfun_in%[ %grid_index% * %Q% + %q%].%field_name%
%$$
corresponds to $latex s_{j,k}^q$$; i.e.,
the stochastic function specified by $latex q$$
at age $latex a_j$$ and cohort $latex c_k$$.
The values for the $icode field_name$$ are listed below:

$list number$$
$cref/optimize_zeta/optimize/sfun_in/optimize_zeta/$$,
	offset added before log in transformation of optimization problem.
$lnext
$cref/optimize_lower/optimize/sfun_in/optimize_lower/$$,
	specifies the lower bound for $latex s_{j,k}^q$$ during the optimization.
$lnext
$cref/optimize_initial/optimize/sfun_in/optimize_initial/$$,
	specifies the initial value for $latex s_{j,k}^q$$
	where the optimization starts.
$lnext
$cref/optimize_upper/optimize/sfun_in/optimize_upper/$$,
	specifies the upper bound for $latex s_{j,k}^q$$ during the optimization.
$lnext
$cref/prior_like/optimize/sfun_in/prior_like/$$,
	Huber loss function for direct prior on $latex s_{j,k}^q$$
$lnext
$cref/prior_zeta/optimize/sfun_in/prior_zeta/$$,
	offset added before log in direct prior residual for $latex s_{j,k}^q$$
$lnext
$cref/prior_sigma/optimize/sfun_in/prior_sigma/$$,
	standard deviation for direct prior on $latex s_{j,k}^q$$
$lnext
$cref/prior_mean/optimize/sfun_in/prior_mean/$$, 
	value corresponding to zero direct prior residual for $latex s_{j,k}^q$$
$lnext
$cref/age_like/optimize/sfun_in/age_like/$$,
	Huber loss function for age residual $latex A_{j,k}^{q,o} (s)$$
$lnext
$cref/age_zeta/optimize/sfun_in/age_zeta/$$,
	offset added to before log in definition of 
	$latex A_{j,k}^{q,o} (s)$$
$lnext
$cref/age_sigma/optimize/sfun_in/age_sigma/$$,
	standard deviation factor for age residual $latex A_{j,k}^{q,o} (s)$$
$lnext
$cref/age_order/optimize/sfun_in/age_order/$$,
	order for the age residual $latex A_{j,k}^{q,o} (s)$$,
	must be $code 1$$ for $latex S( \cdot )$$, $latex C( \cdot )$$.
$lnext
$cref/cohort_like/optimize/sfun_in/cohort_like/$$,
	Huber loss function for cohort residual $latex B_{j,k}^{q,o} (s)$$
$lnext
$cref/cohort_zeta/optimize/sfun_in/cohort_zeta/$$,
	offset added to before log in definition of 
	$latex B_{j,k}^{q,o} (s)$$
$lnext
$cref/cohort_sigma/optimize/sfun_in/cohort_sigma/$$,
	standard deviation factor for cohort residual $latex B_{j,k}^{q,o} (s)$$
$lnext
$cref/cohort_order/optimize/sfun_in/cohort_order/$$,
	order for the cohort residual $latex B_{j,k}^{o.q} (s)$$
$lnext
$cref/cross_like/optimize/sfun_in/cross_like/$$,
	Huber loss function for cross residual $latex D_{j,k}^{q,o} (s)$$
$lnext
$cref/cross_zeta/optimize/sfun_in/cross_zeta/$$,
	offset added to before log in definition of 
	$latex D_{j,k}^{q,o} (s)$$
$lnext
$cref/cross_sigma/optimize/sfun_in/cross_sigma/$$,
	standard deviation factor for cross residual $latex D_{j,k}^{q,o} (s)$$
$lend

$subhead optimize_zeta$$
We use $latex \zeta_{j,k}^{o,q}$$ to denote the offset added 
before the log in the transformation that re-scales the optimization 
problem.
To be specific, 
for each $latex (j, k) \in G$$ and $latex q = 1 , \ldots, Q-1$$,
$latex \[
	\bar{s}_{j,k}^p = \log( \zeta_{j,k}^{o,q} + s_{j,k}^q )
\] $$
The optimization is done by an generic optimizer with respect to
the transformed stochastic functions values $latex \bar{s}_G$$.


$subhead optimize_lower$$
The lower bound for $latex s_{j,k}^q$$ during the optimization
procedure, is specified by 
$codei%
	double %sfun_in%[ %grid_index% * %Q% + %q%].optimize_lower
%$$
We use the notation $latex s_{j,k}^{q,l}$$ for this lower bound
and $latex s_G^l$$ for the vectors of all the lower bounds
on the entire grid.

$subhead optimize_initial$$
The routine $code optimize$$ is a local optimizer and as such it requires 
a point to start the optimization procedure at. 
The $latex  s_{j,k}^q$$ component of this starting point
is specified by the value
$codei%
	double %sfun_in%[ %grid_index% * %Q% + %q%].optimize_initial
%$$
All of these values must be greater than or equal zero.

$subhead optimize_upper$$
The upper bound for $latex s_{j,k}^q$$ during the optimization
procedure, is specified by 
$codei%
	double %sfun_in%[ %grid_index% * %Q% + %q%].optimize_upper
%$$
where must be greater than or equal to corresponding lower bound.
We use the notation $latex s_{j,k}^{q,u}$$ for this upper bound
and $latex s_G^u$$ for the vectors of all the upper bounds
on the entire grid.

$subhead prior_like$$
We use $latex h_{j,k}^{q,p}$$ to denote the Huber loss function index for
the direct prior for the stochastic function value $latex  s_{j,k}^q$$.
This value is specified by
$codei%
	dismod_pde::Likelihood_Enum %sfun_in%[ %grid_index% * %Q% + %q%].prior_like
%$$
See $cref/prior_residual/optimize/sfun_out/prior_residual/$$.

$subhead prior_zeta$$
We use $latex \zeta_{j,k}^{q,p}$$ to denote the offset added before 
the log in the calculation of the direct prior residual for 
the stochastic function value $latex  s_{j,k}^q$$.
This value is specified by
$codei%
	double %sfun_in%[ %grid_index% * %Q% + %q%].prior_zeta
%$$
See $cref/prior_residual/optimize/sfun_out/prior_residual/$$.
It must hold that $latex \zeta_{j,k}^{q,p} > 0$$.

$subhead prior_sigma$$
We use $latex \sigma_{j,k}^{q,p}$$ to denote the standard deviation
of the direct prior for the stochastic function value $latex  s_{j,k}^q$$.
This value is specified by
$codei%
	double %sfun_in%[ %grid_index% * %Q% + %q%].prior_sigma
%$$
See $cref/prior_residual/optimize/sfun_out/prior_residual/$$.

$subhead prior_mean$$
We use $latex \mu_{j,k}^q$$ to denote
the stochastic function value $latex s_{j,k}^q$$
at which the direct prior residual is zero.
This value is specified by
$codei%
	double %sfun_in%[ %grid_index% * %Q% + %q%].prior_mean
%$$
See $cref/prior_residual/optimize/sfun_out/prior_residual/$$.

$subhead age_like$$
We use $latex h_{j,k}^{q,a}$$ to denote the Huber loss function index for
the age residual $latex A_{j,k}^{q,o} (s)$$.
This value is specified by
$codei%
     double %sfun_in%[ %grid_index% * %Q% + %q%].age_like
%$$
See $cref/age_residual/optimize/sfun_out/age_residual/$$.

$subhead age_zeta$$
We use $latex \zeta_{j,k}^{q,a}$$ to denote the offset that is added before
takeing the logs in the definition of$latex A_{j,k}^{q,o} (s)$$.
This value is specified by
$codei%
     double %sfun_in%[ %grid_index% * %Q% + %q%].age_zeta
%$$
See $cref/age_residual/optimize/sfun_out/age_residual/$$.

$subhead age_sigma$$
We use $latex \sigma_{j,k}^{q,a}$$ to denote the standard deviation factor
for the age residual $latex A_{j,k}^{q,o} (s)$$.
This value is specified by
$codei%
     double %sfun_in%[ %grid_index% * %Q% + %q%].age_sigma
%$$
See $cref/age_residual/optimize/sfun_out/age_residual/$$.

$subhead age_order$$
We use $latex o_{j,k}^{q,a}$$ to denote the order
for the age residual $latex A_{j,k}^{q,o} (s)$$.
This value is specified by
$codei%
	size_t %sfun_in%[ %grid_index% * %Q% + %q%].age_order
%$$
It must have value one or two, and 
must be one when $icode q$$ corresponds to 
$latex S( \cdot )$$ ($latex q = 0$$) or 
$latex C( \cdot )$$ ($latex q = 1$$) .
See $cref/age_residual/optimize/sfun_out/age_residual/$$.

$subhead cohort_like$$
We use $latex h_{j,k}^{c, q}$$ to denote the Huber function index for
the cohort residual $latex B_{j,k}^{q,o} (s)$$
This value is specified by
$codei%
     double %sfun_in%[ %grid_index% * %Q% + %q%].cohort_like
%$$
See $cref/cohort_residual/optimize/sfun_out/cohort_residual/$$.

$subhead cohort_zeta$$
We use $latex \zeta_{j,k}^{c, q}$$ to denote the offset that is added before
taking the logs in the definition of $latex B_{j,k}^{q,o} (s)$$
This value is specified by
$codei%
     double %sfun_in%[ %grid_index% * %Q% + %q%].cohort_zeta
%$$
See $cref/cohort_residual/optimize/sfun_out/cohort_residual/$$.

$subhead cohort_sigma$$
We use $latex \sigma_{j,k}^{c, q}$$ to denote the standard deviation factor for
the cohort residual $latex B_{j,k}^{q,o} (s)$$
This value is specified by
$codei%
     double %sfun_in%[ %grid_index% * %Q% + %q%].cohort_sigma
%$$
See $cref/cohort_residual/optimize/sfun_out/cohort_residual/$$.

$subhead cohort_order$$
We use $latex o_{j,k}^{c, q}$$ to denote the order
for the cohort residual $latex B_{j,k}^{q,o} (s)$$.
This value is specified by
$codei%
	size_t %sfun_in%[ %grid_index% * %Q% + %q%].cohort_order
%$$
and must have value one or two.
See $cref/cohort_residual/optimize/sfun_out/cohort_residual/$$.

$subhead cross_like$$
We use $latex h_{j,k}^{d, q}$$ to denote the Huber function index for
the cross residual $latex D_{j,k}^q (s)$$
This value is specified by
$codei%
     double %sfun_in%[ %grid_index% * %Q% + %q%].cross_like
%$$
See $cref/cross_residual/optimize/sfun_out/cross_residual/$$.

$subhead cross_zeta$$
We use $latex \zeta_{j,k}^{d, q}$$ to denote the offset that is added before
taking the logs in the definition of $latex D_{j,k}^{q,o} (s)$$
This value is specified by
$codei%
     double %sfun_in%[ %grid_index% * %Q% + %q%].cross_zeta
%$$
See $cref/cross_residual/optimize/sfun_out/cross_residual/$$.

$subhead cross_sigma$$
We use $latex \sigma_{j,k}^{d, q}$$ to denote the standard deviation factor for
the cross residual $latex D_{j,k}^{q,o} (s)$$
This value is specified by
$codei%
     double %sfun_in%[ %grid_index% * %Q% + %q%].cross_sigma
%$$
See $cref/cross_residual/optimize/sfun_out/cross_residual/$$.

$comment -------------------------------------------------------------------$$

$head covariate_value$$
$index value, covariate X$$
$index covariate, value X$$
$index X, covariate value$$
The argument $icode covariate_value$$ has prototype
$codei%
	const dismod_pde::vector<double>& %covariate_value%
%$$
its size is equal to $icode%L% * %N%$$ where
$icode L$$ is the number of measurements and $icode N$$ is the
number of covariate columns.
We define covariate matrix $latex X \in \B{R}^{L \times N}$$ by
$pre
	$$
$latex X_{\ell,n} = $$ $icode%covariate_value%[ %ell% * %N% + %n% ]%$$
for $icode%n% = 0 , %...% , %N%-1%$$ and
$icode%ell% = 0 , %...% , %L%-1%$$.

$comment -------------------------------------------------------------------$$

$head measure_in$$
The argument $icode measure_in$$ has prototype
$codei%
	const dismod_pde::vector<dismod_pde::Measure_Input_Struct>& %measure_in%
%$$ 
The number of measurements $latex L$$ is specified by
$codei%
	%L% = %measure_in%.size()
%$$
For $icode%ell% = 0 , %...% , %L%-1%$$,
the following fields of $icode%measure_in%[%ell%]%$$ must be set:

$table
$bold field name$$ $cnext $bold Description$$ 
$rnext
$cref/integrand/optimize/measure_in/integrand/$$ $cnext
	The integrand used to model the measurement.
$rnext
$cref/age_lower, age_upper/optimize/measure_in/age_lower, age_upper/$$ $cnext
	The age limits for this measurement.
$rnext
$cref/time_lower, time_upper/optimize/measure_in/time_lower, time_upper/$$ 
	$cnext The time limits for this measurement.
$rnext
$cref/meas_like/optimize/measure_in/meas_like/$$ $cnext
	Huber loss function used to model this measurement.
$rnext
$cref/meas_zeta/optimize/measure_in/meas_zeta/$$ $cnext
	Offset that is added before taking the log in the likelihood.
$rnext
$cref/meas_sigma/optimize/measure_in/meas_sigma/$$ $cnext
	Our model for the standard deviation of this measurement.
$rnext
$cref/meas_value/optimize/measure_in/meas_value/$$ $cnext
	The value for this measurement. 
$tend

$subhead integrand$$
We use $latex I^{i( \ell )} ( \cdot )$$ to denote the integrand 
for the $latex \ell$$-th measurement.
The value $latex i( \ell )$$ is specified by
$codei%
	dismod_pde::Integrand_Enum %measure_in%[%ell%]%.integrand
%$$
Each measurement is a definite integral of one of the following functions 
with respect to age and time (plus some random noise):
$table
$bold Definition$$ 
	$cnext $pre  $$ $bold integrand$$
	$cnext $pre  $$ $bold Description$$
$rnext
$latex I^\iota ( \cdot ) = \iota( \cdot )$$
	$cnext $pre  $$ $code incidence_enum$$
	$cnext $pre  $$ incidence rate

$rnext
$latex I^\rho ( \cdot ) = \rho( \cdot )$$
	$cnext $pre  $$ $code remission_enum$$
	$cnext $pre  $$ remission rate

$rnext
$latex I^\chi ( \cdot ) = \chi( \cdot )$$
	$cnext $pre  $$ $code m_excess_enum$$
	$cnext $pre  $$ excess mortality rate

$rnext
$latex I^\omega ( \cdot ) = \omega( \cdot ) $$
	$cnext $pre  $$ $code m_other_enum$$
	$cnext $pre  $$ other cause mortality rate

$rnext
$latex I^w ( \cdot ) = \omega( \cdot ) + \chi ( \cdot ) $$
	$cnext $pre  $$ $code m_with_enum$$
	$cnext $pre  $$ with condition mortality rate

$rnext
$latex I^p ( \cdot ) = C( \cdot ) / [ S( \cdot ) + C( \cdot ) ]$$
	$cnext $pre  $$ $code prevalence_enum$$
	$cnext $pre  $$ prevalence

$rnext
$latex I^e ( \cdot ) = \chi( \cdot ) I^p ( \cdot ) $$
	$cnext $pre  $$ $code m_prevalence_enum$$
	$cnext $pre  $$ excess mortality rate times prevalence
	
$rnext
$latex I^s ( \cdot ) = I^e ( \cdot )  $$
	$cnext $pre  $$ $code m_specific_enum$$
	$cnext $pre  $$ cause specific mortality rate

$rnext
$latex I^a ( \cdot ) = 
	\omega ( \cdot ) +  I^e ( \cdot )$$
	$cnext $pre  $$ $code m_all_enum$$
	$cnext $pre  $$ all cause mortality rate

$rnext
$latex I^\sigma ( \cdot ) = I^w ( \cdot ) / I^a ( \cdot )  $$
	$cnext $pre  $$ $code m_standard_enum$$
	$cnext $pre  $$ standardized mortality ratio

$rnext
$latex I^r ( \cdot ) = I^w ( \cdot ) / \omega ( \cdot )$$
	$cnext $pre  $$ $code relative_risk_enum$$
	$cnext $pre  $$ relative risk ratio
$tend

$subhead Integrand_Enum$$
$code dismod_pde::Integrand_Enum$$ is an $code enum$$
type with the values listed above.
Sometimes the corresponding integer value $latex i$$ is used to denote a 
integrand function in the documentation below.
The vector
$codei%
	const vector<std::string> dismod_pde::integrand_enum.name()
%$$
can be used to map from the indices $latex i$$ to the corresponding string; 
e.g., 
$codei%
dismod_pde::integrand_enum.name()[ dismod_pde::relative_risk_enum ] == "relative_risk"
%$$
The number of possible integrands is equal to
$codei%
	dismod_pde::n_integrand_enum
%$$

$subhead age_lower, age_upper$$
We use $latex a_{l, \ell}$$ and $latex a_{u, \ell}$$ to denote the 
lower and upper age integration limits for the $latex \ell$$-th measurement.
These values are specified by 
$codei%
	double %measure_in%[%ell%].age_lower
	double %measure_in%[%ell%].age_upper
%$$
For all $latex \ell$$, it must hold that 
$latex \[
	a_1 \leq a_{l, \ell} \leq a_{u, \ell} \leq a_{J-1}
\] $$

$subhead time_lower, time_upper$$
We use $latex t_{l, \ell}$$ and $latex t_{u, \ell}$$ to denote the 
lower and upper time integration limits for  the $latex \ell$$-th measurement.
These values are specified by
$codei%
	size_t %measure_in%[%ell%].time_lower
	size_t %measure_in%[%ell%].time_upper
%$$
For all $latex \ell$$, it must hold that 
$latex \[
	c_1 + a_{J-1} < t_{l, \ell} \leq t_{u, \ell} < c_{K-2} + a_1
\] $$

$subhead meas_like$$
We use $latex h_\ell $$ to denote the Huber function index 
for the $latex \ell$$-th measurement which is specified by
$codei%
	size_t %measure_in%[%ell%].meas_like
%$$
See $cref/measure_out residual/optimize/measure_out/residual/$$.

$subhead meas_zeta$$
We use $latex \zeta_\ell $$ to denote the model offset 
added before the log in the residual
for the $latex \ell$$-th measurement which is specified by
$codei%
	double %measure_in%[%ell%].meas_zeta
%$$
See $cref/measure_out residual/optimize/measure_out/residual/$$.

$subhead meas_sigma$$
We use $latex \sigma_\ell $$ to denote the standard deviation 
for the $latex \ell$$-th measurement which is specified by
$codei%
	double %measure_in%[%ell%].meas_sigma
%$$
See $cref/measure_out residual/optimize/measure_out/residual/$$.

$subhead meas_value$$
We use $latex z_\ell $$ to denote the value
for the $latex \ell$$-th measurement which is specified by
$codei%
	double %measure_in%[%ell%].meas_value
%$$
See $cref/measure_out residual/optimize/measure_out/residual/$$.


$comment -------------------------------------------------------------------$$

$head effect_in$$
The argument $icode effect_in$$ has prototype
$codei%
	const dismod_pde::vector<dismod_pde::Effect_Input_Struct>&  %effect_in%
%$$
and its size must be equal to $icode M$$, the number of 
measurement effects.
For $icode%m% = 0 , %...% , %M%-1%$$,
the following fields of $icode%effect_in%[%m%]%$$ must be set:

$subhead integrand$$
The measurement integrand for this effect is
$codei%
	Integrand_Enum %effect_in%[%m%].integrand
%$$
which is denoted by $latex i(m)$$ below.

$subhead covariate$$
The covariate column for this measurement effect is
$codei%
	size_t %effect_in%[%m%].covariate
%$$
which is denoted by $latex n(m)$$ below.

$subhead lower$$
The lower limit for this measurement effect during optimization is
$codei%
	double %effect_in%[%m%].lower
%$$

$subhead initial$$
The initial value for this measurement effect during optimization is
$codei%
	double %effect_in%[%m%].initial
%$$

$subhead upper$$
The upper limit for this measurement effect during optimization is
$codei%
	double %effect_in%[%m%].upper
%$$

$head effect_out$$
The argument $icode effect_out$$ has prototype
$codei%
	dismod_pde::vector<double>&  %effect_out%
%$$
and its size must be given by 
$icode%effect_out%.size() = %M%$$.  
The input value of its elements does not matter.
Upon return from $code optimize$$,
for $latex m = 0 , \ldots , M-1$$, 
$icode%effect_out%[%i%]%$$ is the optimal estimate for 
the $th m$$ measurement effect $latex \beta_m$$


$comment -------------------------------------------------------------------$$

$head sfun_out$$
The argument $icode sfun_out$$ has prototype
$codei%
     dismod_pde::vector<dismod_pde::Sfun_Output_Struct>& %sfun_out%
%$$
The size of this vector is
$codei%
	%sfun_out%.size() == %grid%.size() * %Q%
%$$
The input value of the elements of $icode sfun_out$$ does not matter.
For $latex j = 0, ..., J-1$$ and $latex k = 0 , ... , K-1$$, if
$codei%
	%valid% = %grid%.pack(%j%, %k%, %grid_index%)
%$$
returns false, the corresponding age-cohort pair 
is not in the computational grid.
Otherwise, Upon return from $code optimize$$,
for $latex q = 0 , \ldots , Q-1$$,
the information stored in
$codei% 
	%sfun_out%[ %grid_index% * %Q% + %q%].%field_name%
%$$
corresponds to $latex s_{j,k}^q$$; i.e., the stochastic function
specified by $latex q$$ at age $latex a_j$$ and cohort $latex c_k$$.
The values for $icode field_name$$ are listed directly below:

$list number$$
$cref/estimate/optimize/sfun_out/estimate/$$,
	final estimate returned by the optimization process.
$lnext
$cref/prior_residual/optimize/sfun_out/prior_residual/$$,
	prior residuals corresponding to final estimate.
$lnext
$cref/prior_scaled/optimize/sfun_out/prior_scaled/$$,
	scaled prior residuals corresponding to final estimate.
$lnext
$cref/age_residual/optimize/sfun_out/age_residual/$$,
	age residuals corresponding to final estimate.
$lnext
$cref/age_scaled/optimize/sfun_out/age_scaled/$$,
	scaled age residuals corresponding to final estimate.
$lnext
$cref/cohort_residual/optimize/sfun_out/cohort_residual/$$,
	cohort residuals corresponding to final estimate.
$lnext
$cref/cohort_scaled/optimize/sfun_out/cohort_scaled/$$,
	scaled cohort residuals corresponding to final estimate.
$lnext
$cref/cross_residual/optimize/sfun_out/cross_residual/$$,
	cross residuals corresponding to final estimate.
$lnext
$cref/cross_scaled/optimize/sfun_out/cross_scaled/$$,
	scaled cross residuals corresponding to final estimate.
$lend

$subhead estimate$$
The $latex  s_{j,k}^q$$ component is of the final estimate
returned by the optimizer is
$codei%
	double %sfun_in%[ %grid_index% * %Q% + %q%].estimate
%$$

$subhead prior_residual$$
The return value of
$codei% 
	double %sfun_out%[ %grid_index% * %Q% + %q%].prior_residual
%$$
is set to the direct prior residual.
Note that this is defined for every $latex (j, k) \in G$$.
To be specific,
$latex \[
P_{j,k}^q ( s_G ) = R \left( 
	 s_{j,k}^q 
	\W{,} \mu_{j,k}^q \W{,}
	\zeta_{j,k}^{q,p} 
\right)
\] $$
where $latex s_G$$ is the return
$cref/estimate/optimize/sfun_out/estimate/$$.

$subhead prior_scaled$$
The return value of
$codei% 
	double %sfun_out%[ %grid_index% * %Q% + %q%].prior_scaled
%$$
is set to the scaled residual for the direct prior which is defined by
$latex \[
\left\{ \begin{array}{ll} 
	\left. P_{j,k}^q ( s_G ) \right/  \sigma_{j,k}^{q,p}
	& \R{if} \; 0 < \sigma_{j,k}^{q,p} < + \infty
	\\
	\R{nan} & \R{Otherwise}
\end{array} \right.
\] $$

$subhead age_residual$$
The return value of
$codei% 
	double %sfun_out%[ %grid_index% * %Q% + %q%].age_residual
%$$
is set to the age residual $latex A_{j,k}^{q,o} (s)$$ (defined below).
For $latex q = 0 , \ldots , Q-1$$,
$latex \[
A_{j,k}^{q,1} ( s_G ) = \R{nan} 
	\W{\Leftarrow}  (j+1,k) \; \R{not \; in} \; G 
\] $$
$latex \[
A_{j,k}^{q,2} ( s_G ) = \R{nan} 
	\W{\Leftarrow} (j+2,k) \; \R{not \; in} \; G 
\] $$
For $latex (j, k) \in G$$ such that $latex (j+1,k) \in G$$,
$latex q = 2 , \ldots , Q-1$$, and 
$latex a_j \leq \alpha \leq a_{j+1}$$, 
we define the function
$latex \[
\tilde{s}^q_{j,k} ( \alpha ) = 
\frac{ 
	s^q_{j,k} ( a_{j+1} - \alpha ) + s^q_{j+1,k} ( \alpha - a_j ) 
}{
	a_{j+1} - a_j 
} 
\] $$ 
We define the functions
$latex \tilde{S}_{j,k} ( a )$$ ($latex q = 0$$) and 
$latex \tilde{C}_{j,k} ( a )$$ ($latex q = 1$$) 
as approximate solutions for the following ODE:
$latex \[
\begin{array}{rcl}
\tilde{S}_{j,k} ( a_j ) & = & S_{j,k} 
\\
\tilde{C}_{j,k} ( a_j ) & = & C_{j,k} 
\\
\partial_\alpha \tilde{S}_{j,k} ( \alpha  ) 
& = &
- [ \tilde{\iota}_{j,k} ( \alpha ) + \tilde{\omega}_{j,k} ( \alpha ) ] 
	\; \tilde{S}_{j,k} ( \alpha ) 
+ \tilde{\rho}_{j,k} ( \alpha ) \; \tilde{C}_{j,k} ( \alpha )
\\
\partial_ \alpha  \tilde{C}_{j,k} (  \alpha  ) 
& = &
\tilde{\iota}_{j,k} ( \alpha ) \; \tilde{S}_{j,k} ( \alpha ) 
- [	\tilde{\rho}_{j,k} ( \alpha ) + 
	\tilde{\omega}_{j,k} ( \alpha ) + 
	\tilde{\chi}_{j,k} ( \alpha ) ] \; \tilde{C}_{j,k} ( \alpha )
\end{array}
\] $$
The parameter
$cref/max_ode_step/optimize/parameter/max_ode_step/$$ 
determines how accurately we approximate the solution of this ODE;
i.e., $latex \tilde{s}_{j,k}^q ( \alpha )$$ for $latex q = 0, 1$$.
We define the first order age residuals 
for $latex S$$ ($latex q = 0$$)
and for $latex C$$ ($latex q = 1$$)
$latex \[
A_{j,k}^{q,1} ( s_G ) 
=
R \left( 
	\tilde{s}_{j,k}^q (  a_{j+1} ) 
	\W{,} s_{j+1,k} \W{,}
	\zeta_{j,k}^{q,a} 
\right)
%
\W{\Leftarrow} (j+1,k) \in G 
\] $$
For $latex q = 2 , \ldots , Q-1$$, we define the 
first order age residual functions
$latex \[
A_{j,k}^{q,1} ( s_G ) 
=
R \left( 
	s_{j+1,k} 
	\W{,} s_{j,k}^q \W{,}
	\zeta_{j,k}^{q,a} 
\right)
%
\W{\Leftarrow} (j+1,k) \in G 
\] $$
For $latex q = 2, \ldots, Q-1$$,
we define the second order age residual functions
$latex \[
A_{j,k}^{q,2} ( s_G ) 
=
\left[ 
\frac{
	R \left( s_{j+2,k} \W{,} s_{j+1,k}^q \W{,} \zeta_{j,k}^{q,a} \right)
}{ 
	a_{j+2} - a_{j+1} 
}
-
\frac{
	R \left( s_{j+1,k} \W{,} s_{j,k}^q \W{,} \zeta_{j,k}^{q,a} \right)
}{ 
	a_{j+1} - a_{j} 
}
\right]
%
\W{\Leftarrow} (j+2,k) \in G 
\] $$

$subhead age_scaled$$
The return the value of 
$codei% 
	double %sfun_out%[ %grid_index% * %Q% + %q%].age_scaled
%$$
is set to the scaled age residual which is defined by
$latex \[
\left\{ \begin{array}{ll}
A_{j,k}^{q,1} ( s_G ) / \sigma_{j,k}^{q,a} 
	& \R{if} \; o_{j,k}^{q,a} = 1 \W{\R{and}} 0 < \sigma_{j,k}^{q,a} < + \infty
\\
A_{j,k}^{q,2} ( s_G ) / \sigma_{j,k}^{q,a} 
	& \R{if} \; o_{j,k}^{q,a} = 2 \W{\R{and}} 0 < \sigma_{j,k}^{q,a} < + \infty
\\
\R{nan}
	& \R{Otherwise}
\end{array} \right.
\] $$

$subhead cohort_residual$$
The return value of
$codei% 
	double %sfun_out%[ %grid_index% * %Q% + %q%].cohort_residual
%$$
is set to the age residual $latex B_{j,k}^{q,o} (s)$$ (defined below).
For $latex q = 0 , \ldots , Q-1$$,
$latex \[
B_{j,k}^{q,1} ( s_G ) = \R{nan} 
	\W{\Leftarrow}  (j,k+1) \; \R{not \; in} \; G 
\] $$
$latex \[
B_{j,k}^{q,2} ( s_G ) = \R{nan} 
	\W{\Leftarrow} (j,k+2) \; \R{not \; in} \; G 
\] $$
We define the first order cohort residual functions
$latex \[
B_{j,k}^{q,1} ( s_G ) 
=
R \left( 
	s_{j,k+1} 
	\W{,} s_{j,k}^q \W{,}
	\zeta_{j,k}^{q,a} 
\right)
%
\W{\Leftarrow} (j, k+1) \in G 
\] $$
We define the second order age residual functions
$latex \[
B_{j,k}^{q,2} ( s_G ) 
=
\left[ 
\frac{
	R \left( s_{j,k+2} \W{,} s_{j,k+1}^q \W{,} \zeta_{j,k}^{q,a} \right)
}{ 
	c_{k+2} - c_{k+1} 
}
-
\frac{
	R \left( s_{j,k+1} \W{,} s_{j,k}^q \W{,} \zeta_{j,k}^{q,a} \right)
}{ 
	c_{k+1} - c_{k} 
}
\right]
%
\W{\Leftarrow} (j, k+2) \in G 
\] $$

$subhead cohort_scaled$$
The return the value of 
$codei% 
	double %sfun_out%[ %grid_index% * %Q% + %q%].cohort_scaled
%$$
is set to the scaled age residual which is defined by
$latex \[
\left\{ \begin{array}{ll}
B_{j,k}^{q,1} ( s_G ) / \sigma_{j,k}^{q,c} 
	& \R{if} \; o_{j,k}^{q,c} = 1 \W{\R{and}} 0 < \sigma_{j,k}^{q,c} < + \infty
\\
B_{j,k}^{q,2} ( s_G ) / \sigma_{j,k}^{q,c} 
	& \R{if} \; o_{j,k}^{q,c} = 2 \W{\R{and}} 0 < \sigma_{j,k}^{q,c} < + \infty
\\
\R{nan}
	& \R{Otherwise}
\end{array} \right.
\] $$

$subhead cross_residual$$
The return value of
$codei% 
	double %sfun_out%[ %grid_index% * %Q% + %q%].cross_residual
%$$
is set to the age residual $latex D_{j,k}^q (s)$$ (defined below).
For $latex q = 0 , \ldots , Q-1$$,
$latex \[
D_{j,k}^q ( s_G ) = \R{nan} \W{\Leftarrow}  
	(j,k+1) \; \R{or} \; (j+1,k) \; \R{or} \; (j+1,k+1) \; \R{not \; in} \; G 
\] $$
otherwise
$latex \[
D_{j,k}^q ( s_G ) 
=
\frac{ 
R \left( s^q_{j+1,k+1} \W{,} s^q_{j+1,k} \W{,} \zeta_{j,k}^{q,d} \right)
-
R \left( s^q_{j,k+1} \W{,} s^q_{j,k} \W{,} \zeta_{j,k}^{q,d} \right)
}{ 
	\sqrt{( c_{k+1} - c_k ) ( a_{j+1} - a_j ) }
}
\] $$

$subhead cross_scaled$$
The return the value of 
$codei% 
	double %sfun_out%[ %grid_index% * %Q% + %q%].cross_scaled
%$$
is set to the scaled age residual which is defined by
$latex \[
\left\{ \begin{array}{ll}
D_{j,k}^q ( s_G ) / \sigma_{j,k}^{q,d} 
	& \R{if} \; 0 < \sigma_{j,k}^{q,c} < + \infty
\\
\R{nan}
	& \R{Otherwise}
\end{array} \right.
\] $$

$comment -------------------------------------------------------------------$$

$head measure_out$$
The argument $icode measure_out$$ has prototype
$codei%
	dismod_pde::vector<dismod_pde::Measure_Output_Struct>& %measure_out%
%$$
and it has size $latex L$$.
The input value of its elements does not matter.
Upon return,
For $latex \ell = 0 , \ldots , L-1$$, the value of 
$codei% 
	%measure_out%[%ell%].%field_name%
%$$
has been set as follows:
$list number$$
$cref/model/optimize/measure_out/model/$$,
is the value that 
$cref/estimate/optimize/sfun_out/estimate/$$ and
$cref/effect_out/optimize/effect_out/$$
predict for the integrand; i.e. $latex \bar{I}_\ell (s)$$.
$lnext
$cref/adjusted/optimize/measure_out/adjusted/$$,
is the value that 
$cref/effect_out/optimize/effect_out/$$
yields for the covariate adjusted measurements $latex Z_\ell ( \beta )$$.
$lnext
$cref/residual/optimize/measure_out/residual/$$,
is the log transformed measurement minus the log transformed model
$lnext
$cref/scaled/optimize/measure_out/scaled/$$,
is a scaled version of the residual. 
$lend

$subhead adjusted$$
Let $latex \beta$$ be the vector of covariate effects
corresponding to the 
$cref/effect_out/optimize/effect_out/$$.
We define the covariate adjusted measurement function by
$latex \[
\begin{array}{rcl}
\delta ( \ell , m ) & = & \left\{ \begin{array}{ll}
	1 & \R{if} \; i(\ell) = i(m) 
	\\
	0 & \R{otherwise}
\end{array} \right.
\\
Z_\ell ( \beta )  & = & z_\ell \; \exp \left( 
	- \sum_{m=0}^{M-1} \delta ( \ell, m ) X_{\ell, n(m)} \beta_m 
\right)
\end{array}
\] $$
See 
$cref/covariate_value/optimize/covariate_value/$$ for the definition
of $latex X$$,
$cref/effect_in/optimize/effect_in/integrand/$$ for the definition
of the integrand index $latex i(m)$$, and
$cref/measure_in/optimize/measure_in/integrand/$$ for the definition
of the integrand index $latex i(\ell)$$.
The return value of
$codei%
	double %measure_out%[%ell%].adjusted
%$$
is equal to $latex Z_\ell ( \beta )$$.

$subhead model$$
Let $latex s_G$$ be the stochastic functions,
on the computational grid $latex G$$,
corresponding to the final
$cref/estimate/optimize/sfun_out/estimate/$$.
The return value of
$codei%
	double %measure_out%[%ell%].model
%$$
is given by $latex \bar{I}_\ell ( s_G )$$ defined below:
$pre

$$
The midpoints for the age and time limits are denoted by
$latex \[
\begin{array}{rcl}
\bar{a}_\ell & = & ( a_{l,\ell} + a_{u,\ell} ) / 2
\\
\bar{t}_\ell & = & ( t_{l,\ell} + t_{u,\ell} ) / 2
\end{array}
\] $$
The indices $latex \jmath ( \ell )$$,
$latex \underline{\kappa}( \ell )$$, and
$latex \overline{\kappa}( \ell )$$ are defined by
$latex \[
\begin{array}{rcl}
a_{\jmath (\ell)} \leq       
	& \bar{a}_\ell & 
		<  a_{ \jmath(\ell)+1}
\\
c_{\underline{\kappa}(\ell)} \leq
	& \bar{t}_\ell - a_{\jmath (\ell)} & 
		< c_{\underline{\kappa}(\ell) + 1} 
\\
c_{\overline{\kappa}(\ell)} \leq
	& \bar{t}_\ell - a_{\jmath (\ell)+1} & 
		< c_{\overline{\kappa}(\ell) + 1} 
\end{array}
\] $$
The notation $latex i( \ell )$$ is the integrand index for 
the $latex \ell$$-th measurement;
see $cref/integrand/optimize/measure_in/integrand/$$.
We define $latex \bar{I}_\ell (s) $$ as the bilinear interpolant 
of $latex I^{i(\ell)} ( \cdot )$$,
with respect to age and time, 
evaluated at age $latex \bar{a}_\ell$$ and time $latex \bar{t}_\ell$$.
$latex \[
\begin{array}{rcl}
\bar{I}_\ell ( s_G )
& = &
I_{\jmath ( \ell ) , \underline{\kappa} ( \ell )} ^{i( \ell ) }
( s_G )
\frac{ 
	a_{ \jmath ( \ell ) + 1 } - \bar{a}_\ell
}{  
	a_{ \jmath ( \ell ) + 1 } -  a_{ \jmath ( \ell ) }
}
\frac{ 
	a_{\jmath( \ell )} + c_{\underline{\kappa} ( \ell ) + 1} -  \bar{t}_\ell
}{  
	c_{ \underline{\kappa} ( \ell ) + 1 } -  c_{ \underline{\kappa} ( \ell ) }
}
\\
& + &
I_{\jmath ( \ell ), \underline{\kappa} ( \ell ) + 1}^{i( \ell ) }
( s_G )
\frac{ 
	a_{ \jmath ( \ell ) } - \bar{a}_\ell
}{  
	a_{ \jmath ( \ell ) + 1 } -  a_{ \jmath ( \ell ) }
}
\frac{ 
	\bar{t}_\ell - a_{\jmath ( \ell )} - c_{\underline{\kappa} ( \ell )}
}{  
	c_{ \underline{\kappa} ( \ell ) + 1 } -  c_{ \underline{\kappa} ( \ell ) }
}
\\
& + &
I_{\jmath ( \ell ) + 1 , \overline{\kappa} ( \ell )} ^{i( \ell ) }
( s_G )
\frac{ 
	\bar{a}_\ell - a_{ \jmath ( \ell ) }
}{  
	a_{ \jmath ( \ell ) + 1 } -  a_{ \jmath ( \ell ) }
}
\frac{ 
	a_{\jmath ( \ell )+1} + c_{\overline{\kappa} ( \ell )+1} -  \bar{t}_\ell
}{  
	c_{\overline{\kappa} ( \ell )+1} -  c_{\overline{\kappa}( \ell )}
}
\\
& + &
I_{\jmath ( \ell ) + 1 , \overline{\kappa} ( \ell ) + 1}^{i( \ell ) }
( s_G )
\frac{ 
	\bar{a}_\ell - a_{ \jmath ( \ell ) }
}{  
	a_{ \jmath ( \ell ) + 1 } -  a_{ \jmath ( \ell ) }
}
\frac{ 
	\bar{t}_\ell -  a_{\jmath ( \ell )+1} - c_{\overline{\kappa} ( \ell )}
}{  
	c_{\overline{\kappa} ( \ell )+1} -  c_{\overline{\kappa}( \ell )}
}
\end{array}
\] $$

$subhead residual$$
For $latex \ell = 0 , \ldots , L-1$$,
we use $latex R_\ell ( s_G , \beta )$$ to denote the measurement residuals 
which are defined by
$latex \[
R_\ell ( s_G , \beta )  
=  
R \left[ 
	Z_\ell ( \beta ) 
	\W{,} \bar{I}_\ell ( s_G ) \W{,}
	\zeta_\ell 
\right]
\] $$

$subhead scaled$$
For $latex \ell = 0 , \ldots , L-1$$,
the scaled residual is defined by
$latex \[
\left\{ \begin{array}{ll} 
R_\ell( s_G , \beta ) / \sigma_\ell 
	& \R{if} \; 0 < \sigma_\ell < + \infty
\\
\R{nan}
	& \R{otherwise}
\end{array} \right.
\] $$

$comment -------------------------------------------------------------------$$

$head message$$
The return value $icode message$$ has prototype
$codei%
	std::string %message%
%$$
Upon return, it has one of the following values:
$codep
	"optimize: failed for unknown reason"
	"optimize: success"
	"optimize: estimate is not as accurate as desired"
	"optimize: maximum number of iterations exceeded"
	"optimize: stopped because step size was to small"
	"optimize: stopped because estimate was diverging"
$$

$head MAP Optimization$$

$subhead Objective$$
The MAP objective corresponding to the residuals and likelihoods above is
$latex \[
\begin{array}{rcl}
f( s_G , \beta )
& = & 
\sum_{(j,k) \in G} 
\sum_{q=0}^5
H \left[
	A^{q,o}_{j,k} ( s_G )
	\W{,} \sigma^{q,a}_{j,k} \W{,} 
	h^{q,a}_{j,k} 
\right] 
+
H \left[
	B^{q,o}_{j,k} ( s_G )
	\W{,} \sigma^{q,c}_{j,k} \W{,} 
	h^{q,c}_{j,k} 
\right] 
\\
& + &
\sum_{(j,k) \in G} 
\sum_{q=0}^5
H \left[
	D^q_{j,k} ( s_G )
	\W{,} \sigma^{q,d}_{j,k} \W{,} 
	h^{q,d}_{j,k} 
\right] 
+
\sum_{(j,k) \in G} 
\sum_{q=0}^5
H \left[
	P^q_{j,k} ( s_G )
	\W{,} \sigma^{q,p}_{j,k} \W{,} 
	h^{q,p}_{j,k} 
\right] 
\\
& + &
\sum_{\ell = 1}^L 
H \left[
	R_\ell ( s_G , \beta )
	\W{,} \sigma_\ell \W{,} 
	h_\ell 
\right] 
\end{array}
\] $$

$subhead Problem$$
The return 
$cref/estimate/optimize/sfun_out/estimate/$$ $latex s_G$$ and
$cref/effect_out/optimize/effect_out/$$ $latex \beta$$ are approximate
solutions of the MAP estimate problem
$latex \[
\begin{array}{rll}
\R{minimize} & 
f ( s_G , \beta ) \W{\R{w.r.t}} ( s_G , \beta ) 
\\
\R{subject \; to} &
	s_G^l \leq  s_G \leq s_G^u \W{\R{and}} \beta^l \leq \beta \leq \beta^u
\end{array}
\] $$
The optimization is done with respect to the re-scaled variables
$latex \bar{s}_G$$; see
$cref/optimize_zeta/optimize/sfun_in/optimize_zeta/$$.

$children%
	omh/ipopt_trace.omh%
	example/optimize_xam.cpp%
	test/dismoditis.cpp
%$$

$head Example$$
The file $cref/optimize_xam.cpp/$$ contains an example and test
of $code optimize$$.
It returns true if $code optimize$$ passes the test
and false if it fails.
The file $cref/dismoditis.cpp/$$ is another example and test
of $code optimize$$.

$end

*/
# include <string>
# include <dismod_pde/define.hpp>
# include <dismod_pde/pair2index.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file optimize.hpp
Include file for \ref optimize().
*/
std::string optimize(
	const Optimize_Parameter_Struct&      parameter        ,
	const pair2index&                     grid             ,
	const vector<double>&                 covariate_value  ,
	const vector<Sfun_Input_Struct>&      sfun_in          ,
	vector<Sfun_Output_Struct>&           sfun_out         ,
	const vector<Measure_Input_Struct>&   measure_in       ,
	vector<Measure_Output_Struct>&        measure_out      ,
	const vector<Effect_Input_Struct>&    effect_in        ,
	vector<double>&                       effect_out
);

DISMOD4_END_NAMESPACE

# endif
