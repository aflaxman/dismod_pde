/*
$begin optimize_xam.cpp$$
$spell
	pde
	sfun
	est
	bool
	dismod
	xam
	Enum
$$

$section Example and Test of dismod_pde C++ Optimize Procedure$$
$index test, c++ optimize$$
$index example, c++ optimize$$
$index optimize, c++ example$$ 
$index c++, optimize example$$


$head Syntax$$
$icode%ok% = optimize_xam(
	%n_age%, %n_cohort%, %likelihood%
)%$$

$head n_age$$
$index n_age, C++ optimize example$$
The argument $icode n_age$$ has prototype
$codei%
	size_t %n_age%
%$$.
It specifies the number of points in the age grid for this test case.

$head n_cohort$$
$index n_cohort, C++ optimize example$$
The argument $icode n_cohort$$ has prototype
$codei%
	size_t %n_cohort%
%$$.
It specifies the number of points in the cohort grid for this test case.

$head likelihood$$
$index likelihood, C++ optimize example$$
The argument $icode likelihood$$ has prototype
$codei%
	dismod_pde::vector< dismod_pde::Likelihood_Enum > likelihood;
%$$
$table
$icode%likelihood%[0]%$$ $cnext likelihood for even index direct prior 
$rnext
$icode%likelihood%[1]%$$ $cnext likelihood for odd index direct prior 
$rnext
$icode%likelihood%[2]%$$ $cnext likelihood for age residuals
$rnext
$icode%likelihood%[4]%$$ $cnext likelihood for cohort residuals
$rnext
$icode%likelihood%[4]%$$ $cnext likelihood for cross residuals
$rnext
$icode%likelihood%[5]%$$ $cnext likelihood for measurements
$tend

$head ok$$
The return value $icode ok$$ has prototype
$codei%
	bool %ok% 
%$$
If true, the $cref/optimize/$$ procedure returned a successful status; i.e.,
estimated values agreed with the simulated values.

$code
$verbatim%example/optimize_xam.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/

// BEGIN C++
# include <dismod_pde/dismod_pde.hpp>
# include <cmath>
# include <cstdlib>
# include <iomanip>

// BEGIN empty namespace
namespace {

	// import the dismod_pde vector type
	using dismod_pde::vector;

	// import the dismod_pde stochastic function names
	using dismod_pde::S_enum;
	using dismod_pde::C_enum;
	using dismod_pde::iota_enum;
	using dismod_pde::rho_enum;
	using dismod_pde::chi_enum;
	using dismod_pde::omega_enum;

	// number of stochastic functions
	const size_t n_stochastic_ = dismod_pde::n_stochastic_enum;

	// get the stochastic functions at one grid point
	void get_state_i(
		const vector<double>&          state       , 
		size_t                         i           , 
		double&                        S_i         ,
		double&                        C_i         ,
		double&                        i_i         ,
		double&                        r_i         ,
		double&                        x_i         ,
		double&                        u_i         )
	{
		S_i = state[ i * n_stochastic_ + S_enum ];
		C_i = state[ i * n_stochastic_ + C_enum ];
		i_i = state[ i * n_stochastic_ + iota_enum ];
		r_i = state[ i * n_stochastic_ + rho_enum ];
		x_i = state[ i * n_stochastic_ + chi_enum ];
		u_i = state[ i * n_stochastic_ + omega_enum ];
	}

	bool near_equal(double value, double check, double anorm)
	{	return fabs(value - check) <= 1e-12 * anorm;
	}
		
}
// END empty namespace

bool optimize_xam(
	size_t n_age                                         , 
	size_t n_cohort                                      , 
	dismod_pde::vector<dismod_pde::Likelihood_Enum> likelihood )
{	// initialize return value
	bool ok = true;
	assert(n_age > 2 );
	assert(n_cohort > 3 );

	// simulation values for rate constants
	const double iota_sim = 1e-2;
	const double rho_sim  = 2e-2;
	const double chi_sim  = 3e-2;
	const double omega_sim= 4e-2;

	// some temporaries
	size_t i, j, k, ell;
	double S_j, C_j, iota_j, rho_j, chi_j, omega_j;
	double iota_jp, rho_jp, chi_jp, omega_jp;

	// limits on age
	double age_lower = 0.0;
	double age_upper = 40.;

	// grided values for age 
	vector<double> age(n_age);
	double delta_age = (age_upper - age_lower) / (n_age - 1);
	for(j = 1; j < n_age-1; j++)
		age[j] = age_lower + delta_age * j;
	age[0] = age_lower;
	age[n_age-1] = age_upper;

	// limits on time
	double time_lower = 2000.;
	double time_upper = 2005.;

	// limits on cohort
	double cohort_lower = time_lower - age_upper;
	double cohort_upper = time_upper - age_lower;

	// grided values for cohort 
	double delta_cohort = (cohort_upper - cohort_lower) / (n_cohort - 1);
	vector<double> cohort(n_cohort);
	for(k = 1; k < n_cohort-1; k++)
		cohort[k] = cohort_lower + k * delta_cohort;
	cohort[0]          = cohort_lower;
	cohort[n_cohort-1] = cohort_upper;

	// object that packs non-rectangular grid in a linear array
	dismod_pde::pair2index grid(age, cohort, time_lower, time_upper);

	// use differential equation to simulated value for 
	// variables S, C at all ages:
	vector<double> S_sim(n_age), C_sim(n_age);
	S_sim[0] = 1.;
	C_sim[0] = 0.;
	for(j = 0; j < n_age-1; j++)
	{	// note same simulated value used for iota, rho, chi, omega below
		iota_j  = iota_jp = iota_sim;
		rho_j   = rho_jp  = rho_sim;
		chi_j   = chi_jp  = chi_sim;
		omega_j = omega_jp= omega_sim;
		// Use Huen's method and a small step size 
		// to get solution of ODE with no noise.
		size_t nstep = 200;
		double da    = (age[j+1] - age[j]) / nstep;
		double iota, rho, chi, omega, Sm, Cm;
		double S     = S_sim[j];
		double C     = C_sim[j];
		for(i = 0; i < nstep; i++)
		{	iota = iota_j + (iota_jp - iota_j) * i / nstep; 
			rho  = rho_j  + (rho_jp  - rho_j)  * i / nstep; 
			chi  = chi_j  + (chi_jp  - chi_j) * i / nstep; 
			omega= omega_j+ (omega_jp- omega_j)   * i / nstep; 
			Sm   = S + (rho * C - (iota + omega) * S) * da / 2.;
			Cm   = C + (iota * S - (rho + omega + chi) * C) * da / 2.;
			S    = S + (rho * Cm - (iota + omega) * Sm) * da;
			C    = C + (iota * Sm - (rho + omega + chi) * Cm) * da;
		}
		S_sim[j+1] = S;
		C_sim[j+1] = C;
	}

	// simulate value for state vector
	size_t n_grid       = grid.size();
	size_t n_state      = n_stochastic_ * n_grid;
	vector<double> simulated(n_state); 
	for(i = 0; i < n_grid; i++)
	{	// simulated values for the stochastic variables 
		// iota, rho, chi, and omega
		grid.unpack(j, k, i);
		simulated[ i * n_stochastic_ + S_enum ] = S_sim[j];
		simulated[ i * n_stochastic_ + C_enum ] = C_sim[j];
		// note no-noise case uses same value for 
		// iota, rho, chi, and omega as above
		simulated[ i * n_stochastic_ + iota_enum ] = iota_sim;
		simulated[ i * n_stochastic_ + rho_enum  ] = rho_sim;
		simulated[ i * n_stochastic_ + chi_enum  ] = chi_sim;
		simulated[ i * n_stochastic_ + omega_enum] = omega_sim;
	}
	// dismod_pde parameter structure
	dismod_pde::Optimize_Parameter_Struct parameter;

	// numbe of integration steps per age interval
	parameter.max_ode_step           = 1.;

	// level of printing during optimization
	parameter.print_level            = 0;

	// maximum number of optimizer iterations to attempt
	parameter.max_iteration          = 100;

	// optimizer convergence criteria
	parameter.converge_tolerance     = 1e-6;

	// extra debugging output 
	// (currently forces a retape every time the function argument changes)
	parameter.debug_output           = false;

	// the constant infinity
	const double infinity = DISMOD4_PLUS_INFINITY;

	// values specified for each computional grid point
	vector<dismod_pde::Sfun_Input_Struct> sfun_in(n_state);
	for(i = 0; i < n_state; i++)
	{	// optimize
		sfun_in[i].optimize_zeta    = 1.0;
		sfun_in[i].optimize_lower   = 0.0;
		sfun_in[i].optimize_initial = .05; 
		sfun_in[i].optimize_upper   = infinity;
		// prior
		if( i % 2 == 0 )
			sfun_in[i].prior_like  = likelihood[0];
		else	sfun_in[i].prior_like  = likelihood[1];
		sfun_in[i].prior_zeta       = .1;
		sfun_in[i].prior_sigma      = .2;
		sfun_in[i].prior_mean       = simulated[i];

		// likelihood for residuals in age dir
		sfun_in[i].age_like              = likelihood[2];
		sfun_in[i].age_zeta              = .3;
		sfun_in[i].age_sigma             = .4;
		sfun_in[i].age_order             = 1;

		// likelihood for residuals in cohort dir
		sfun_in[i].cohort_like           = likelihood[3];
		sfun_in[i].cohort_zeta           = .5;
		sfun_in[i].cohort_sigma          = .6;
		sfun_in[i].cohort_order          = 1;

		// likelihood for cross term residuals
		sfun_in[i].cross_like            = likelihood[4];
		sfun_in[i].cross_zeta            = .7;
		sfun_in[i].cross_sigma           = .8;
	}
	// make ODE deterministic for S and C in the age direction
	for(i = 0; i < n_grid; i++)
	{	grid.unpack(j, k, i);
		if( j == 0 )
		{	// initial condition for ODE
			sfun_in[i * n_stochastic_ + S_enum].prior_mean  = 1.;
			sfun_in[i * n_stochastic_ + S_enum].prior_sigma = 0.;
			//
			sfun_in[i * n_stochastic_ + C_enum].prior_mean  = 0.;
			sfun_in[i * n_stochastic_ + C_enum].prior_sigma = 0.;
		}
		// extra values corresponding to j(u, k) are not used
		sfun_in[i * n_stochastic_ + S_enum].age_sigma        = 0.;
		sfun_in[i * n_stochastic_ + C_enum].age_sigma        = 0.;
	}


	// determine the number of grid points where we can center a measurement
	size_t n_measure_point = 0;
	for(j = 1; j < n_age-1; j++)
	{	for(k = 1; k < n_cohort-1; k++)
		{	bool ok_mm   = grid.pack(j-1, k-1, i);
			bool ok_pp   = grid.pack(j+1, k+1, i);
			if( ok_mm & ok_pp )
				++n_measure_point;
		}
	}

	// noiseless measurements corresponding to the simulated values
	size_t n_integrand = dismod_pde::n_integrand_enum;
	size_t n_measure   = n_integrand * n_measure_point;
	ell      = 0;
	vector< dismod_pde::Measure_Input_Struct > measure_in(n_measure);
	vector<double> I(n_integrand);
	for(j = 1; j < n_age-1; j++)
	{	for(k = 1; k < n_cohort-1; k++)
		{	bool ok_mm   = grid.pack(j-1, k-1, i);
			bool ok_pp   = grid.pack(j+1, k+1, i);
			if( ok_mm & ok_pp )
			{	// minimum age distance to near by grid point
				double da = std::min(age[j] - age[j-1], age[j+1] - age[j]);

				// age limits of integration so do not pass a grid point
				double a_low = age[j] - da / 2.;
				double a_up  = age[j] + da / 2.;

				// minimum cohort distance to near by grid point
				double dc = std::min(
					cohort[k] - cohort[k-1], cohort[k+1] - cohort[k]
				);

				// time limits of integration so do not pass a cohort value
				double t_low = age[j] + cohort[k] - dc / 2.;
				double t_up  = age[j] + cohort[k] + dc / 2.;

				// values at this grid point
# ifndef NDEBUG
				bool ok = grid.pack(j, k, i);
				assert( ok );
# endif
				get_state_i(simulated, i,
					S_j, C_j, iota_j, rho_j, chi_j, omega_j
				);
				I[dismod_pde::incidence_enum]
					= iota_j;
				I[dismod_pde::remission_enum]  
					= rho_j;
				I[dismod_pde::m_excess_enum]   
					= chi_j;
				I[dismod_pde::m_other_enum] 
					= omega_j;
				I[dismod_pde::m_with_enum] 
					= omega_j + chi_j;
				I[dismod_pde::prevalence_enum] 
					= C_j / ( S_j + C_j );
				I[dismod_pde::m_prevalence_enum] 
					= chi_j * I[dismod_pde::prevalence_enum];
				I[dismod_pde::m_specific_enum] 
					= I[dismod_pde::m_prevalence_enum];
				I[dismod_pde::m_all_enum]
					= omega_j + I[dismod_pde::m_prevalence_enum];
				I[dismod_pde::m_standard_enum] 
					= I[dismod_pde::m_with_enum] / I[dismod_pde::m_all_enum];
				I[dismod_pde::relative_risk_enum] 
					= I[dismod_pde::m_with_enum] / omega_j;
				for(i = 0; i < n_integrand; i++)
				{
					measure_in[ell].integrand  = 
						dismod_pde::Integrand_Enum(i);
					measure_in[ell].age_lower       = a_low;
					measure_in[ell].age_upper       = a_up;
					measure_in[ell].time_lower      = t_low;
					measure_in[ell].time_upper      = t_up;
					measure_in[ell].meas_zeta       = 1.;
					measure_in[ell].meas_sigma      = 1.;
					measure_in[ell].meas_value      = I[i];
					measure_in[ell].meas_like       = likelihood[5];
					ell++;
				}
			}
		} // end k
	} // end j

	// no covariate information
	vector<double>                       covariate_value(0);
	vector<dismod_pde::Effect_Input_Struct> effect_in(0);

	// information returned by optimize
	vector<dismod_pde::Sfun_Output_Struct>    sfun_out(n_state);
	vector<dismod_pde::Measure_Output_Struct> measure_out(n_measure);
	vector<double>                         effect_out;
	std::string                            message;
	/*
	optimize the estimate 
	*/
	message = dismod_pde::optimize(
		parameter,    grid,          covariate_value,
		sfun_in,      sfun_out, 
		measure_in,   measure_out,
		effect_in,    effect_out
	);

	// check that message is as expected
	ok &= (message == "optimize: success");

	// check sfun_out estimates 
	double max_abs_diff = 0.;
	size_t i_max        = 0;
	for(i = 0; i < n_state; i++)
	{	double diff  = sfun_out[i].estimate - simulated[i];
		if( fabs(diff) > max_abs_diff )
			i_max = i;
		max_abs_diff = std::max(max_abs_diff, fabs(diff) );
	} 
	ok  &= max_abs_diff <= 1e-4;
	if( max_abs_diff > 1e-4 )
	{	size_t i_grid = i_max / n_stochastic_;
		size_t q      = i_max % n_stochastic_;
		grid.unpack(j, k, i_grid);
		std::cout << "optimize_xam:";
		std::cout << "  j = " << j;
		std::cout << ", k = " << k;
		std::cout << ", q = " << q;
		std::cout << ", max_abs_diff = " << max_abs_diff;
		std::cout << std::endl;
	}

	// check measure_out residuals
	for(ell = 0; ell < n_measure; ell++)
	{	double zeta        = measure_in[ell].meas_zeta;
		double sigma       = measure_in[ell].meas_sigma;
		double value       = measure_in[ell].meas_value;
		//
		double model       = measure_out[ell].model;
		double adjusted   = measure_out[ell].adjusted;
		double residual    = measure_out[ell].residual ;
		double scaled      = measure_out[ell].scaled ;
		//
		double check, anorm;
		check   = log( adjusted + zeta ) - log( model + zeta );
		anorm   = fabs(log( adjusted + zeta )) + fabs(log( model + zeta ));
		ok     &= near_equal(residual, check, anorm);
		if( sigma == 0. || sigma == infinity )
			ok = isnan(scaled);
		else
		{	check   = check / sigma;
			anorm   = anorm / sigma;
			ok     &= near_equal(scaled, check, anorm);
		}
		// No covartiates in this example so 
		ok &= adjusted == value;
	}

	// check sfun_out prior residuals
	for(i = 0; i < n_state; i++)
	{	double zeta      = sfun_in[i].prior_zeta;
		double sigma     = sfun_in[i].prior_sigma;
		double mean      = sfun_in[i].prior_mean;
		//
		double estimate  = sfun_out[i].estimate;
		double residual  = sfun_out[i].prior_residual;
		double scaled    = sfun_out[i].prior_scaled;
		double check, anorm;
		check   = log( estimate + zeta ) - log( mean + zeta );
		anorm   = fabs(log( estimate + zeta )) + fabs(log( mean + zeta ));
		ok     &= near_equal(residual, check, anorm);
		if( sigma == 0. || sigma == infinity )
			ok = isnan(scaled);
		else
		{	check   = check / sigma;
			anorm   = anorm / sigma;
			ok     &= near_equal(scaled, check, anorm);
		}
	}

	// check sfun_out cohort residuals 
	for(i = 0; i < n_grid; i++)
	{	grid.unpack(j, k, i);
		size_t kp    = k + 1;
		size_t ip;
		bool   ok_kp = grid.pack(j, kp, ip);
		size_t q;
		if( ! ok_kp)
		{	for(q = 0; q < n_stochastic_; q++)
			{	ok &= 
				isnan( sfun_out[i*n_stochastic_ + q].cohort_residual );
				ok &= 
				isnan( sfun_out[i*n_stochastic_ + q].cohort_scaled );
			}
		}	
		else
		{	for(q = 0; q < n_stochastic_; q++)
			{	size_t s_index_k    = i  * n_stochastic_ + q;
				size_t s_index_kp   = ip * n_stochastic_ + q;
				double zeta_k       = sfun_in[s_index_k].cohort_zeta;
				double sigma_k      = sfun_in[s_index_k].cohort_sigma;
				//
				double estimate_k   = sfun_out[s_index_k].estimate;
				double estimate_kp  = sfun_out[s_index_kp].estimate;
				double residual_k   = sfun_out[s_index_k].cohort_residual;
				double scaled_k     = sfun_out[s_index_k].cohort_scaled;
# ifndef NDEBUG
				size_t order_k      = sfun_in[s_index_k].cohort_order;
				assert( order_k == 1 );
# endif
				double check, anorm;
				check  = log(estimate_kp + zeta_k) 
				       - log(estimate_k  + zeta_k);
				anorm  = fabs(log(estimate_kp + zeta_k)) 
				       + fabs(log(estimate_k  + zeta_k));
				ok    &= near_equal(residual_k, check, anorm);
				if( sigma_k == 0. || sigma_k == infinity )
					ok &= isnan( scaled_k );
				else
				{	check   = check / sigma_k;
					anorm   = anorm / sigma_k;
					ok     &= near_equal(scaled_k, check, anorm);
				}
			}
		}
	}

	// check sfun_out age residuals (with the exception of S and C)
	for(i = 0; i < n_grid; i++)
	{	grid.unpack(j, k, i);
		size_t jp    = j+1;
		size_t ip;
		bool   ok_jp = grid.pack(jp, k, ip);
		size_t q;
		if( ! ok_jp)
		{	for(q = 0; q < n_stochastic_; q++)
			{	ok &= 
				isnan( sfun_out[i*n_stochastic_ + q].age_residual );
				ok &= 
				isnan( sfun_out[i*n_stochastic_ + q].age_scaled );
			}
		}	
		else
		{	assert( S_enum == 0 && C_enum == 1 );
			for(q = 2; q < n_stochastic_; q++)
			{	size_t s_index_j    = i  * n_stochastic_ + q;
				size_t s_index_jp   = ip * n_stochastic_ + q;
				double zeta_j       = sfun_in[s_index_j].age_zeta;
				double sigma_j      = sfun_in[s_index_j].age_sigma;
# ifndef NDEBUG
				size_t order_j      = sfun_in[s_index_j].age_order;
				assert( order_j == 1 );
# endif
				//
				double estimate_j   = sfun_out[s_index_j].estimate;
				double estimate_jp  = sfun_out[s_index_jp].estimate;
				double residual_j   = sfun_out[s_index_j].age_residual;
				double scaled_j     = sfun_out[s_index_j].age_scaled;
				double check, anorm;
				check  = log(estimate_jp + zeta_j) 
				       - log(estimate_j  + zeta_j);
				anorm  = fabs(log(estimate_jp + zeta_j)) 
				       + fabs(log(estimate_j  + zeta_j));
				ok    &= near_equal(residual_j, check, anorm);
				if( sigma_j == 0. || sigma_j == infinity )
					ok &= isnan( scaled_j );
				else
				{	check   = check / sigma_j;
					anorm   = anorm / sigma_j;
					ok     &= near_equal(scaled_j, check, anorm);
				}
			}
		}
	}

	// check sfun_out cross residuals 
	for(i = 0; i < n_grid; i++)
	{	size_t i_jpk, i_jkp, i_jpkp;
		grid.unpack(j, k, i);
		bool ok_jpk   = grid.pack(j+1,   k, i_jpk);
		bool ok_jkp   = grid.pack(  j, k+1, i_jkp);
		bool ok_jpkp  = grid.pack(j+1, k+1, i_jpkp);
		size_t q;
		if( ! (ok_jpk & ok_jkp & ok_jpkp) )
		{	for(q = 0; q < n_stochastic_; q++)
			{	ok &= 
				isnan( sfun_out[i*n_stochastic_ + q].cross_residual );
				ok &= 
				isnan( sfun_out[i*n_stochastic_ + q].cross_scaled );
			}
		}	
		else
		{	double root_dc = sqrt(cohort[k+1] - cohort[k]); 
			double root_da = sqrt(age[j+1] - age[j]);
			for(q = 0; q < n_stochastic_; q++)
			{	size_t s_index_jk   = i  * n_stochastic_ + q;
				size_t s_index_jpk  = i_jpk * n_stochastic_ + q;
				size_t s_index_jkp  = i_jkp * n_stochastic_ + q;
				size_t s_index_jpkp = i_jpkp * n_stochastic_ + q;
				double zeta_jk      = sfun_in[s_index_jk].cross_zeta;
				double sigma_jk     = sfun_in[s_index_jk].cross_sigma;
				//
				double estimate_jk  = sfun_out[s_index_jk].estimate;
				double estimate_jpk = sfun_out[s_index_jpk].estimate;
				double estimate_jkp = sfun_out[s_index_jkp].estimate;
				double estimate_jpkp= sfun_out[s_index_jpkp].estimate;
				double residual_jk  = sfun_out[s_index_jk].cross_residual;
				double scaled_jk    = sfun_out[s_index_jk].cross_scaled;
				double num, den, check, norm;
				num   = log(estimate_jpkp + zeta_jk) 
				      - log(estimate_jpk  + zeta_jk)
				      - log(estimate_jkp  + zeta_jk)
				      + log(estimate_jk   + zeta_jk);
				norm  = fabs( log(estimate_jpkp + zeta_jk) )
				      + fabs( log(estimate_jpk  + zeta_jk) )
				      + fabs( log(estimate_jkp  + zeta_jk) )
				      + fabs( log(estimate_jk   + zeta_jk) );
				den   = root_da * root_dc;
				check = num / den;
				norm  = norm / den;
				ok   &= near_equal(residual_jk, check, norm);
				if( sigma_jk == 0. || sigma_jk == infinity )
					ok &= isnan( scaled_jk );
				else
				{	check   = check / sigma_jk;
					norm    = norm  / sigma_jk;
					ok     &= near_equal(scaled_jk, check, norm);
				}
			}
		}
	}

	return ok;
}

bool optimize_xam(void)
{	bool   ok         = true;
	size_t n_age      = 7;
	size_t n_cohort   = 7;

	dismod_pde::vector< dismod_pde::Likelihood_Enum > likelihood(6);
	likelihood[0]   = dismod_pde::gaussian_enum; // even indexed prior residuals
	likelihood[1]   = dismod_pde::gaussian_enum; // odd indexed prior residuals
	likelihood[2]   = dismod_pde::gaussian_enum; // age residuals
	likelihood[3]   = dismod_pde::gaussian_enum; // cohort residuals
	likelihood[4]   = dismod_pde::gaussian_enum; // cross residuals
	likelihood[5]   = dismod_pde::laplace_enum;  // measurements

	ok &= optimize_xam(n_age, n_cohort, likelihood);

	return ok;
}
// END C++
