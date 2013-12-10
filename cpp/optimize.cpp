# include <dismod_pde/optimize.hpp>
# include <dismod_pde/fg_info.hpp>
# include <cppad/ipopt/solve.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file optimize.cpp
Implemtnation of \ref dismod_pde::optimize() routine.
*/

/*!
Routine that optimizes the dismod_pde objective.

This routine is completely specified in the user OMhelp documentation
for dismod_pde.

\param [in] parameter
See \ref Optimize_Parameter_Struct.

\param [in] grid
defines the age-cohort grid that is used for computation.
Also converts from age-chort grid pair to single grid index.

\param [in] covariate_value
is a vector with size <code>L * N</code>.
The value for the n-th covariate at measurement ell is
<code>covariate_value[ell*N + n]</code>.

\param [in] sfun_in
Optimization input information that is defined for each point
on the computational grid, see \ref Sfun_Input_Struct.

\param [out] sfun_out
Optimization return information that is computed for each point
on the computational grid, see \ref Sfun_Output_Struct.

\param [in] measure_in
Information about each measurement. See \ref Measure_Input_Struct.

\param [out] measure_out
Model residuals define on the age-cohort grid
and corresponding to the final value of \c estimate in \a sfun_out.

\param [in] effect_in
is a vector with size <code>M</code> (the number of measurement effects).
The 
$code Integrand Enum integrand$$, 
$code size_t covariate$$,
$code double lower$$,
$code double initial$$
$code double upper$$
are include for each measurement effect.  

\param [out] effect_out
is a vector with size \c M.
The input value of its elements does not matter.
Upon return, it is the optimal estimate for 
the measurement effect vector \c beta.

\return 
It no error occurs, the return value is \c "optimize: success".
*/
std::string optimize(
	const Optimize_Parameter_Struct&      parameter       ,
	const pair2index&                     grid            ,
	const vector<double>&                 covariate_value ,
	const vector<Sfun_Input_Struct>&      sfun_in         ,
	vector<Sfun_Output_Struct>&           sfun_out        ,
	const vector<Measure_Input_Struct>&   measure_in      ,
	vector<Measure_Output_Struct>&        measure_out     ,
	const vector<Effect_Input_Struct>&    effect_in       ,
	vector<double>&                       effect_out      )
{
	typedef vector<double>       Dvector;
	typedef vector< AD<double> > ADvector;

	// some temporaries
	size_t i, j, k, ell, q, i_grid, i_grid_p;

	// age grid
	const vector<double> age( grid.age() );
	// cohort grid
	const vector<double> cohort( grid.cohort() );

	// value used for plus infinity
	Ipopt::Number infinity = DISMOD4_PLUS_INFINITY;
	// name of the stochastic function
	const vector<std::string> function = stochastic_enum.name();
	// number of stochastic functions
	const size_t n_stochastic = function.size();
	// number of measurements
	const size_t n_measure    = measure_in.size();
	// number of grid points
	const size_t n_grid       = grid.size();
	// number of components in the state vector
	const size_t n_state      = n_grid * n_stochastic;
	// number of effects
	const size_t n_effect     = effect_out.size();

	// check some input values
	assert( n_state == sfun_in.size() );
	assert( n_state == sfun_out.size() );
	assert( n_measure == measure_out.size() );
	assert( n_effect == effect_in.size() );

# ifndef NDEBUG
	// number of age values
	const size_t n_age        = age.size();
	assert( n_age > 1 );

	// number of cohort values
	const size_t n_cohort     = cohort.size();
	assert( n_cohort > 1);

	for(ell = 0; ell < n_measure; ell++)
	{	bool ok = measure_in[ell].age_lower  >= age[0];
		ok     &= measure_in[ell].age_upper  <= age[n_age-1];
		if( ! ok )
		{	using std::endl;
			std::ostringstream msg;
			msg << endl;
			msg << "optimize: error in measurement index " << ell << endl;
			msg << "age_lower < age[1] or age[n_age-1] < age_upper"; 
			msg << endl;
			DISMOD4_ASSERT_MSG(ok, msg.str());
		}
	}
	size_t order;
	for(i_grid = 0; i_grid < n_grid; i_grid++)
	{	grid.unpack(j, k, i_grid);
		for(q = 0; q < n_stochastic; q++)
		{	i        = i_grid * n_stochastic + q;
			order    = sfun_in[i].cohort_order;
			bool ok  = (order == 1) | (order == 2);
			order    = sfun_in[i].age_order;
			if( order == 2 )
				ok &= (q != S_enum) & (q != C_enum);
			else	ok &= (order == 1);
			if( ! ok )
			{	using std::endl;
				std::ostringstream msg;
				msg << endl;
				msg << "optimize: error in order " << endl;
				msg << "age = " << age[j]; 
				msg << ", cohort = " << cohort[k]; 
				msg << ", function = " << function[q]; 
				msg << endl;
				DISMOD4_ASSERT_MSG(ok, msg.str());
			}
		}
	}
# endif
	// construct the CppAD::ipopt::solve call back function object
	FG_info fg_info(
		parameter       ,
		age             ,
		cohort          ,
		grid            ,
		covariate_value ,
		sfun_in         ,
		measure_in      ,
		effect_in
	);

	// Determine the number of auxillary variables and number of constraints 
	size_t n_auxillary  = fg_info.n_auxillary();
	size_t n_equality   = fg_info.n_equality();
	size_t n_inequality = fg_info.n_inequality();
	assert( n_effect == fg_info.n_effect() );

	// number of arguments (size of x) in ipopt optimization problem
	size_t n_ipopt = n_state + n_auxillary + n_effect;
	assert( n_ipopt == fg_info.x_lower().size() );
	assert( n_ipopt == fg_info.x_upper().size() );

	// number of constraints (range for g) in ipopt optimization problem
	size_t m_ipopt = n_equality + n_inequality; 
	assert( m_ipopt == fg_info.g_lower().size() );
	assert( m_ipopt == fg_info.g_upper().size() );

	// initial value for the state vector (in AD form)
	ADvector state(n_state);
	for(i = 0; i < n_state; i++)
		state[i] = sfun_in[i].optimize_initial;

	// initial value for covariate multipliers (in AD form)
	ADvector beta( n_effect );
	for(i = 0; i < n_effect; i++)
		beta[i] = effect_in[i].initial;

	// Determine initial values for the auxillary variables
	Dvector aux( n_auxillary );
	size_t aux_index    = 0;
	Ipopt::Number s_r   = 0.;
	double        sigma = 0.;
	bool          valid = false;
	ADvector s_r_vec(n_stochastic);
	for(i_grid = 0; i_grid < n_grid; i_grid++)
	{	grid.unpack(j, k, i_grid);
		size_t jp = j+1;
		// first direct prior residuals for this grid point
		for(q = 0; q < n_stochastic; q++)
		{	i     = i_grid * n_stochastic + q;
			sigma =  sfun_in[i].prior_sigma;
			if( (sigma != 0.) & (sigma != infinity) )
			{	s_r = Value( fg_info.prior_scaled(
					j, k, Stochastic_Enum(q), state
				));
				switch( sfun_in[i].prior_like )
				{	case gaussian_enum:
					break;

					case laplace_enum:
					aux[aux_index++] = sqrt(2.) * fabs(double(s_r));
					break;

					default:
					assert(false);
				}
			}
		}
		valid = grid.pack(jp, k, i_grid_p );
		if( valid )
		{	// second comes age residuals for this grid point
			s_r_vec = fg_info.age_scaled(j, k, state);
			for(q = 0; q < n_stochastic; q++)
			{	i     = i_grid * n_stochastic + q;
				sigma = sfun_in[i].age_sigma;
				if( sfun_in[i].age_order == 2 )
					valid = grid.pack(jp+1, k, i_grid_p);
				else	valid = true;
				if( valid & (sigma != 0.) & (sigma != infinity) )
				{	s_r = Value( s_r_vec[q] );
					switch( sfun_in[i].age_like )
					{	case gaussian_enum:
						break;

						case laplace_enum:
						aux[aux_index++] = sqrt(2.) * fabs(double(s_r));
						break;

						default:
						assert(false);
					}
				}
			}
		}
		valid = grid.pack(j, k+1, i_grid_p);
		if( valid )
		{	// third comes cohort residuals for this grid point
			for(q = 0; q < n_stochastic; q++)
			{	i     = i_grid * n_stochastic + q;
				sigma = sfun_in[i].cohort_sigma;
				if( sfun_in[i].cohort_order == 2 )
					valid = grid.pack(j, k+2, i_grid_p);
				else	valid = true;
				if( valid & (sigma != 0.) & (sigma != infinity) )
				{	s_r = Value( fg_info.cohort_scaled(
						j, k, Stochastic_Enum(q), state
					));
					switch( sfun_in[i].cohort_like )
					{	case gaussian_enum:
						break;

						case laplace_enum:
						aux[aux_index++] = sqrt(2.) * fabs(double(s_r));
						break;

						default:
						assert(false);
					}
				}
			}
		}
		valid = true;
		valid &= grid.pack(j,   k+1, i_grid_p);
		valid &= grid.pack(j+1,   k, i_grid_p);
		valid &= grid.pack(j+1, k+1, i_grid_p);
		if( valid )
		{	// fourth comes cross residuals for this grid point
			for(q = 0; q < n_stochastic; q++)
			{	i     = i_grid * n_stochastic + q;
				sigma = sfun_in[i].cross_sigma;
				if( (sigma != 0.) & (sigma != infinity) )
				{	s_r = Value( fg_info.cross_scaled(
						j, k, Stochastic_Enum(q), state
					));
					switch( sfun_in[i].cross_like )
					{	case gaussian_enum:
						break;

						case laplace_enum:
						aux[aux_index++] = sqrt(2.) * fabs(double(s_r));
						break;

						default:
						assert(false);
					}
				}
			}
		}
	} // end loop over grid residuals
	// last come the measurements residuals
	for(ell = 0; ell < n_measure; ell++)
	{	if( (measure_in[ell].meas_sigma != 0.)       & 
		    (measure_in[ell].meas_sigma != infinity) )
		{	s_r = Value( fg_info.measure_scaled(ell, state, beta) );
			// ADdouble may not be efficient, but no big deal and
			// avoids template definition of measure_scaled.
			switch( measure_in[ell].meas_like )
			{	case gaussian_enum:
				break;
	
				case laplace_enum:
				aux[aux_index++] = sqrt(2.) * fabs(double(s_r));
				break;

				default:
				assert(false);
			}
		}
	}
	assert( aux_index == n_auxillary );

	// initial components for x
	Dvector x_initial(n_ipopt);
	for(i = 0; i < n_state; i++)
	{
		// following is checked by fg_info constructor
		assert( sfun_in[i].optimize_zeta + sfun_in[i].optimize_lower > 0. );
# ifndef NDEBUG
		if( sfun_in[i].optimize_initial < sfun_in[i].optimize_lower  ||
		    sfun_in[i].optimize_initial > sfun_in[i].optimize_upper  )
		{	using std::endl;
			std::ostringstream msg;
			i_grid = i / n_stochastic;	
			q      = i % n_stochastic;
			grid.unpack(j, k, i_grid);
			msg << endl;
			msg << "optimize: sfun = " <<  function[q];
			msg << ", age = " << age[j];
			msg << ", cohort = " << cohort[k];
			msg << ", lower = " << sfun_in[i].optimize_lower;
			msg << ", initial = " << sfun_in[i].optimize_initial;
			msg << ", upper = " << sfun_in[i].optimize_upper;
			msg << endl;
			DISMOD4_ASSERT_MSG(false, msg.str());
		}
# endif
# if DISMOD4_RESCALE_SFUN_OPT
		x_initial[i] = log( 
			sfun_in[i].optimize_zeta + sfun_in[i].optimize_initial
		);
# else
		x_initial[i] = sfun_in[i].optimize_initial;
# endif
	}

	for(i = 0; i < n_auxillary; i++)
		x_initial[n_state + i] = aux[i];

	for(i = 0; i < n_effect; i++)
		x_initial[n_state + n_auxillary + i] = Value( beta[i] );

	// ----------------------------------------------------------------------
	// Use CppAD::ipopt::solve to solve the problem
	// ----------------------------------------------------------------------
	std::ostringstream options;

	// Do not retape
	options << "Retape false\n";

	// Use sparse forward mode (reverse seems to work just as well)
	options << "Sparse true forward\n";

	// level of output generatoed by ipopt
	options << "Integer print_level " << parameter.print_level << '\n';

	// maximum number of iterations
	options << "Integer max_iter " << parameter.max_iteration << '\n';

	// lower bound that represents minus infinity
	options << "Numeric nlp_lower_bound_inf " << -infinity << '\n';

	// upper bound that represents plus infinity
	options << "Numeric nlp_upper_bound_inf " << +infinity << '\n';

	// approximate accuracy in first order necessary conditions;
	// see Mathematical Programming, Volume 106, Number 1, 
	// Pages 25-57, Equation (6)
	options << "Numeric tol " << parameter.converge_tolerance << '\n';

	// use exact Hessian
	options << "String hessian_approximation exact\n";

# if 0
	// derivative testing
	options << "String derivative_test second-order\n";
	options << "Numeric point_perturbation_radius 0.1\n";
# endif

	// Default strategy for the interior point penalty parameter
	options << "String mu_strategy monotone\n";

	// defaults: init = 0.1, mu_factor = 0.2, power = 1.5, barrier_factor=10.
	options << "Numeric mu_init 0.1\n";
	options << "Numeric mu_linear_decrease_factor 0.1\n";
	options << "Numeric mu_superlinear_decrease_power 1.5\n";
	options << "Numeric barrier_tol_factor 10.0\n";

	// print user options
	options << "String print_user_options yes\n";
	

	CppAD::ipopt::solve_result<Dvector> solution;

	CppAD::ipopt::solve<Dvector, FG_info>(
		options.str(),
		x_initial,
		fg_info.x_lower(),
		fg_info.x_upper(),
		fg_info.g_lower(),
		fg_info.g_upper(),
		fg_info,
		solution
	);


	// return message
	std::string message;
	switch( solution.status )
	{	typedef CppAD::ipopt::solve_result<Dvector> result;
		case result::not_defined:
		case result::unknown:
		case result::local_infeasibility:
		case result::user_requested_stop:
		case result::restoration_failure:
		case result::error_in_step_computation:
		case result::invalid_number_detected:
		case result::internal_error:
		message = "optimize: failed for unknown reason";
		break;

		case result::success:
		message = "optimize: success";
		break;

		case result::maxiter_exceeded:
		message = "optimize: maximum number of iterations exceeded";
		break;

		case result::stop_at_tiny_step:
		message = "optimize: stopped because step size was to small";
		break;

		case result::stop_at_acceptable_point:
		message = "optimize: estimate is not as accurate as desired";
		break;

		case result::diverging_iterates:
		message = "optimize: stopped because estimate was diverging";
		break;

		default:
		assert(false);
	}


	// return estimate for stochastic functions
	for(i = 0; i < n_state; i++)
	{
# if DISMOD4_RESCALE_SFUN_OPT
		sfun_out[i].estimate = exp(solution.x[i]) - sfun_in[i].optimize_zeta;
# else
		sfun_out[i].estimate = solution.x[i];
# endif
		state[i]             = sfun_out[i].estimate;
	}

	// return estimate for covariate multiplier
	for(i = 0; i < n_effect; i++)
		beta[i] = effect_out[i] = solution.x[n_state + n_auxillary + i];

	// return residual information
	ADvector residual_vec(n_stochastic);
	ADvector scaled_vec(n_stochastic);
	for(i_grid = 0; i_grid < n_grid; i_grid++)
	{	grid.unpack(j, k, i_grid);
		size_t jp = j+1;
		// first direct prior residuals for this grid point
		for(q = 0; q < n_stochastic; q++)
		{	i     = i_grid * n_stochastic + q;
			sfun_out[i].prior_residual = Value( 
				fg_info.prior_residual( j, k, Stochastic_Enum(q), state)
			);
			sfun_out[i].prior_scaled = Value( 
				fg_info.prior_scaled( j, k, Stochastic_Enum(q), state)
			);
			//
			sigma = sfun_in[i].prior_sigma;
			if( sigma == 0. || sigma == infinity )
				sfun_out[i].prior_scaled = dismod_pde::nan();
			//
			// for valid residuals, these will be overwritten 
			sfun_out[i].age_residual    = dismod_pde::nan();
			sfun_out[i].age_scaled      = dismod_pde::nan();
			sfun_out[i].cohort_residual = dismod_pde::nan();
			sfun_out[i].cohort_scaled   = dismod_pde::nan();
			sfun_out[i].cross_residual  = dismod_pde::nan();
			sfun_out[i].cross_scaled    = dismod_pde::nan();
		}
		valid = grid.pack(jp, k, i_grid_p);
		if( valid )
		{	// second comes age residuals for this grid point
			residual_vec = fg_info.age_residual(j, k, state);
			scaled_vec   = fg_info.age_scaled(j, k, state);
			for(q = 0; q < n_stochastic; q++)
			{	i     = i_grid * n_stochastic + q;	
				sigma = sfun_in[i].age_sigma;
				if( sfun_in[i].age_order == 2 )
					valid = grid.pack(jp+1, k, i_grid_p);
				else	valid = true;
				if( valid )
					sfun_out[i].age_residual = Value( residual_vec[q] );
				if( valid & (sigma != 0.) & (sigma != infinity) )
					sfun_out[i].age_scaled   = Value( scaled_vec[q] );
			}
		}
		valid = grid.pack(j, k+1, i_grid_p);
		if( valid )
		{	// third comes cohort residuals for this grid point
			for(q = 0; q < n_stochastic; q++)
			{	i     = i_grid * n_stochastic + q;
				sigma = sfun_in[i].cohort_sigma;
				if( sfun_in[i].cohort_order == 2 )
					valid = grid.pack(j, k+2, i_grid_p);
				else	valid = true;
				if( valid )
				{	sfun_out[i].cohort_residual = Value( 
						fg_info.cohort_residual( 
							j, k, Stochastic_Enum(q), state
					));
				}
				if( valid & (sigma != 0.) & (sigma != infinity) )
				{	sfun_out[i].cohort_scaled = Value( 
						fg_info.cohort_scaled( 
							j, k, Stochastic_Enum(q), state
					));
				}
			}
		}
		valid  = true;
		valid &= grid.pack(j+1,   k, i_grid_p);
		valid &= grid.pack(  j, k+1, i_grid_p);
		valid &= grid.pack(j+1, k+1, i_grid_p);
		if( valid )
		{	// fourth comes cross residuals for this grid point
			for(q = 0; q < n_stochastic; q++)
			{	i     = i_grid * n_stochastic + q;
				sigma = sfun_in[i].cross_sigma;
				sfun_out[i].cross_residual = Value( 
					fg_info.cross_residual( 
							j, k, Stochastic_Enum(q), state
				));
				if( (sigma != 0.) & (sigma != infinity) )
				{	sfun_out[i].cross_scaled = Value( 
						fg_info.cross_scaled( 
							j, k, Stochastic_Enum(q), state
					));
				}
			}
		}
	} // loop over grid residuals
	// last come the measurement residuals
	for(ell = 0; ell < n_measure; ell++)
	{	measure_out[ell].adjusted = Value(
			fg_info.measure_adjusted(ell, beta)
		);
		measure_out[ell].model     = Value(
			fg_info.measure_model(ell, state)
		);
		measure_out[ell].residual = Value(
			fg_info.measure_residual(ell, state, beta)
		);
		measure_out[ell].scaled    = Value(
			fg_info.measure_scaled(ell, state, beta)
		);
		sigma = measure_in[ell].meas_sigma;
		if( sigma == 0 || sigma == infinity )
			measure_out[ell].scaled = dismod_pde::nan(); 
	}

	return message;
}

DISMOD4_END_NAMESPACE
