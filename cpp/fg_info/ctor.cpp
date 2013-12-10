# include <dismod_pde/define.hpp>
# include <dismod_pde/fg_info.hpp>
# include <sstream>

DISMOD4_BEGIN_NAMESPACE
/*!
\file fg_info_ctor.cpp
Implementaiton of constructor for \ref FG_info. 
*/

// ------------------------------------------------------------------------
/*!
Pass optimize input though to constructor. 

All of the arguments in this constructor as used as references and hence
must have at least as long a life time (not be destructed) before the 
\c FG_info object that is being constructed.

\param [in] parameter 
parameter structure as passed to optimize

\param [in] age
values for age on the grid

\param [in] cohort
values for cohort on the grid

\param [in] grid
converts from age-cohort indices to linear vector indices.

\param [in] covariate_value
covariate values as passed to optimize.

\param [in] sfun_in
information about optimization problem that is define at each grid point,
see \ref Sfun_Input_Struct.

\param [in] measure_in
measure information as passed to optimize

\param [in] effect_in
measurement effect information as passed to optimize
*/
FG_info::FG_info(
	const Optimize_Parameter_Struct&      parameter       ,
	const vector<double>&                 age             ,
	const vector<double>&                 cohort          ,
	const pair2index&                     grid            ,
	const vector<double>&                 covariate_value ,
	const vector<Sfun_Input_Struct>&      sfun_in         ,
	const vector<Measure_Input_Struct>&   measure_in      ,
	const vector<Effect_Input_Struct>&    effect_in       )
