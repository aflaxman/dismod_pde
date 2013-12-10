# include <dismod_pde/fg_info.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file fg_info_measure.cpp
Implementaiton of the \ref FG_info member functions
\ref FG_info::measure_residual() and \ref FG_info::measure_scaled(). 
*/

// ------------------------------------------------------------------------
/*!
Compute a residual for a measurement value.

\param [in] measure_index
index that determines which measurement

\param [in] state
argument to the residual function

\param [in] beta
values for the covariate multipliers

\return
the residual for this measurement; i.e. R_ell( state, beta). 
*/
AD<double> FG_info::measure_residual(
	size_t          measure_index     , 
	const ADvector& state             ,
	const ADvector& beta              ) const
{
	ADdouble model = measure_model(measure_index, state);
	ADdouble Z_ell = measure_adjusted(measure_index, beta);
	double zeta    = measure_in_[measure_index].meas_zeta;
	// note that log(z + zeta) does not change and could be cached
	// by fg_info's constructor.
	ADdouble ret = log(zeta + Z_ell) - log(zeta + model);
	return ret;
}

// ------------------------------------------------------------------------
/*!
Compute a scaled residual for a measurement value.

\return 
Residual divided by the corresponding standard deviation. 
In the special case where the standard devaition is zero or infinity,
the return value is the residual.

\param [in] measure_index
index that determines which measurement

\param [in] state
argument to the residual function

\param [in] beta
values for the covariate multipliers
*/
AD<double> FG_info::measure_scaled(
	size_t          measure_index     , 
	const ADvector& state             ,
	const ADvector& beta              ) const
{
	double sigma = measure_in_[measure_index].meas_sigma;
	ADdouble r   = measure_residual(measure_index, state, beta);
	if( (sigma != 0.) & (sigma != infinity_) )
		r = r / sigma;
	DISMOD4_ASSERT_MSG(
		! CppAD::isnan(r),
		"fg_info::a measure_scaled residual is not a number"
	);
	return r;
}

DISMOD4_END_NAMESPACE
