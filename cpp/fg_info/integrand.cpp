# include <dismod_pde/fg_info.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file fg_info_integrand.cpp
Implementaiton of the \ref FG_info member function 
\ref FG_info::compute_integrand(). 
*/
// ------------------------------------------------------------------------
/*!
Compute an integrand value.

\param [in] age_index
index that determines age for grid point.

\param [in] cohort_index
index that determnes cohort for grid point. 

\param [in] measure_index
index that determines which measurement.

\param [in] state
argument to the integrand function

\par
The specified grid point must be value in the sence of the
time limits corresponding to \a grid_.

\return 
the integrand correspdonding to this measurement and age-cohort grid point. 
*/
AD<double> FG_info::compute_integrand(
	size_t           age_index          , 
	size_t           cohort_index       , 
	size_t           measure_index      , 
	const ADvector&  state              ) const
{	ADdouble result = 0.;

	assert( age_index < age_.size() );
	assert( cohort_index < cohort_.size() );

	size_t grid_index;
# ifndef NDEBUG
	bool ok = grid_.pack(age_index, cohort_index, grid_index);
# else
	grid_.pack(age_index, cohort_index, grid_index);
# endif
	assert( ok );

	size_t index = grid_index * n_stochastic_;
	ADdouble iota, rho, C, S, omega, chi;
	Integrand_Enum integrand = measure_in_[measure_index].integrand;
	switch( integrand ) 
	{
		case incidence_enum:
		iota    = state[index + iota_enum];
		result  = iota;
		break;

		case m_all_enum:
		C       = state[index + C_enum]; 
		S       = state[index + S_enum]; 
		omega   = state[index + omega_enum]; 
		chi     = state[index + chi_enum]; 
		result  = omega + chi * C / (S + C);
		break;

		case m_excess_enum:
		chi     = state[index + chi_enum];
		result  = chi;
		break;

		case m_prevalence_enum:
		case m_specific_enum:
		chi     = state[index + chi_enum];
		C       = state[index + C_enum]; 
		S       = state[index + S_enum]; 
		result  = chi * C / (S + C);
		break;

		case m_other_enum:
		omega   = state[index + omega_enum];
		result  = omega;
		break;

		case m_standard_enum:
		C       = state[index + C_enum]; 
		S       = state[index + S_enum]; 
		omega   = state[index + omega_enum]; 
		chi     = state[index + chi_enum]; 
		result  = (omega + chi) / ( omega + chi * C / (S + C) );
		break;

		case m_with_enum:
		omega   = state[index + omega_enum]; 
		chi     = state[index + chi_enum]; 
		result  = omega + chi;
		break;

		case pop_C_enum:
		C       = state[index + C_enum];
		result  = C;
		break;

		case prevalence_enum:
		C       = state[index + C_enum]; 
		S       = state[index + S_enum]; 
		result  = C / (S + C);
		break;

		case relative_risk_enum:
		omega   = state[index + omega_enum]; 
		chi     = state[index + chi_enum]; 
		result  = (omega + chi) / omega;
		break;

		case remission_enum:
		rho     = state[index + rho_enum]; 
		result  = rho;
		break;

		default:
		assert(false);
	}
	return result;
}
DISMOD4_END_NAMESPACE
