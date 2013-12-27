# ifndef DISMOD4_DEFINE_HPP
# define DISMOD4_DEFINE_HPP

# include <iostream>   // for DISMOD4_ASSERT_MSG
# include <cstddef>
# include <cppad/cppad.hpp>

/*!
\file define.hpp
Define preprocessor symbols and macros used by dismod_pde.
*/

/*!
\def DISMOD4_RESCALE_SFUN_OPT
Should the stochastic functions S, C, iota, rho, chi, and omega be scaled
using the offset log transformation just for optimization purposes
(1 for yes and 0 for no).
*/
# define DISMOD4_RESCALE_SFUN_OPT 1

/*!
\def DISMOD4_PLUS_INFINITY
Value dismod_pde uses to denote plus infinity.
*/
# define DISMOD4_PLUS_INFINITY 1e19


/*!
\def DISMOD4_BEGIN_NAMESPACE
Declares beginning of dismod_pde namespace in way that is not reconized by doxygen.
*/
# define DISMOD4_BEGIN_NAMESPACE namespace dismod_pde {

/*!
\def DISMOD4_END_NAMESPACE
Used for end that matches the beginning of a dismod_pde namespace.
*/
# define DISMOD4_END_NAMESPACE }

/*!
\def DISMOD4_ASSERT_MSG
Used to check for an error in the usage of dismod_pde.
If the logical expression is false, a message is printed and an assert
is generated. (This may change in the future to print the erorr message
through an error handler that can be redirected.)
$pre

$$
The number of times that the logical expression is evaluated is
not specified. For example it might be evaluated once, twice,
or not at all.
*/
# ifdef NDEBUG
# define DISMOD4_ASSERT_MSG(logical_expression, msg) // do nothing
# else
# define DISMOD4_ASSERT_MSG(logical_expression, msg)      \
{    if( ! ( logical_expression ) )                       \
          std::cerr << std::endl << msg << std::endl ;    \
     assert(logical_expression);                          \
}
# endif

DISMOD4_BEGIN_NAMESPACE

/*!
Simple vector template type used in dismod_pde.
*/
using CppAD::vector; 
using CppAD::AD;

/// The value not a number
inline double nan(void)
{	return( CppAD::nan( static_cast<double>(0.) ) ); }

/// Check for not a number
inline bool isnan(double x)
{	return CppAD::isnan(x); }

// --------------------------------------------------------------------------
/// Mathematical symbols used for the stochastic fucntions
enum Stochastic_Enum {
	/// fraction of people that can get condition, but do not have it
	S_enum,    
	/// fraction of people that have the condition
	C_enum,   
	/// incedence rate for getting the condition
	iota_enum, 
	/// remission rate for being cured of the condition
	rho_enum, 
	/// excess death rate due to the condition
	chi_enum, 
	// death rate due to other causes
	omega_enum ,

	// number of stochastic enums
	n_stochastic_enum
};
/// Class that maps from Stochastic_Enum to stochastic function name.
class Stochastic_Enum_Name {
private:
	/// mapping from enum value to name of stochastic function
	/// effectively const because set once by contstructor and never changed
	vector<std::string> name_;

	/// used to count how many times constructor is called
	static size_t ctor_count(void) 
	{	static size_t count = 0;
		return count++;
	}
public:
	/// constructor: should only be called once.
	Stochastic_Enum_Name(void)
	{
# ifndef NDEBUG
		size_t count = ctor_count();
		assert( count == 0 );
# endif
		//
		const char* name[] = {
			"S", "C", "iota", "rho", "chi", "omega"
		};
		const size_t n_enum = sizeof( name ) / sizeof( name[0] );
		assert( size_t(n_stochastic_enum) == n_enum );
		//
		name_.resize(n_enum);
		size_t q;
		for(q = 0; q < n_enum; q++)
			name_[q] = name[q];
	}
	/// use this member function to get mapping from enum to name 
	const vector<std::string>& name(void) const
	{	return name_; }
};
extern const Stochastic_Enum_Name stochastic_enum;
// --------------------------------------------------------------------------
/*
Likelhood function choices
*/
enum Likelihood_Enum { 
	/// Gaussian likelihood; i.e., square penalty
	gaussian_enum, 
	/// Laplace distribution; i.e., absolute value penalty
	laplace_enum,

