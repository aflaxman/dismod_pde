# include <dismod_pde/fg_info.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file fg_info_cohort.cpp
Implementation of the \ref FG_info member functions
\ref FG_info::cohort_residual() and \ref FG_info::cohort_scaled().
*/

// ------------------------------------------------------------------------
/*!
Compute a residual in cohort direction for stochastic functions.

\return 
residual 

\param [in] age_index
index that determines age for the grid point.

\param [in] cohort_index
index that determines cohort for the grid point.
Both this value and <code>cohort_index + 1</code>
must be valid grid points.

\param [in] stochastic
stochastic function 

\param [in] state
argument to the residual function
*/
AD<double> FG_info::cohort_residual(
	size_t           age_index          , 
	size_t           cohort_index       , 
	Stochastic_Enum  stochastic         , 
	const ADvector   &state             ) const
{
	size_t j         = age_index;
	size_t k         = cohort_index;
	size_t kp        = cohort_index+1;
	// 
	size_t g_index_k, g_index_kp, g_index_kpp;
# ifndef NDEBUG
	bool ok_k        = grid_.pack(j, k,   g_index_k);
# else
	grid_.pack(j, k,   g_index_k);
# endif
# ifndef NDEBUG
	bool ok_kp       = grid_.pack(j, kp,  g_index_kp);
# else
	grid_.pack(j, kp,  g_index_kp);
# endif
	assert( ok_k & ok_kp );
	//
	size_t s_index_k    = g_index_k   * n_stochastic_ + stochastic;
	size_t s_index_kp   = g_index_kp  * n_stochastic_ + stochastic;
	double dc_k         = cohort_[kp] - cohort_[k];
	//
	size_t order_k      = sfun_in_[s_index_k].cohort_order;
	double zeta_k       = sfun_in_[s_index_k].cohort_zeta;
	ADdouble df_k       = log(state[s_index_kp] + zeta_k)
	                    - log(state[s_index_k] + zeta_k);
	if( order_k == 1 ) 
		return df_k;
	//
	assert( order_k == 2 );
	size_t kpp          = cohort_index+2;
# ifndef NDEBUG
	bool ok_kpp         = grid_.pack(j, kpp, g_index_kpp);
# else
	grid_.pack(j, kpp, g_index_kpp);
# endif
	assert( ok_kpp );
	size_t s_index_kpp  = g_index_kpp * n_stochastic_ + stochastic;
	//
	double dc_kp       = cohort_[kpp] - cohort_[kp];
	ADdouble df_kp     = log(state[s_index_kpp] + zeta_k) 
	                   - log(state[s_index_kp]  + zeta_k);
	//
	ADdouble ddf_k     = df_kp / dc_kp - df_k / dc_k;
	return ddf_k;
}

// ------------------------------------------------------------------------
/*!
Compute a scaled residual in cohort direction for one stochastic function
at one grid point.

\return 
Residual divided by the corresponding
cohort standard deviation times square root of change in cohort.
In the special case where the standard devaition is zero or infinity,
the return value is the residual.

\param [in] age_index
index that determines age 

\param [in] cohort_index
index that determines cohort for the grid point.
Both this value and <code>cohort_index + 1</code>
must be valid grid points.

\param [in] stochastic
stochastic function 

\param [in] state
argument to the residual function
*/
AD<double> FG_info::cohort_scaled(
	size_t           age_index          , 
	size_t           cohort_index       , 
	Stochastic_Enum  stochastic         , 
	const ADvector&  state              ) const
{
	// cohort_residual asserts that this is a valid difference 
	ADdouble r = cohort_residual(age_index, cohort_index, stochastic, state);
	//
	// check for sigma zero or infinity
	size_t i_grid;
	grid_.pack(age_index, cohort_index, i_grid);
	size_t i     = i_grid   * n_stochastic_ + stochastic;
	double sigma = sfun_in_[i].cohort_sigma;
	if( (sigma == 0.) | (sigma == infinity_) )
		return r;
	DISMOD4_ASSERT_MSG(
		! CppAD::isnan(r),
		"fg_info::a cohort_scaled residual is not a number"
	);
	return r / sigma;
}

DISMOD4_END_NAMESPACE
