# include <dismod_pde/fg_info.hpp>
# include <dismod_pde/sc_ode.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file fg_info_cross.cpp
Implementation of the \ref FG_info member functions
\ref FG_info::cross_residual() and \ref FG_info::cross_scaled().
*/

// ------------------------------------------------------------------------
/*!
Compute the cross term residual for one of the stochastic functions.

\return
residuals for one of the stochastic functions at the specified grid point. 

\param [in] age_index
index that determines age at this grid point for this residual
(referred to as j above and below).

\param [in] cohort_index
index that determines cohort  at this grid point for this residual
(referred to as k above and below). 

\param [in] stochastic
the stochastic function

\param [in] state
argument to the residual function.

\par Assumptions
-# It is assumed that 
(j,k), (j+1,k), (j,k+1), (j+1,k+1) are all valid grid pairs.
-# It is assumed that the stochastic function is not
S_enum or C_enum.
*/
AD<double> FG_info::cross_residual(
	size_t           age_index         , 
	size_t           cohort_index      , 
	Stochastic_Enum  stochastic        ,
	const ADvector   &state            ) const
{
	size_t k         = cohort_index;
	size_t j         = age_index;
	//
	size_t g_index_jk, g_index_jpk, g_index_jkp, g_index_jpkp;
# ifndef NDEBUG
	bool ok_jk    = grid_.pack(j,   k,   g_index_jk);
# else
	grid_.pack(j,   k,   g_index_jk);
# endif
# ifndef NDEBUG
	bool ok_jpk   = grid_.pack(j+1, k,   g_index_jpk);
# else
	grid_.pack(j+1, k,   g_index_jpk);
# endif
# ifndef NDEBUG
	bool ok_jkp   = grid_.pack(j,   k+1, g_index_jkp);
# else
	grid_.pack(j,   k+1, g_index_jkp);
# endif
# ifndef NDEBUG
	bool ok_jpkp  = grid_.pack(j+1, k+1, g_index_jpkp);
# else
	grid_.pack(j+1, k+1, g_index_jpkp);
# endif
	assert( ok_jk & ok_jpk & ok_jkp & ok_jpkp );
	//
	size_t s_index_jk   = g_index_jk   * n_stochastic_ + stochastic;
	size_t s_index_jpk  = g_index_jpk  * n_stochastic_ + stochastic;
	size_t s_index_jkp  = g_index_jkp  * n_stochastic_ + stochastic;
	size_t s_index_jpkp = g_index_jpkp * n_stochastic_ + stochastic;
	//
	double root_da   = sqrt(age_[j+1] - age_[j]);
	double root_dc   = sqrt(cohort_[k+1] - cohort_[k]);
	double zeta      = sfun_in_[s_index_jk].cross_zeta;
	//
	ADdouble f_jk   = log( state[s_index_jk]   + zeta );
	ADdouble f_jpk  = log( state[s_index_jpk]  + zeta );
	ADdouble f_jkp  = log( state[s_index_jkp]  + zeta );
	ADdouble f_jpkp = log( state[s_index_jpkp] + zeta );
	//
	ADdouble num    = (f_jpkp - f_jpk - f_jkp + f_jk);
	double   den    = (root_da * root_dc); 

	return num / den;
}

// ------------------------------------------------------------------------
/*!
Compute the scaled cross residual for one the stochastic functions
at one grid point.

\return
Residual divided by standard deviation.
In the special case where the standard devaition is zero or infinity,
the return value is the residual.

\param [in] age_index
index that determines age at this grid point
(referred to as j below).

\param [in] cohort_index
index that determines cohort at this grid point
(referred to as k below).

\param [in] stochastic
stochastic function

\param [in] state
argument to the residual function

\par Assumptions
-# It is assumed that 
(j,k), (j+1,k), (j,k+1), (j+1,k+1) are all valid grid pairs.
-# It is assumed that the stochastic function is not
S_enum or C_enum.

*/
AD<double> FG_info::cross_scaled(
	size_t           age_index         , 
	size_t           cohort_index      , 
	Stochastic_Enum  stochastic        ,
	const ADvector   &state            ) const
{
	// cohort_residual asserts that this is a valid difference 
	ADdouble r = cross_residual(age_index, cohort_index, stochastic, state);
	//
	// check for sigma zero or infinity
	size_t i_grid;
	grid_.pack(age_index, cohort_index, i_grid);
	size_t i     = i_grid   * n_stochastic_ + stochastic;
	double sigma = sfun_in_[i].cross_sigma;
	if( (sigma == 0.) | (sigma == infinity_) )
		return r;
	DISMOD4_ASSERT_MSG(
		! CppAD::isnan(r),
		"fg_info::a cohort_scaled residual is not a number"
	);
	return r / sigma;
}


DISMOD4_END_NAMESPACE