	/// number of likelihood enums
	n_likelihood_enum
};
/// Class that maps from Likelihood_Enum to likelihood function name.
class Likelihood_Enum_Name {
private:
	/// mapping from enum value to name of likelihood function
	/// effectively const because set once by contstructor and never changed
	vector<std::string> name_;

	/// used to count how many times constructor is called
	static size_t ctor_count(void) 
	{	static size_t count = 0;
		return count++;
	}
public:
	/// constructor: should only be called once.
	Likelihood_Enum_Name(void)
	{
# ifndef NDEBUG
		size_t count = ctor_count();
		assert( count == 0 );
# endif
		//
		const char* name[] = {
			"gaussian", "laplace"
		};
		const size_t n_enum = sizeof( name ) / sizeof( name[0] );
		assert( size_t(n_likelihood_enum) == n_enum );
		//
		name_.resize(n_enum);
		size_t q;
		for(q = 0; q < n_enum; q++)
			name_[q] = name[q];
	}
	/// use this member function to get mapping from enum to name 
	const vector<std::string>& name(void) const
	{	return name_; }
};
extern const Likelihood_Enum_Name likelihood_enum;
// --------------------------------------------------------------------------

/*!
Measurement integrands
*/
enum Integrand_Enum { 
	/// incidence 
	incidence_enum, 
	/// all cause mortality 
	m_all_enum,
	/// excess mortality
	m_excess_enum,
	/// other cause mortality
	m_other_enum,
	/// prevalence times excess mortality
	m_prevalence_enum,
	/// cause specific mortality
	m_specific_enum,
	/// standardized mortality ratio
	m_standard_enum,
	/// with condition mortality
	m_with_enum,
	/// with-condition population size 
	pop_C_enum, 
	/// prevalence 
	prevalence_enum, 
	/// relative risk
	relative_risk_enum,
	/// remission 
	remission_enum,

	/// number of integrand enums
	n_integrand_enum
};
/// Class that maps from Integrand_Enum to integrand function name.
class Integrand_Enum_Name {
private:
	/// mapping from enum value to name of integrand function
	/// effectively const because set once by contstructor and never changed
	vector<std::string> name_;

	/// used to count how many times constructor is called
	static size_t ctor_count(void) 
	{	static size_t count = 0;
		return count++;
	}
public:
	/// constructor: should only be called once.
	Integrand_Enum_Name(void)
	{
# ifndef NDEBUG
		size_t count = ctor_count();
		assert( count == 0 );
# endif
		//
		const char* name[] = {
			"incidence"      ,
			"m_all"          ,
			"m_excess"       ,
			"m_other"        ,
			"m_prevalence"   ,
			"m_specific"     ,
			"m_standard"     ,
			"m_with"         ,
			"pop_C"          ,
			"prevalence"     ,
			"relative_risk"  ,
			"remission"
		};
		const size_t n_enum = sizeof( name ) / sizeof( name[0] );
		assert( size_t(n_integrand_enum) == n_enum );
		//
		name_.resize(n_enum);
		size_t q;
		for(q = 0; q < n_enum; q++)
			name_[q] = name[q];
	}
	/// use this member function to get mapping from enum to name 
	const vector<std::string>& name(void) const
	{	return name_; }
};
extern const Integrand_Enum_Name integrand_enum;
// --------------------------------------------------------------------------

