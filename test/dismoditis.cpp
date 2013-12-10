/*
$begin dismoditis.cpp$$
$spell
	pde
	csv
	sim
	sfun
	Dismod
	Dismoditis
$$

$section A dismod_pde C++ Optimize Example and Test$$
$index example, c++ optimize$$
$index optimize, c++ example$$
$index c++, optimize example$$


$children%
	test/dismoditis.sh.in
%$$
$head Create Simulated Data and Results in C++ Source$$
The source code below uses the external variables
$code  dismoditis_data$$ and $code  dismoditis_sim$$
which are created by $cref dismoditis.sh$$
with the help of $cref d4_simulate.py$$.

$code
$verbatim%test/dismoditis.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
# include <dismod_pde/create_grid.hpp>
# include <dismod_pde/optimize.hpp>

// measurement data as defined by dismoditis_data.cpp
extern dismod_pde::Measure_Input_Struct dismoditis_data[];
extern size_t n_dismoditis_data;

// solution as defined by dismoditis_sim.cpp
extern double dismoditis_sim[];
extern size_t n_dismoditis_sim;

namespace { // BEGIN empty namespace

dismod_pde::vector<double> solution(const dismod_pde::pair2index&  grid)
{	using dismod_pde::vector;

	// some temporaries
	size_t i, q;
	size_t j_sim, k_sim;
	size_t i_grid, j_grid, k_grid;
	

	// copy into solution into dismod_pde vector so that indexing gets checked
	size_t n_simulated = n_dismoditis_sim;
	vector<double> simulated(n_simulated);
	for(i = 0; i < n_simulated; i++)
		simulated[i] = dismoditis_sim[i];
	
	const size_t n_stochastic = dismod_pde::n_stochastic_enum;
	const size_t n_function   = 2 + n_stochastic;
	const size_t n_row        = n_dismoditis_sim / n_function;
	assert( n_simulated == n_row * n_function );

	// extract the age values corresponding to the solution
	size_t n_age_sim = 0;
	while( simulated[ n_age_sim * n_function + 0 ]  == simulated[0] )
		n_age_sim++;
	vector<double> age_sim(n_age_sim);
	for(j_sim = 0; j_sim < n_age_sim; j_sim++)
		age_sim[j_sim] = simulated[j_sim * n_function + 1 ];

	// extract the cohort grid corresponding to the solution
	size_t n_cohort_sim = n_row / n_age_sim;
	vector<double> cohort_sim(n_cohort_sim);
	for(k_sim = 0; k_sim < n_cohort_sim; k_sim++)
		cohort_sim[k_sim] = simulated[k_sim * n_age_sim * n_function + 0 ]; 

	// now values on the grid
	size_t n_grid = grid.size();
	vector<double> result(n_grid * n_stochastic);
	vector<double> age_grid    = grid.age();
	vector<double> cohort_grid = grid.cohort();

	// now fill it in
	for(i_grid = 0; i_grid < n_grid; i_grid++)
	{	grid.unpack(j_grid, k_grid, i_grid);

		// age and cohort values for this point on the grid
		double a = age_grid[j_grid];
		double c = cohort_grid[k_grid];

		// corresponding age and cohort indices in solution
		size_t j_sim = 0;
		while( age_sim[j_sim] < a && j_sim < n_age_sim - 1 )
			j_sim++;
		if( j_sim > 0 )
			j_sim--;
		size_t k_sim = 0;
		while( cohort_sim[k_sim] < c && k_sim < n_cohort_sim - 1 )
			k_sim++;
		if( k_sim > 0 )
			k_sim--;

		// now use bin-linear interpolation to convert from
		// surrounding solution values to grid value
		double da_sim = age_sim[j_sim    + 1] - age_sim[j_sim];
		double dc_sim = cohort_sim[k_sim + 1] - cohort_sim[k_sim];
		double r_j   = (age_sim[j_sim    + 1] - a) / da_sim;
		double r_k   = (cohort_sim[k_sim + 1] - c) / dc_sim;
		double r_jp  = (a - age_sim[j_sim    + 0]) / da_sim;
		double r_kp  = (c - cohort_sim[k_sim + 0]) / dc_sim;
		for(q = 0; q < n_stochastic; q++)
		{	i   = (k_sim *     n_age_sim + j_sim)     * n_function + q + 2;
			double f_j_k = simulated[i]; 
			//
			i   = (k_sim *     n_age_sim + j_sim + 1) * n_function + q + 2;
			double f_jp_k = simulated[i]; 
			//
			i   = ((k_sim+1) * n_age_sim + j_sim)     * n_function + q + 2;
			double f_j_kp = simulated[i]; 
			//
			i   = ((k_sim+1) * n_age_sim + j_sim + 1) * n_function + q + 2;
			double f_jp_kp = simulated[i]; 
			//
			// bilinear interpolation sum
			double sum = 0.;
			sum += r_j  * r_k  * f_j_k;
			sum += r_jp * r_k  * f_jp_k;
			sum += r_j  * r_kp * f_j_kp;
			sum += r_jp * r_kp * f_jp_kp;
			//
			result[ i_grid * n_stochastic + q ] = sum;;
		}
	}
	return result;
}

bool test_case(double delta_age, double delta_cohort)
{
	bool ok = true;

	size_t n_stochastic = dismod_pde::n_stochastic_enum;
	double infinity     = DISMOD4_PLUS_INFINITY;

	// import the dismod_pde vector type
	using dismod_pde::vector;

	// import the dismod_pde stochastic function names
	using dismod_pde::S_enum;
	using dismod_pde::C_enum;
	using dismod_pde::iota_enum;
	using dismod_pde::rho_enum;
	using dismod_pde::chi_enum;
	using dismod_pde::omega_enum;
	using dismod_pde::gaussian_enum;
	using dismod_pde::laplace_enum;
	using dismod_pde::vector;

	// temporary indices
	size_t i, j, k, ell, i_grid;

	// copy the measurement data into structure expected by dismod_pde
	size_t n_measure = n_dismoditis_data;
	vector<dismod_pde::Measure_Input_Struct> measure_in(n_measure);
	for(ell = 0; ell < n_measure; ell++)
		measure_in[ell] = dismoditis_data[ell];

	// object that packs non-rectangular grid in a linear array
	dismod_pde::pair2index grid = dismod_pde::create_grid(
			delta_age, delta_cohort, measure_in
	);

	// grid siz and number of stochastic function values
	size_t n_grid       = grid.size();
	size_t n_state      = n_stochastic * n_grid;

	// dismod_pde parameter struucture
	dismod_pde::Optimize_Parameter_Struct parameter;

	// maximum step size used for to approximate the ODE for S and C in age
	parameter.max_ode_step           = 1.;

	// level of printing during optimization
	parameter.print_level            = 5;

	// maximum number of optimizer iterations
	parameter.max_iteration          = 80;

	// optimizer convergence criteria
	parameter.converge_tolerance     = 1e-6;

	// extra debugging output 
	// (currently forces a retape every time the function argument changes)
	parameter.debug_output           = false;

	vector<dismod_pde::Sfun_Input_Struct> sfun_in(n_state);
	for(i = 0; i < n_state; i++)
	{
		sfun_in[i].prior_like            = gaussian_enum;
		sfun_in[i].age_like              = gaussian_enum;
		sfun_in[i].cohort_like           = gaussian_enum;
		sfun_in[i].cross_like            = gaussian_enum;

		sfun_in[i].prior_zeta            = 1.;
		sfun_in[i].age_zeta              = 1.;
		sfun_in[i].cohort_zeta           = 1.;
		sfun_in[i].cross_zeta            = 1.;

		sfun_in[i].prior_sigma           = infinity;
		sfun_in[i].age_sigma             = 1e-4;
		sfun_in[i].cohort_sigma          = 1e-4;
		sfun_in[i].cross_sigma           = infinity;

		sfun_in[i].prior_mean            = .05;
		sfun_in[i].age_order             = 1;
		sfun_in[i].cohort_order          = 1;
	}
	// make ODE deterministic for S and C in the age direction
	for(i_grid = 0; i_grid < n_grid; i_grid++)
	{	grid.unpack(j, k, i_grid);
		i = i_grid * n_stochastic;
		if( j == 0 )
		{	// use prior to set initial condition for S
			sfun_in[i + S_enum].prior_sigma = 0.;
			sfun_in[i + S_enum].prior_mean  = 1.;

			// use prior to set initial condition for C
			sfun_in[i + C_enum].prior_sigma = 0.;
			sfun_in[i + C_enum].prior_mean  = 0.;
		}
		// Make age direction of ODE deterministic for S and C
		// extra values corresponding to j(u, k) are not used
		sfun_in[i + S_enum].age_sigma       = 0.;
		sfun_in[i + C_enum].age_sigma       = 0.;

		// Turn off cohort direction residuals for S and C
		// sfun_in[i + S_enum].cohort_sigma       = infinity;
		// sfun_in[i + C_enum].cohort_sigma       = infinity;
	}


	for(i = 0; i < n_state; i++)
	{	sfun_in[i].optimize_zeta  = 1.0;	
		sfun_in[i].optimize_lower = 0.0;
		// initialize the estimate as equal to the prior
		sfun_in[i].optimize_initial = sfun_in[i].prior_mean;
		sfun_in[i].optimize_upper   = 1.0;
	}

	// no covariate information
	vector<double>                       covariate_value(0);
	vector<dismod_pde::Effect_Input_Struct> effect_in(0);

	// information returned by optimize
	vector<dismod_pde::Sfun_Output_Struct>    sfun_out(n_state);
	vector<dismod_pde::Measure_Output_Struct> measure_out(n_measure);
	vector<double>                         effect_out;
	std::string                            message;
	/*
	compute the estimate 
	*/
	message = dismod_pde::optimize(
		parameter,     grid,          covariate_value,
		sfun_in,       sfun_out, 
		measure_in,    measure_out,
		effect_in,     effect_out
	);

	// check that message is as expected
	ok &= (message == "optimize: success");

	// check the solution against the simulated values
	vector<double> simulated = solution(grid);
	double max_rel_diff = 0.;
	for(i = 0; i < n_state; i++) 
	{	double rel_diff  = fabs( 
			1. - (sfun_out[i].estimate + 1e-3) / (simulated[i] + 1e-3)
		);
		max_rel_diff = std::max(max_rel_diff, rel_diff);
		if( rel_diff >= 2e-2 )
		{	size_t q      = i % n_stochastic;
			size_t i_grid = i / n_stochastic;
			grid.unpack(j, k, i_grid);
			std::cout << "rel_diff = " << rel_diff;
			std::cout << ", age = "    << grid.age()[j];
			std::cout << ", cohort = " << grid.cohort()[k];
			std::cout << ", estimate = " << sfun_out[i].estimate;
			std::cout << ", simulated = " << simulated[i];
			std::cout << ", q = " << q;
			std::cout << std::endl;
		}
	}
	ok  &= max_rel_diff <= 2e-2;
	return ok;
}

	
} // END empty namespace

bool dismoditis(void)
{	bool ok             = true;
	double delta_age    = 20;
	double delta_cohort = 20;
	std::cout << "starting dismoditis:" << std::endl;
	ok &= test_case(delta_age, delta_cohort);
	if( ok )
		std::cout << "OK: dismoditis" << std::endl;
	else std::cout << "Error" << std::endl;
	return ok;

}
// END C++
