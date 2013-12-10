# include <dismod_pde/fg_info.hpp>
# include <dismod_pde/sc_ode.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file fg_info_age.cpp
Implementation of the \ref FG_info member functions
\ref FG_info::age_residual() and \ref FG_info::age_scaled().
*/

// ------------------------------------------------------------------------
/*!
Compute the residual in age direction for all the stochastic functions.

\return
residuals for all the stochastic functions at the specified grid point. 
If order is 2 for a residual component, and (age_index+2,cohort_index) is not
a valid arg-cohort pair, the corresponding component of the return
vector is plus infinity (it is assumed that the corresponding component
is not used).

\param [in] age_index
index that determines age at this grid point.
Both this value and <code>age_index + 1</code> must be valid
indices grid point indices for this \c cohort_index.

\param [in] cohort_index
index that determines cohort  at this grid point

\param [in] state
argument to the residual function

\par Assumptions
It is assumed (and asserted) that (age_index+1,cohort_index) is a valid 
age-cohort pair. 
*/
vector< AD<double> > FG_info::age_residual(
	size_t           age_index         , 
	size_t           cohort_index      , 
	const ADvector   &state            ) const
{
	size_t k         = cohort_index;
	size_t j         = age_index;
	size_t jp        = j + 1;
	size_t jpp       = jp + 1;
	//
	size_t g_index_j, g_index_jp, g_index_jpp;
# ifndef NDEBUG
	bool ok_j        = grid_.pack(j,   k, g_index_j);
# else
	grid_.pack(j,   k, g_index_j);
# endif
# ifndef NDEBUG
	bool ok_jp       = grid_.pack(jp,  k, g_index_jp);
# else
	grid_.pack(jp,  k, g_index_jp);
# endif
	bool ok_jpp      = grid_.pack(jpp, k, g_index_jpp);
	assert( ok_j & ok_jp );
	//
	size_t s_index_j   = g_index_j   * n_stochastic_;
	size_t s_index_jp  = g_index_jp  * n_stochastic_;
	size_t s_index_jpp = g_index_jpp * n_stochastic_;
	//
	double da_j        = age_[jp] - age_[j];
	double  da_jp      = 0.;
	if( ok_jpp )
		da_jp         = age_[jpp] - age_[jp]; 
	//
	// Determine ODE solution for S( age[jp] ) and C( age[jp] )
	ADdouble S_j     = state[s_index_j  + S_enum];
	ADdouble S_jp    = state[s_index_jp + S_enum];
	//
	ADdouble C_j     = state[s_index_j  + C_enum];
	ADdouble C_jp    = state[s_index_jp + C_enum];
	//
	SC_ode sc_ode(j, k, age_, grid_, state);
	double   ai      = age_[j];
	double   af      = age_[jp];
	ADdouble ti      = ai;
	ADdouble tf      = af;
	size_t M         = size_t( (af - ai) / parameter_.max_ode_step + 1. );
	ADvector xi(2);
	xi[0]            = S_j;
	xi[1]            = C_j; 
	ADvector xf      = CppAD::Rosen34(sc_ode, M, ti, tf, xi);
	//
	ADvector residual(n_stochastic_);
	size_t q;
	vector<ADdouble> ode_sol(2);
	const char* describe = "vec = Approximate ODE solution for next (S, C)";
	for(q = 0; q < n_stochastic_; q++)
	{
		size_t order_j  = sfun_in_[s_index_j + q].age_order;
		double zeta_j   = sfun_in_[s_index_j + q].age_zeta;
		ADdouble df_j   = 0.;
		switch( Stochastic_Enum(q) )
		{
			case S_enum:
			assert( order_j == 1 );
			ode_sol[0]  = xf[0] + zeta_j;
			df_j        = log(S_jp + zeta_j) - log(xf[0] + zeta_j);
			residual[q] = df_j;
			break;

			case C_enum:
			assert( order_j == 1 );
			ode_sol[1]  = xf[1] + zeta_j;
 			df_j        = log(C_jp + zeta_j) - log(xf[1] + zeta_j);
			residual[q] = df_j;
			break;

			case iota_enum:
			case rho_enum:
			case chi_enum:
			case omega_enum:
			assert( (order_j == 1) | (order_j == 2 ) );
			df_j = log( state[s_index_jp + q] + zeta_j )
			     - log( state[s_index_j  + q] + zeta_j );
			//
			if( order_j == 1 )
				residual[q] = df_j;
			//
			else if ( ! ok_jpp )
				residual[q] = infinity_; // value should not be used
			else
			{
				ADdouble df_jp  = log( state[s_index_jpp + q] + zeta_j )
				                - log( state[s_index_jp  + q] + zeta_j );
				ADdouble ddf_j  = df_jp / da_jp - df_j / da_j;
				residual[q]     = ddf_j;
			}
			break;

			default:
			assert(false);
		}
	}
	if( parameter_.debug_output )
	{	check_pos(describe, ode_sol, age_index, cohort_index, state);
		check_pos(describe, ode_sol, jp, cohort_index, state);
	}

	return residual;
}

// ------------------------------------------------------------------------
/*!
Compute the scaled residual in age direction for all the stochastic functions
at one grid point.

\return
Residual divided by standard deviation.
In the special case where the standard devaition is zero or infinity,
the return value is the residual.

\param [in] age_index
index that determines age at this grid point.

\param [in] cohort_index
index that determines cohort at this grid point.

\param [in] state
argument to the residual function

\par
This index pair must be valid.
In addition, if age_index is not zero,
the pair <code>(age_index + 1, cohort_index)</code>
must also be valid.
*/
vector< AD<double> > FG_info::age_scaled(
	size_t           age_index         , 
	size_t           cohort_index      , 
	const ADvector   &state            ) const
{
	size_t i;
# ifndef NDEBUG
	bool ok = grid_.pack(age_index, cohort_index, i);
# else
	grid_.pack(age_index, cohort_index, i);
# endif
	assert( ok );
	i *= n_stochastic_;

	ADvector res(n_stochastic_);
	res     = age_residual(age_index, cohort_index, state);
	size_t q;
	for(q = 0; q < n_stochastic_; q++)
	{	double sigma = sfun_in_[i + q].age_sigma;
		if( (sigma != 0.) & (sigma != infinity_) )
			res[q] = res[q] / sigma;
		DISMOD4_ASSERT_MSG(
			! CppAD::isnan(res[q]),
			"fg_info::an age_scaled residual is not a number"
		);
	}

	return res;
}

DISMOD4_END_NAMESPACE