/*!
Structure used to pass parameters to optimizer
*/
struct Optimize_Parameter_Struct {
	/// vector of age grid values
	vector<double> age_grid;
	/// vector of cohort grid values
	vector<double> cohort_grid;
	/// number of approximate ODE integrations steps in each age interval
	double max_ode_step;
	/// level of printing during optimization
	size_t print_level;
	/// maximum number of optimizer iterations
	size_t max_iteration;
	/// convergence tolerance
	double converge_tolerance;
	/// possibly slower execution with extra debugging output
	bool debug_output;
};		

/*!
Structure used to define model for one measurement 
*/
struct Measure_Input_Struct {
	/// integrand for this measurement
	Integrand_Enum integrand;

	/// lower limit for integration in time direction
	double time_lower;
	/// upper limit for integration in cohort direction
	double time_upper;
	/// lower limit for integration in age direction
	double age_lower;
	/// upper limit for integration in age direction
	double age_upper;

	/// normalized likelihood for this measurement
	Likelihood_Enum meas_like;
	/// standard deviation of residual of logs
	double meas_sigma;
	/// offset added before taking logs
	double meas_zeta;
	/// value of the measurement
	double meas_value;
};

/*!
Structure used to hold information for one measurement effect
*/
struct Effect_Input_Struct {
	/// integrand for this effect
	Integrand_Enum integrand;
	/// index of column in covariate_value maxtrix for this effect
	size_t covariate;
	/// lower limit for this effect
	double lower;
	/// initial estimate for this effect
	double initial;
	/// upper limit for this effect
	double upper;
};

/*!  
Structure used to define optimization input for one point where the
stochastic functions are defined.
*/
struct Sfun_Input_Struct {
	/// offset added before taking logs in scaled version of problem 
	double optimize_zeta;
	/// optimization lower bound
	double optimize_lower;
	/// initial point where the optimization procedure starts.
	double optimize_initial;
	/// optmization upper bound
	double optimize_upper;

	/// likelihood for the direct prior information
	Likelihood_Enum prior_like;
	/// offset added before taking logs
	double prior_zeta;
	/// standard deviation of the direct prior information 
	double prior_sigma;
	/// mean of the direct prior information 
	double prior_mean;

	/// normalized likelihood for differences in age directiion
	/// (cannot be uniform)
	Likelihood_Enum age_like;
	/// offset added before taking logs
	double age_zeta;
	/// standard deviation of residual of logs
	double age_sigma;
	// order for residuals in the age direction
	// (must be one for the functions S and C)
	size_t age_order;

	/// normalized likelihood for differences in cohort directiion
	/// (cannot be uniform)
	Likelihood_Enum cohort_like;
	/// offset added before taking logs
	double cohort_zeta;
	/// standard deviation of residual of logs
	double cohort_sigma;
	/// order for residuals in the cohort direction (must be one or two)
	size_t cohort_order;

	/// normalized likelihood for cross difference in cohort and age
	Likelihood_Enum cross_like;
	/// offset added before talking logs
	double cross_zeta;
	/// standard deviation of residual of logs
	double cross_sigma;

};

/*!
Structure used by optimizer to pass back information 
for one point where the stochastic fucntions are defined. 
*/
struct Sfun_Output_Struct {
	/// final estimate returned by the optimizer
	double estimate;
	/// prior residual 
	double prior_residual;
	/// prior scaled residual 
	double prior_scaled;
	/// age residual 
	double age_residual;
	/// age scaled residual
	double age_scaled;
	/// cohort residual 
	double cohort_residual;
	/// cohort scaled residual 
	double cohort_scaled;
	/// cross residual 
	double cross_residual;
	/// cross scaled residual 
	double cross_scaled;
};

/*!
Structure used by optimizer to pass back information for one measurement point
*/
struct Measure_Output_Struct {
	/// covariate adjusted measurement value 
	double adjusted;
	/// model for the adjusted measurement
	double model;
	/// residual for this measurement
	double residual;
	/// If sigma not zero, scaled residual, otherwise zero
	double scaled;
};


DISMOD4_END_NAMESPACE

# endif
