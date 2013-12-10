# ifndef DISMOD4_FG_INFO_HPP
# define DISMOD4_FG_INFO_HPP

# include <dismod_pde/define.hpp>
# include <dismod_pde/pair2index.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file fg_info.hpp
Include file for class that passes information to CppAD::ipopt::solve.
*/


/*!
Class for computing values needed by CppAD::ipopt::solve to compute
dismod_pde objective f(x), constraints g(x), and their derivatives.
*/
class FG_info 
{
public:
	typedef CppAD::AD<double>         ADdouble;
	typedef CppAD::vector<double>     Dvector;
	typedef CppAD::vector<ADdouble>   ADvector;
private:
	/// Parameter structure as passed to optimize
	const Optimize_Parameter_Struct&      parameter_;
	/// Converts from age index to value
	const vector<double>&                 age_;
	/// Converts cohort index to value
	const vector<double>&                 cohort_;
	/// convert grid indices to linear vector indices
	const pair2index&                     grid_;
	/// measured covariate values
	const vector<double>&                 covariate_value_;
	/// Information that is defined on each computational grid point
	const vector<Sfun_Input_Struct>&      sfun_in_;
	/// Measurement information as passed to optimize
	const vector<Measure_Input_Struct>&   measure_in_;
	/// Effects information as passed to optimize
	const vector<Effect_Input_Struct>&    effect_in_;

	/// Number of stochastic functions at each grid point.
	/// Currently these functions are S, C, iota, rho, chi, and omega,
	const size_t                          n_stochastic_;
	/// Number of measurement effects
	const size_t                          n_effect_;
	/// Numeric value that represents plus infinity for Ipopt limits.
	const double                          infinity_;

	/// Number of covariates
	/// (effectively const becauuse only set in the constructor.)
	size_t n_covariate_;
	/// Number of equality constraints
	/// (effectively const because only set in the constructor.)
	size_t n_equality_;
	/// Number of inequality constraints
	/// (effectively const because only set in the constructor.)
	size_t n_inequality_;
	/// Number of auxillary variables 
	/// (effectively const because only set in the constructor.)
	size_t n_auxillary_;

	/// Lower limits for argument in Ipopt optimization problem
	/// (effectively const because only set in the constructor.)
	Dvector                     x_lower_;
	/// Upper limits for argument in Ipopt optimization problem
	/// (effectively const because only set in the constructor.)
	Dvector                     x_upper_;
	/// Lower limits for constraints in Ipopt optimization problem
	/// (effectively const because only set in the constructor.)
	Dvector                     g_lower_;
	/// Upper limits for constraints in Ipopt optimization problem
	/// (effectively const because only set in the constructor.)
	Dvector                     g_upper_;

	/// age index for grid interval that surround measurement midpoint
	/// (Effectively const because only set in the constructor.)
	vector<size_t> age_midpoint_index_;
	/// cohort indices for grid interval that surround measurement midpoint
	/// (Effectively const because only set in the constructor.)
	vector<size_t> cohort_midpoint_index_first;
	vector<size_t> cohort_midpoint_index_second;
	/*!
	Interpolation weights for each of the four points surrounding the midpoint.
	If j = age_midpoint_index_(ell), kj = cohort_midpoint_index_first[ell],
	kjp = cohort_midpoint_index_second[ell], for p = 0, 1, 2, 3 
	the value midpoint_weight_[4*ell + p] is the weight 
	for  grid point (j, kj), (j, kj+1), (j+1, kjp), (j+1, kjp+1) respectively.
	(Effectively const because only set in the constructor.)
	*/
	vector<double> midpoint_weight_;
	//
	// documentation in fg_info_ingtegrand.cpp
	ADdouble compute_integrand(
		size_t           age_index          , 
		size_t           cohort_index       , 
		size_t           measure_index      , 
		const ADvector   &state
	) const;
	//
	// documentation in fg_info_pos.cpp
	void check_pos(
	const char*              describe           ,
	const vector<ADdouble>&  vec                ,
	size_t                   age_index          ,
	size_t                   cohort_index       ,
	const ADvector&          state
	) const;

public:
	/// Infinity as used in FG_info;
	/// computed by FG_info constructor and const.
	double infinity(void) const
	{	return infinity_; }

