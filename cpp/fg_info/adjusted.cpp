# include <dismod_pde/fg_info.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file fg_info_model.cpp
Implementaiton of the \ref FG_info member function 
\ref FG_info::measure_adjusted(). 
*/
// ------------------------------------------------------------------------
/*!
Compute the covariate adjusted value for a measurement.

\param [in] measure_index
index that determines which measurement

\param [in] beta
covariate multipliers for this correction.

\return
the covariate adjusted value for the measurement; i.e., Z_ell ( beta ) . 
*/
AD<double> FG_info::measure_adjusted(
	size_t            measure_index   ,
	const ADvector&   beta            ) const

{
	double   z_ell = measure_in_[measure_index].meas_value;
	ADdouble sum = 0.;
	size_t m;
	for(m = 0; m < n_effect_; m++)
	{	size_t n   = effect_in_[m].covariate;
		double x_i = covariate_value_[measure_index * n_covariate_ + n ];
		if( effect_in_[m].integrand == measure_in_[measure_index].integrand )
			sum = sum - x_i * beta[m];
	}
	return z_ell * exp( sum );
}

DISMOD4_END_NAMESPACE
