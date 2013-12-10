# include <dismod_pde/fg_info.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file fg_info_eval_r.cpp
Implementation of the \ref FG_info member function \ref FG_info::eval_r().
*/


/*!
\def DISMOD4_FG_INFO_EVAL_R_TRACE
Should the fg_info_eval_r print its retun fg vector each time it is called
(1 for yes and 0 for no).
*/
# define DISMOD4_FG_INFO_EVAL_R_TRACE 0

// ------------------------------------------------------------------------
/*!
This function is effectively const, but it needs to be the virtual
replacement for a non-const function.

\param [out] fg
The input elements of this vector does not matter, but it must 
have the correct size. Upon retunrn it is [ f(x), g(x) ]; i.e.,
the objective followed by the constraints.

\param [in] x
is a vector containing the state followed by values for the
Laplace distributed measurements.
*/

void FG_info::operator()(ADvector& fg, const ADvector&  x)
{	// temporaries
	size_t i, j, k, ell, q, i_grid, i_grid_p;

	// number of grid points
	const size_t n_grid       = grid_.size();
	// number of components in the state vector
	const size_t n_state      = n_grid * n_stochastic_;
	// number of measurements
	const size_t n_measure    = measure_in_.size();

	// check arguments
	assert( x.size() == n_state + n_auxillary_ + n_effect_ );
	assert( fg.size() ==  1 + n_equality_ + n_inequality_  );

	// split x into state, auxillary variables, and covariate multipliers
	ADvector state( n_state );
	ADvector aux( n_auxillary_ );
	ADvector beta( n_effect_ );
	for(i = 0; i < n_state; i++)
	{
# if DISMOD4_RESCALE_SFUN_OPT
		state[i] = exp( x[i] ) - sfun_in_[i].optimize_zeta;
# else
		state[i] = x[i];
# endif
	}
	for(i = 0; i < n_auxillary_; i++)
		aux[i] = x[n_state + i];
	for(i = 0; i < n_effect_; i++)
		beta[i] = x[n_state + n_auxillary_ + i];

	// declare the return vector

	// initialize summation for objective function f(x)
	ADdouble sum = 0.;

	/*
	now compute the objective f(x) and the constraints g(x)
	*/
	size_t aux_index = 0;
	size_t g_index   = 0;
	ADdouble res     = 0.;
	double   sigma   = 0.;
	bool     valid   = false;
	ADvector r_vec(n_stochastic_);
	for(i_grid = 0; i_grid < n_grid; i_grid++)
	{	grid_.unpack(j, k, i_grid);
		size_t jp = j+1;
		// first direct prior residuals for this grid point
		for(q = 0; q < n_stochastic_; q++)
		{	i     = i_grid * n_stochastic_ + q;
			sigma = sfun_in_[i].prior_sigma;
			res   = prior_scaled(
				j, k, Stochastic_Enum(q), state
			);
			if( sigma == infinity_ )
				;
			else if( sigma == 0. )
			{	assert( g_lower_[g_index] == 0. );
				assert( g_upper_[g_index] == 0. );
				// scale factor to better ensure constraints
				fg[++g_index] = res;
			}
			else	switch( sfun_in_[i].prior_like )
			{
				// f(x) = f(x) + res * res / 2
				case gaussian_enum:
				sum = sum + res * res / 2.;
				break;

				// f(x) = f(x) + aux[aux_index]
				// aux[aux_index] >= + sqrt(2) * res
				// aux[aux_index] >= - sqrt(2) * res
				case laplace_enum:
				sum = sum + aux[aux_index];
				//
				assert( g_lower_[g_index] == 0. );
				assert( g_upper_[g_index] == infinity_ );
				fg[++g_index] = aux[aux_index] - sqrt(2.) * res;
				//
				assert( g_lower_[g_index] == 0. );
				assert( g_upper_[g_index] == infinity_ );
				fg[++g_index] = aux[aux_index] + sqrt(2.) * res;
				//
				aux_index++;
				break;

				default:
				assert(false);
			}
		}
		valid = grid_.pack(jp, k, i_grid_p);
		if( valid )
		{	// second comes age residuals for this grid point
			r_vec = age_scaled(j, k, state);
			for(q = 0; q < n_stochastic_; q++)
			{	i     = i_grid * n_stochastic_ + q;
				sigma = sfun_in_[i].age_sigma;
				res   = r_vec[q];
				if( sfun_in_[i].age_order == 2 )
					valid = grid_.pack(jp+1, k, i_grid_p);
				else	valid = true;
				if( ! valid | (sigma == infinity_) )
					;
				else if( sigma == 0. )
				{	assert( g_lower_[g_index] == 0. );
					assert( g_upper_[g_index] == 0. );
					// scale factor to better ensure constraints
					fg[++g_index] = res;
				}
				else	switch( sfun_in_[i].age_like )
				{	// f(x) = f(x) + res * res / 2
					case gaussian_enum:
					sum = sum + res * res / 2.;
					break;

					// f(x) = f(x) + aux[aux_index]
					// aux[aux_index] >= + sqrt(2) * res
					// aux[aux_index] >= - sqrt(2) * res
					case laplace_enum:
					sum = sum + aux[aux_index];
					//
					assert( g_lower_[g_index] == 0. );
					assert( g_upper_[g_index] == infinity_ );
					fg[++g_index] = aux[aux_index] - sqrt(2.) * res;
					//
					assert( g_lower_[g_index] == 0. );
					assert( g_upper_[g_index] == infinity_ );
					fg[++g_index] = aux[aux_index] + sqrt(2.) * res;
					//
					aux_index++;
					break;

					default:
					assert(false);
				}
			}
		}
		valid = grid_.pack(j, k+1, i_grid_p);
		if( valid )
		{	// third comes cohort residuals for this grid point
			for(q = 0; q < n_stochastic_; q++)
			{	i     = i_grid * n_stochastic_ + q;
				sigma = sfun_in_[i].cohort_sigma;
				if( sfun_in_[i].cohort_order == 2 )
					valid = grid_.pack(j, k+2, i_grid_p);
				else	valid = true;
				if( ! valid | (sigma == infinity_) )
					;
				else
				{	res   = cohort_scaled(
						j, k, Stochastic_Enum(q), state
					);
					if( sigma == 0. )
					{	assert( g_lower_[g_index] == 0. );
						assert( g_upper_[g_index] == 0. );
						// scale factor to better ensure constraints
						fg[++g_index] = res;
					}
					else	switch(sfun_in_[i].cohort_like)
					{	// f(x) = f(x) + res * res / 2
						case gaussian_enum:
						sum = sum + res * res / 2.;
						break;

						// f(x) = f(x) + aux[aux_index]
						// aux[aux_index] >= + sqrt(2) * res
						// aux[aux_index] >= - sqrt(2) * res
						case laplace_enum:
						sum = sum + aux[aux_index];
						//
						assert( g_lower_[g_index] == 0. );
						assert( g_upper_[g_index] == infinity_ );
						fg[++g_index] = aux[aux_index] - sqrt(2.) * res;
						//
						assert( g_lower_[g_index] == 0. );
						assert( g_upper_[g_index] == infinity_ );
						fg[++g_index] = aux[aux_index] + sqrt(2.) * res;
						//
						aux_index++;
						break;

						default:
						assert(false);
					}
				}
			}
		} 
		valid  = true;
		valid &= grid_.pack(j,   k+1,  i_grid_p);
		valid &= grid_.pack(j+1,   k,  i_grid_p);
		valid &= grid_.pack(j+1,  k+1, i_grid_p);
		if( valid )
		{	// fourth comes cross residuals for this grid point
			for(q = 0; q < n_stochastic_; q++)
			{	i     = i_grid * n_stochastic_ + q;
				sigma = sfun_in_[i].cross_sigma;

				if( ! valid | (sigma == infinity_) )
					;
				else
				{	res = cross_scaled(
						j, k, Stochastic_Enum(q), state
					);
				 	if( sigma == 0. )
					{	assert( g_lower_[g_index] == 0. );
						assert( g_upper_[g_index] == 0. );
						fg[++g_index] = res;
					}
					else	switch( sfun_in_[i].cross_like )
					{	// f(x) = f(x) + res * res / 2
						case gaussian_enum:
						sum = sum + res * res / 2.;
						break;

						// f(x) = f(x) + aux[aux_index]
						// aux[aux_index] >= + sqrt(2) * res
						// aux[aux_index] >= - sqrt(2) * res
						case laplace_enum:
						sum = sum + aux[aux_index];
						//
						assert( g_lower_[g_index] == 0. );
						assert( g_upper_[g_index] == infinity_ );
						fg[++g_index] = aux[aux_index] - sqrt(2.) * res;
						//
						assert( g_lower_[g_index] == 0. );
						assert( g_upper_[g_index] == infinity_ );
						fg[++g_index] = aux[aux_index] + sqrt(2.) * res;
						//
						aux_index++;
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
	{	res = measure_scaled(ell, state, beta);
		if( measure_in_[ell].meas_sigma == infinity_ )
			;
		else if( measure_in_[ell].meas_sigma == 0. )
		{	assert( g_lower_[g_index] == 0. );
			assert( g_upper_[g_index] == 0. );
			// scale factor to better ensure constraints
			fg[++g_index] = res;
		}
		else	switch( measure_in_[ell].meas_like )
		{	// f(x) = f(x) + res * res / 2
			case gaussian_enum:
			sum = sum + res * res / 2.;
			break;

			// f(x) = f(x) + aux[aux_index]
			// aux[aux_index] >= + sqrt(2) * res
			// aux[aux_index] >= - sqrt(2) * res
			case laplace_enum:
			sum = sum + aux[aux_index];
			//
			assert( g_lower_[g_index] == 0. );
			assert( g_upper_[g_index] == infinity_ );
			fg[++g_index] = aux[aux_index] - sqrt(2.) * res;
			//
			assert( g_lower_[g_index] == 0. );
			assert( g_upper_[g_index] == infinity_ );
			fg[++g_index] = aux[aux_index] + sqrt(2.) * res;
			//
			aux_index++;
			break;

			default:
			assert(false);
		}
	}
	assert( g_index == n_equality_ + n_inequality_ );
	assert( aux_index == n_auxillary_ );
	fg[0] = sum;

	DISMOD4_ASSERT_MSG(
		! CppAD::hasnan(fg) ,
		"fg_info_eval_r: objective or constraint is not a number"
	);
# if DISMOD4_FG_INFO_EVAL_R_TRACE
	for(i = 0; i < fg.size(); i++)
		std::cout << "fg[" << i << "] = " << fg[i] << std::endl;
# else
# endif
	return;
}

DISMOD4_END_NAMESPACE