	/// Number of equality constraints
	/// computed by FG_info constructor and const.
	size_t n_equality(void) const
	{	return n_equality_; }

	/// Number of inequality constraints
	/// computed by FG_info constructor and const.
	size_t n_inequality(void) const
	{	return n_inequality_; }

	/// Number of auxillary variables 
	/// computed by FG_info constructor and const.
	size_t n_auxillary(void) const
	{	return n_auxillary_; };

	/// Number of measurement effects
	size_t n_effect(void) const
	{	return n_effect_; };

	/// Constraint lower limits;
	/// computed by FG_info constructor and effectively const.
	const Dvector &g_lower(void) const
	{	return g_lower_; }

	/// Constraint upper limits;
	/// computed by FG_info constructor and effectively const.
	const Dvector &g_upper(void) const
	{	return g_upper_; }

	/// Lower limits for optimization argument;
	/// computed by FG_info constructor and effectively const.
	const Dvector &x_lower(void) const
	{	return x_lower_; }

	/// Upper limits for optimization argument;
	/// computed by FG_info constructor and effectively const.
	const Dvector &x_upper(void) const
	{	return x_upper_; }

	FG_info(
		const Optimize_Parameter_Struct&        parameter       ,
		const vector<double>&                   age             ,
		const vector<double>&                   cohort          ,
		const pair2index&                       grid            ,
		const vector<double>&                   covariate_value ,
		const vector<Sfun_Input_Struct>&        sfun_in         ,
		const vector<Measure_Input_Struct>&     measure_in      ,
		const vector<Effect_Input_Struct>&      effect_in
	);
	// ----------------------------------------------------------------
	ADvector age_residual(
		size_t            age_index         , 
		size_t            cohort_index      , 
		const ADvector&   state             
	) const;
	ADdouble cohort_residual(
		size_t            age_index          , 
		size_t            cohort_index       , 
		Stochastic_Enum   stochastic         , 
		const ADvector&   state              
	) const;
	ADdouble cross_residual(
		size_t            age_index          , 
		size_t            cohort_index       , 
		Stochastic_Enum   stochastic         , 
		const ADvector&   state              
	) const;
	ADdouble prior_residual(
		size_t            age_index         , 
		size_t            cohort_index      , 
		Stochastic_Enum   stochastic        , 
		const ADvector&   state             
	) const;
	ADdouble measure_residual(
		size_t            measure_index     , 
		const ADvector&   state             ,
		const ADvector&   beta
	) const;
	ADdouble measure_model(
		size_t            measure_index     , 
		const ADvector&   state
	) const;
	ADdouble measure_adjusted(
		size_t            measure_index     , 
		const ADvector&   beta
	) const;
	// ----------------------------------------------------------------
	ADvector age_scaled(
		size_t            age_index         , 
		size_t            cohort_index      , 
		const ADvector&   state             
	) const;
	ADdouble cohort_scaled(
		size_t            age_index          , 
		size_t            cohort_index       , 
		Stochastic_Enum   stochastic         , 
		const ADvector&   state              
	) const;
	ADdouble cross_scaled(
		size_t            age_index          , 
		size_t            cohort_index       , 
		Stochastic_Enum   stochastic         , 
		const ADvector&   state              
	) const;
	ADdouble prior_scaled(
		size_t            age_index         , 
		size_t            cohort_index      , 
		Stochastic_Enum   stochastic        , 
		const ADvector&   state             
	) const;
	ADdouble measure_scaled(
		size_t            measure_index     , 
		const ADvector&   state             ,
		const ADvector&   beta
	) const;
	// ----------------------------------------------------------------

	// interface required by CppAD::ipopt::solve
	void operator()(ADvector& fg, const ADvector& x);
};

DISMOD4_END_NAMESPACE
# endif