:
parameter_(parameter)                                            ,
age_(age)                                                        ,
cohort_(cohort)                                                  ,
grid_(grid)                                                      ,
covariate_value_(covariate_value)                                ,
sfun_in_(sfun_in)                                                ,
measure_in_(measure_in)                                          ,
effect_in_(effect_in)                                            ,
n_stochastic_(n_stochastic_enum)                                 ,
n_effect_( effect_in.size() )                                    , 
infinity_(DISMOD4_PLUS_INFINITY)
{	using std::endl;

	// some temporaries
	size_t i, j, k, ell, q, i_grid, i_grid_p;

	// number of measurements
	size_t n_measure    = measure_in.size();
	// number of grid points
	size_t n_grid       = grid.size();
	// number of components in the state vector
	size_t n_state      = n_grid * n_stochastic_;
	// number of covariates
	if( n_measure == 0 )
		n_covariate_ = 0;
	else	n_covariate_ = covariate_value.size() / n_measure;

	// check some input values
	assert( n_state == sfun_in.size() );
	assert( covariate_value.size() == n_measure * n_covariate_ );

	// -------------------------------------------------------------------
	// Determine the number of constraints and auxillary variables
	// -------------------------------------------------------------------
	n_equality_   = 0;
	n_inequality_ = 0;
	n_auxillary_  = 0;
	//
	double sigma         = 0.;
	bool   valid         = false;
	for(i_grid = 0; i_grid < n_grid; i_grid++)
	{	grid.unpack(j, k, i_grid);
		size_t jp = j+1;
		// first direct prior residuals for this grid point
		for(q = 0; q < n_stochastic_; q++)
		{	i     = i_grid * n_stochastic_ + q;
			sigma = sfun_in[i].prior_sigma;
			if( sigma == infinity_ )
				;
			else if( sigma == 0. )
				n_equality_ += 1;
			else	switch( sfun_in[i].prior_like )
			{
				case gaussian_enum:
				n_inequality_ += 0;
				n_auxillary_  += 0;
				break;

				case laplace_enum:
				n_inequality_ += 2;
				n_auxillary_  += 1;
				break;

				default:
				assert(false);
			}
		}
		valid = grid.pack(jp, k, i_grid_p);
		if( valid )
		{	// second comes age residuals for this grid point
			for(q = 0; q < n_stochastic_; q++)
			{	i     = i_grid * n_stochastic_ + q;
				sigma = sfun_in[i].age_sigma;
				if( sfun_in[i].age_order == 2 )
					valid = grid.pack(jp+1, k, i_grid_p);
				else	valid = true;

				if( ! valid | (sigma == infinity_) )
					;
				else if( sigma == 0. )
					n_equality_ += 1;
				else	switch( sfun_in[i].age_like )
				{	case gaussian_enum:
					n_inequality_ += 0;
					n_auxillary_  += 0;
					break;

					case laplace_enum:
					n_inequality_ += 2;
					n_auxillary_  += 1;
					break;

					default:
					assert(false);
				}
			}
		}
		valid = grid.pack(j, k+1, i_grid_p);
		if( valid )
		{	// third comes cohort residuals for this grid point
			for(q = 0; q < n_stochastic_; q++)
			{	i     = i_grid * n_stochastic_ + q;
				sigma = sfun_in[i].cohort_sigma;
				if( sfun_in[i].cohort_order == 2 )
					valid = grid.pack(j, k+2, i_grid_p);
				else	valid = true;
					
				if( ! valid | (sigma == infinity_) )
					;
				else if( sigma == 0. )
					n_equality_ += 1;
				else	switch( sfun_in[i].cohort_like )
				{	case gaussian_enum:
					n_inequality_ += 0;
					n_auxillary_  += 0;
					break;

					case laplace_enum:
					n_inequality_ += 2;
					n_auxillary_  += 1;
					break;

					default:
					assert(false);
				}
			}
		}
		valid  = true;
		valid &= grid.pack(j,   k+1,  i_grid_p);
		valid &= grid.pack(j+1,   k,  i_grid_p);
		valid &= grid.pack(j+1, k+1,  i_grid_p);
		if( valid )
		{	// fourth comes cross residuals for this grid point
			for(q = 0; q < n_stochastic_; q++)
			{	i     = i_grid * n_stochastic_ + q;
				sigma = sfun_in[i].cross_sigma;

				if( ! valid | (sigma == infinity_) )
					;
				else if( sigma == 0. )
				{	n_equality_ += 1;
				}
				else	switch( sfun_in[i].cross_like )
				{	case gaussian_enum:
					n_inequality_ += 0;
					n_auxillary_  += 0;
					break;

					case laplace_enum:
					n_inequality_ += 2;
					n_auxillary_  += 1;
					break;

					default:
					assert(false);
				}
			}
		}
	} // end loop over grid residuals

	// last come the measurements residuals
	for(ell = 0; ell < n_measure; ell++)
	{
		if( measure_in[ell].meas_sigma == infinity_ )
			;
		else if( measure_in[ell].meas_sigma == 0. )
			n_equality_ += 1;
		else	switch( measure_in[ell].meas_like )
		{
			case gaussian_enum:
			n_inequality_ += 0;
			n_auxillary_  += 0;
			break;

			case laplace_enum:
			n_inequality_ += 2;
			n_auxillary_  += 1;
			break;

			default:
			assert(false);
		}
	}
	// -------------------------------------------------------------------
	// Set x_lower_ and x_upper to their final values.
	// -------------------------------------------------------------------
	x_lower_.resize(n_state + n_auxillary_ + n_effect_);
	x_upper_.resize(n_state + n_auxillary_ + n_effect_);
	for(i = 0; i < n_state; i++)
	{	double zeta = sfun_in[i].optimize_zeta;
# ifndef NDEBUG
		if( zeta + sfun_in[i].optimize_lower <= 0. )
		{	using std::endl;
			std::ostringstream msg;
			i_grid = i / n_stochastic_;	
			q      = i % n_stochastic_;
			grid.unpack(j, k, i_grid);
			msg << endl;
			msg << "optimize: optimize_zeta + optimize_lower <= 0 "<< endl;
			msg << "sfun = " <<  stochastic_enum.name()[q];
			msg << ", age = " << age[j];
			msg << ", cohort = " << cohort[k];
			msg << ", optimize_zeta = " << zeta;
			msg << ", optimize_lower = " << sfun_in[i].optimize_lower;
			msg << endl;
			DISMOD4_ASSERT_MSG(false, msg.str());
		}
# endif
# if DISMOD4_RESCALE_SFUN_OPT
		x_lower_[i] = log( zeta + sfun_in[i].optimize_lower );
		if( sfun_in[i].optimize_upper == infinity_ )
			x_upper_[i] = infinity_;
		else	x_upper_[i] = log( zeta + sfun_in[i].optimize_upper );
# else
		x_lower_[i] = sfun_in[i].optimize_lower;
		x_upper_[i] = sfun_in[i].optimize_upper;
# endif
		if( x_upper_[i] < x_upper_[i] )
		{	using std::endl;
			std::ostringstream msg;
			i_grid = i / n_stochastic_;	
			q      = i % n_stochastic_;
			grid.unpack(j, k, i_grid);
			msg << endl;
			msg << "optimize: upper < lower "<< endl;
			msg << "sfun = " <<  stochastic_enum.name()[q];
			msg << ", age = " << age[j];
			msg << ", cohort = " << cohort[k];
			msg << endl;
			DISMOD4_ASSERT_MSG(false, msg.str());
		}
	}
	for(i = 0; i < n_auxillary_; i++)
	{	x_lower_[n_state + i] = -infinity_;
		x_upper_[n_state + i] = +infinity_;
	}
	for(i = 0; i < n_effect_; i++)
	{	x_lower_[n_state + n_auxillary_ + i] = effect_in[i].lower;
		x_upper_[n_state + n_auxillary_ + i] = effect_in[i].upper;
		bool ok =  effect_in[i].lower   <=  effect_in[i].initial;
		ok     &=  effect_in[i].initial <=  effect_in[i].upper;
		if( ! ok )
		{	std::ostringstream msg;
			msg << endl;
			msg << "i = " << i << endl;
			msg << "effect_in[i].lower   > effect_in[i].initial or " << endl;
			msg << "effect_in[i].initial > effect_in[i].upper" << endl;
			msg << endl;
			DISMOD4_ASSERT_MSG(false, msg.str())
		}
	}
	// -------------------------------------------------------------------
	// Set g_lower_ and g_upper to their final values.
	// -------------------------------------------------------------------
	g_lower_.resize(n_equality_ + n_inequality_);
	g_upper_.resize(n_equality_ + n_inequality_);
	size_t  g_index = 0;
	for(i_grid = 0; i_grid < n_grid; i_grid++)
	{	grid.unpack(j, k, i_grid);
		size_t jp = j+1;
		// first direct prior residuals for this grid point
		for(q = 0; q < n_stochastic_; q++)
		{	i     = i_grid * n_stochastic_ + q;
			sigma = sfun_in[i].prior_sigma;
			if( sigma == infinity_ )
				;
			else if( sigma == 0. )
			{	g_lower_[g_index] = 0.;
				g_upper_[g_index] = 0.;
				g_index++;
			}
			else	switch( sfun_in[i].prior_like )
			{	case gaussian_enum:
				break;

				case laplace_enum:
				g_lower_[g_index] = 0.;
				g_upper_[g_index] = infinity_;
				g_index++;
				g_lower_[g_index] = 0.;
				g_upper_[g_index] = infinity_;
				g_index++;
				break;

				default:
				assert(false);
			}
		}
		valid = grid.pack(jp, k, i_grid_p);
		if( valid )
		{	// second comes age residuals for this grid point
			for(q = 0; q < n_stochastic_; q++)
			{	i     = i_grid * n_stochastic_ + q;
				sigma = sfun_in[i].age_sigma;
				if( sfun_in[i].age_order == 2 )
					valid = grid.pack(jp+1, k, i_grid_p);
				else	valid = true;

				if( ! valid | (sigma == infinity_) )
					;
				else if( sigma == 0. )
				{	g_lower_[g_index] = 0.;
					g_upper_[g_index] = 0.;
					g_index++;
				}
				else	switch( sfun_in[i].age_like )
				{	case gaussian_enum:
					break;

					case laplace_enum:
					g_lower_[g_index] = 0.;
					g_upper_[g_index] = infinity_;
					g_index++;
					g_lower_[g_index] = 0.;
					g_upper_[g_index] = infinity_;
					g_index++;
					break;

					default:
					assert(false);
				}
			}
		}
		valid = grid.pack(j, k+1, i_grid_p);
		if( valid )
		{	// third comes cohort residuals for this grid point
			for(q = 0; q < n_stochastic_; q++)
			{	i     = i_grid * n_stochastic_ + q;
				sigma = sfun_in[i].cohort_sigma;
				if( sfun_in[i].cohort_order == 2 )
					valid = grid.pack(j, k+2, i_grid_p);
				else	valid = true;

				if( ! valid | (sigma == infinity_) )
					;
				else if( sigma == 0. )
				{	g_lower_[g_index] = 0.;
					g_upper_[g_index] = 0.;
					g_index++;
				}
				else switch( sfun_in[i].cohort_like )
				{	case gaussian_enum:
					break;

					case laplace_enum:
					g_lower_[g_index] = 0.;
					g_upper_[g_index] = infinity_;
					g_index++;
					g_lower_[g_index] = 0.;
					g_upper_[g_index] = infinity_;
					g_index++;
					break;

					default:
					assert(false);
				}
			}
		}
		valid  = true;
		valid &= grid.pack(j,   k+1,  i_grid_p);
		valid &= grid.pack(j+1,   k,  i_grid_p);
		valid &= grid.pack(j+1, k+1,  i_grid_p);
		if( valid )
		{	// fourth comes cross residuals for this grid point
			for(q = 0; q < n_stochastic_; q++)
			{	i     = i_grid * n_stochastic_ + q;
				sigma = sfun_in[i].cross_sigma;

				if( ! valid | (sigma == infinity_) )
					;
				else if( sigma == 0. )
				{	g_lower_[g_index] = 0.;
					g_upper_[g_index] = 0.;
					g_index++;
				}
				else switch( sfun_in[i].cross_like )
				{	case gaussian_enum:
					break;

					case laplace_enum:
					g_lower_[g_index] = 0.;
					g_upper_[g_index] = infinity_;
					g_index++;
					g_lower_[g_index] = 0.;
					g_upper_[g_index] = infinity_;
					g_index++;
					break;

					default:
					assert(false);
				}

			}
		}
	} // loop over grid residuals
	// last come the measurement residuals
	for(ell = 0; ell < n_measure; ell++)
	{	if( measure_in[ell].meas_sigma == infinity_ )
			;
		else if( measure_in[ell].meas_sigma == 0. )
		{	g_lower_[g_index] = 0.;
			g_upper_[g_index] = 0.;
			g_index++;
		}
		else	switch( measure_in[ell].meas_like )
		{
			case gaussian_enum:
			break;

			case laplace_enum:
			g_lower_[g_index] = 0.;
			g_upper_[g_index] = infinity_;
			g_index++;
			g_lower_[g_index] = 0.;
			g_upper_[g_index] = infinity_;
			g_index++;
			break;

			default:
			assert(false);
		}
	}
	// -------------------------------------------------------------------
	// Cache midpoint information to speed up measurement model evaluation
	// -------------------------------------------------------------------
	double a_ell, t_ell;
	size_t kj, kjp;
	age_midpoint_index_.resize(n_measure);
	cohort_midpoint_index_first.resize(n_measure);
	cohort_midpoint_index_second.resize(n_measure);
	midpoint_weight_.resize(4 * n_measure);
	for(ell = 0; ell < n_measure; ell++)
	{	// midpoint age value
		a_ell = (measure_in[ell].age_lower + measure_in[ell].age_upper)/2.; 
		// midpoint time value 
		t_ell = (measure_in[ell].time_lower + measure_in[ell].time_upper)/2.; 

		// determine age index just before a_ell (or zero)
		j = age.size() - 2;
		while( 0 < j && a_ell < age[j] )
			--j;

		// determine the cohort index corresponding to j
		kj = cohort.size() - 2;
		while( 0 < kj && t_ell < age[j] + cohort[kj] )
			kj--;
		if( 0 < kj && ! grid.pack(j, kj+1, i) )
			kj--;
	
		// determine the cohort index corresponding to j+1
		kjp = cohort.size() - 2;
		while( 0 < kjp && t_ell < age[j+1] + cohort[kjp] )
			kjp--;
		if( 0 < kjp && ! grid.pack(j+1, kjp+1, i) )
			kjp--;

		// make sure time limits in pair2index constructor are large enough
# ifndef NDEBUG
		bool ok = true;
		ok     &=    age[j] <= a_ell && a_ell <= age[j+1]; 
		ok     &= cohort[kj]    + age[j]   <= t_ell; 
		ok     &= cohort[kj+1]  + age[j]   >= t_ell; 
		ok     &= cohort[kjp]   + age[j+1] <= t_ell; 
		ok     &= cohort[kjp+1] + age[j+1] >= t_ell; 
		ok     &= grid.pack(j, kj,   i);
		ok     &= grid.pack(j, kj+1, i);
		ok     &= grid.pack(j+1, kjp, i);
		ok     &= grid.pack(j+1, kjp+1, i);
		if( ! ok )
		{	std::ostringstream msg;
			msg << endl;
			msg << "measure_in[" << ell << "]: ";
			msg << "age = " << a_ell << ", "; 
			msg << "time = " << t_ell << ", "; 
			msg << "cohort = " << a_ell + t_ell << endl;
			//
			msg << "grid.time_lower()= " << grid.time_lower() << ", ";
			msg << "grid.time_upper()= " << grid.time_upper() << endl;
			//
			bool valid  = grid.pack(j, kj, i);
			msg << "age["    << j   << "] = " << age[j]        << ", ";
			msg << "cohort[" << kj  << "] = " << cohort[kj]    << ",";
			msg << "time = " << age[j] + cohort[kj]            << ",";
			msg << "valid = " << valid                         << endl;
			//
			valid  = grid.pack(j, kj+1, i);
			msg << "age["    << j    << "] = " << age[j]       << ", ";
			msg << "cohort[" << kj+1 << "] = " << cohort[kj+1] << ",";
			msg << "time = " << age[j] + cohort[kj+1]          << ",";
			msg << "valid = " << valid                         << endl;
			//
			valid  = grid.pack(j+1, kjp, i);
			msg << "age["    << j+1  << "] = " << age[j+1]     << ", ";
			msg << "cohort[" << kjp  << "] = " << cohort[kjp]  << ",";
			msg << "time = " << age[j+1] + cohort[kjp]         << ",";
			msg << "valid = " << valid                         << endl;
			//
			valid  = grid.pack(j+1, kjp+1, i);
			msg << "age["    << j+1   << "] = " << age[j+1]       << ", ";
			msg << "cohort[" << kjp+1 << "] = " << cohort[kjp+1]  << ",";
			msg << "time = " << age[j+1] + cohort[kjp+1]          << ",";
			msg << "valid = " << valid                            << endl;
			//
			DISMOD4_ASSERT_MSG(ok, msg.str());
		}
# endif
		
		
		// set final indices
		age_midpoint_index_[ell]          = j;
		cohort_midpoint_index_first[ell]  = kj;
		cohort_midpoint_index_second[ell] = kjp;

		// first weight is for (j, kj)
		double den = (age[j+1] - age[j])*(cohort[kj+1] - cohort[kj]);
		midpoint_weight_[4 * ell + 0] =
			(age[j+1] - a_ell) * (age[j] + cohort[kj+1] - t_ell) / den;

		// second weight is for (j, kj+1)
		midpoint_weight_[4 * ell + 1] =
			(age[j+1] - a_ell) * (t_ell - age[j] - cohort[kj]) / den;
		
		// third weight is for (j+1, kjp)
		den = (age[j+1] - age[j])*(cohort[kjp+1] - cohort[kjp]);
		midpoint_weight_[4 * ell + 2] =
			(a_ell - age[j]) * (age[j+1] + cohort[kjp+1] - t_ell) / den;
		
		// fourth weight is for (j+1, kjp+1)
		midpoint_weight_[4 * ell + 3] =
			(a_ell - age[j]) * (t_ell - age[j+1] - cohort[kjp]) / den;
	}
# ifndef NDEBUG
	for(i = 0; i < 4 * n_measure; i++)
		assert( midpoint_weight_[i] >= 0. );
# endif
}
DISMOD4_END_NAMESPACE
