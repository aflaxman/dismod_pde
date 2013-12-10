# include <dismod_pde/create_grid.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file create_grid.cpp
Implementation of the routine \ref create_grid().
*/

/*!
Create an Age-Cohort Grid For a Measurement Set

\param [in] delta_age
must be greater than zero and
specifies the step size used between age values in the grid.

\param [in] delta_cohort
must be greater than zero and
specifies the step size used between cohort values in the grid.

\param [in] measure_in
This structure that defines the measurements,
and the model for the measurements,
that will be used to fit the stochastic functions
on the resulting grid.
See \ref optimize().

\return
The is an age cohort grid that surrounds the measurement values
by a spacing of \c delta_age and \c delta_cohort.

*/
pair2index create_grid(
	double                               delta_age       ,
	double                               delta_cohort    ,
	const vector<Measure_Input_Struct>&  measure_in      )
{	
	// some constants
	const size_t n_measure = measure_in.size();
	assert( delta_age > 0. );
	assert( delta_cohort > 0.);

	// some temporaries
	size_t j, k, ell;

	double age_lower    = measure_in[0].age_lower;
	double age_upper    = measure_in[0].age_upper;
	double time_lower   = measure_in[0].time_lower; 
	double time_upper   = measure_in[0].time_upper;
	for(ell = 1; ell < n_measure; ell++)
	{	age_lower      = std::min(age_lower, measure_in[ell].age_lower);
		age_upper      = std::max(age_upper, measure_in[ell].age_upper);
		time_lower     = std::min(time_lower, measure_in[ell].time_lower);
		time_upper     = std::max(time_upper, measure_in[ell].time_upper);
	}
	assert( age_lower >= 0.0 );
	age_lower = 0.0;

	// adjust age_upper to be a multiple of delta_age
	if( fmod(age_upper, delta_age) != 0. )
		age_upper = age_upper + delta_age - fmod(age_upper, delta_age);

	// now determine the number of age values
	size_t n_age = 1 + int( .5 + (age_upper - age_lower) / delta_age );

	// Define the age values to start at age_lower and end at age_upper.
	vector<double> age(n_age);
	for(j = 1; j < n_age-1; j++)
		age[j] = age_lower + j * delta_age;
	age[0]       = age_lower;
	age[n_age-1] = age_upper;

	// Determine cohort limits that span the region of interest
	double cohort_lower = time_lower - age_upper;
	double cohort_upper = time_upper - age_lower;

	// adjust lower cohort limit to be a multiple of delta_cohort
	cohort_lower -= fmod(cohort_lower, delta_cohort); 

	// adjust upper cohort limit to be a multiple of delta_cohort
	if( fmod(cohort_upper, delta_cohort) != 0 )
		cohort_upper += delta_cohort - fmod(cohort_upper, delta_cohort); 

	// now determine the number of cohort values
	size_t n_cohort = 1 + int( 
			.5 + (cohort_upper - cohort_lower) / delta_cohort 
	);
	// now evenly space the cohort grid from cohort_lower to cohort_upper
	vector<double> cohort(n_cohort);
	for(k = 1; k < n_cohort-1; k++)
		cohort[k] = cohort_lower + k * delta_cohort;
	cohort[0]          = cohort_lower;
	cohort[n_cohort-1] = cohort_upper;

	// construct the return value
	pair2index grid(age, cohort, time_lower, time_upper);

	return grid;
}

DISMOD4_END_NAMESPACE
