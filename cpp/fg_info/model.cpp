# include <dismod_pde/fg_info.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file fg_info_model.cpp
Implementaiton of the \ref FG_info member function 
\ref FG_info::measure_model(). 
*/
// ------------------------------------------------------------------------
/*!
Compute the model for a measurement value.

\param [in] measure_index
index that determines which measurement

\param [in] state
argument to the residual function

\return
the model for the measurement value. 
*/
AD<double> FG_info::measure_model(
	size_t            measure_index    ,
	const ADvector&   state            ) const

{
	// age interval for this measurement 
	size_t j   = age_midpoint_index_[measure_index];
	// cohort interval for age[j]
	size_t kj  = cohort_midpoint_index_first[measure_index];
	// cohort interval for age[j+1]
	size_t kjp = cohort_midpoint_index_second[measure_index];

	// initialize summation
	ADdouble Ibar = 0.;

	// the four points in order
	// (j,kj), (j,kj+1), (j+1,kjp), (j+1,kjp+1)
	size_t i, ji, ki;
	for(i = 0; i < 4; i++)
	{	double weight = midpoint_weight_[4 * measure_index + i];
		switch(i)
		{	case 0:
			ji = j;
			ki = kj;
			break;

			case 1:
			ji = j;
			ki = kj + 1;
			break;

			case 2:
			ji = j + 1;
			ki = kjp;
			break;

			case 3:
			ji = j + 1;
			ki = kjp + 1;
			break;
		}	
		ADdouble integrand = compute_integrand(ji, ki, measure_index, state);
		Ibar += integrand * weight;
	}
	return Ibar;
}

DISMOD4_END_NAMESPACE
