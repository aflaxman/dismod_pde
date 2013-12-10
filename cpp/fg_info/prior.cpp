# include <dismod_pde/fg_info.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file fg_info_prior.cpp
Implementation of the \ref FG_info member functions
\ref FG_info::prior_residual() and \ref FG_info::prior_scaled().
*/

// ------------------------------------------------------------------------
/*!
Compute residual for one point in prior estimate.

\return
residual 

\param [in] age_index 
index that determines age value for this grid point.

\param [in] cohort_index
index that determines cohort value for this grid point.

\param [in] stochastic 
stochastic function

\par
The pair \a age_index , \a cohort_index
must correspond to a valid grid point in the sense of \c grid_.

\param [in] state
argument to the residual function
*/
AD<double> FG_info::prior_residual(
	size_t            age_index         , 
	size_t            cohort_index      , 
	Stochastic_Enum   stochastic        , 
	const ADvector&   state             ) const
{
	size_t grid_index;
# ifndef NDEBUG
	bool ok = grid_.pack(age_index, cohort_index, grid_index);
# else
	grid_.pack(age_index, cohort_index, grid_index);
# endif
	assert( ok );

	size_t index         = grid_index * n_stochastic_ + stochastic;
	double zeta          = sfun_in_[index].prior_zeta;
	double mean          = sfun_in_[index].prior_mean;
	ADdouble ret;
	ret = log(state[index] + zeta) - log(mean + zeta);
	return ret;
}

// ------------------------------------------------------------------------
/*!
Compute scaled residual for one stochastic function and at one
grid point.

\return 
Residual divided by the corresponding prior standard deviation. 
In the special case where the standard devaition is zero,
the return value is the residual.

\param [in] age_index 
index that determines age value for this grid point.

\param [in] cohort_index
index that determines cohort value for this grid point.

\param [in] stochastic 
stochastic function

\param [in] state
argument to the residual function

\par
The pair \a age_index , \a cohort_index
must correspond to a valid grid point in the sense of \c grid_.
*/
AD<double> FG_info::prior_scaled(
	size_t            age_index         , 
	size_t            cohort_index      , 
	Stochastic_Enum   stochastic        , 
	const ADvector&   state             ) const
{
	size_t grid_index;
# ifndef NDEBUG
	bool ok = grid_.pack(age_index, cohort_index, grid_index);
# else
	grid_.pack(age_index, cohort_index, grid_index);
# endif
	assert( ok );

	size_t index = grid_index * n_stochastic_ + stochastic;
	double sigma = sfun_in_[index].prior_sigma;
	ADdouble res = prior_residual(age_index, cohort_index, stochastic, state);
	if( (sigma != 0.) & (sigma != infinity_) )
		res = res / sigma;
	DISMOD4_ASSERT_MSG(
		! CppAD::isnan(res),
		"fg_info::a prior_scaled residual is not a number"
	);
	return res;
}

DISMOD4_END_NAMESPACE
