# include <dismod_pde/define.hpp>
# include <dismod_pde/sc_ode.hpp>

DISMOD4_BEGIN_NAMESPACE

/*!
\file sc_ode.cpp
Implementaiton of class that defines the ODE for S(*) and C(*) w.r.t age.
*/

/// Constructor
SC_ode::SC_ode(
	/// ode is from age_index to the next valid age index
	size_t                age_index, 
	/// fixed cohort for this ode
	size_t                cohort_index, 
	/// age grid values
	const vector<double>& age,
	/// converts from age-cohort grid pair to a single index
	const pair2index&     grid,
	/// value of the entire state vector
	const ADvector&       state
)  
{
	size_t n_stochastic = n_stochastic_enum;
	//
	size_t grid_index_j;
# ifndef NDEBUG
	bool ok_j        = grid.pack(age_index, cohort_index, grid_index_j);
# else
	grid.pack(age_index, cohort_index, grid_index_j);
# endif
	size_t index_j   = grid_index_j * n_stochastic;
	//
	size_t grid_index_jp;
	size_t jp        = age_index + 1;
# ifndef NDEBUG
	bool ok_jp       = grid.pack(jp, cohort_index, grid_index_jp);
# else
	grid.pack(jp, cohort_index, grid_index_jp);
# endif
	size_t index_jp  = grid_index_jp * n_stochastic;
	//
	assert( ok_j & ok_jp );

	age_j_   = age[age_index];
	age_jp_  = age[jp];
	//
	iota_j_  = state[index_j  + iota_enum];
	iota_jp_ = state[index_jp + iota_enum];
	//
	rho_j_   = state[index_j  + rho_enum];
	rho_jp_  = state[index_jp + rho_enum];
	//
	chi_j_   = state[index_j  + chi_enum];
	chi_jp_  = state[index_jp + chi_enum];
	//
	omega_j_    = state[index_j  + omega_enum];
	omega_jp_   = state[index_jp + omega_enum];
}

// ------------------------------------------------------------------------
/*!
Compute the function \f$ f(a, S, C) = [ S'(a) , C'(a) ] \f$. 

\param [in] a
The value of age at which to evaluate the ODE.

\param [in] sc
A vector of size 2 containing
the value of S(a) and C(a) in that order.

\param [in, out] f
On input is a a vector of size 2 but the value of its elements does not matter.
On return, f[0] is the derivative of S(a) w.r.t age,
and f[1] is te derivative of C(a) w.r.t. age.

\par
Note that the other stochastic functions iota, rho, chi, and omega,
are linearly interpolated with respect to age when evaluating the 
derivatives of S and C.
*/
void SC_ode::Ode(
	const ADdouble &a     ,
	const ADvector &sc    ,
	ADvector       &f     )
{	
# ifndef NDEBUG
	ADdouble epsilon = 1e-10 * (age_jp_ - age_j_); 
	assert( age_j_ <= a + epsilon && a - epsilon <= age_jp_ );
# endif

	ADdouble ratio_j  = (age_jp_ - a) / (age_jp_ - age_j_);
	ADdouble ratio_jp = (a - age_j_)  / (age_jp_ - age_j_);
	//
	ADdouble S        = sc[0];
	ADdouble C        = sc[1];
	ADdouble iota     = ratio_j * iota_j_ + ratio_jp * iota_jp_;  
	ADdouble rho      = ratio_j * rho_j_  + ratio_jp * rho_jp_;  
	ADdouble chi      = ratio_j * chi_j_  + ratio_jp * chi_jp_;  
	ADdouble omega    = ratio_j * omega_j_   + ratio_jp * omega_jp_;  
	//
	ADdouble dS       = rho * C  - (iota + omega) * S;
	ADdouble dC       = iota * S - (rho + omega + chi) * C;
	//
	f[0]              = dS; // S'(a)
	f[1]              = dC; // C'(a)
}


// ------------------------------------------------------------------------
/*!
Compute the function \f$ f_a (a, S, C) = \partial_a f(a, S, C)\f$. 

\param [in] a
The value of age at which to evaluate the age partial of the ODE.

\param [in] sc
A vector of size 2 containing
the value of S(a) and C(a) in that order.

\param [in, out] f_a
On input is a a vector of size 2 but the value of its elements does not matter.
On return, f_a[0] is the partial of S(a) w.r.t age,
and f_a[1] is the partial of C(a) w.r.t. age.

\par
Note that the other stochastic functions iota, rho, chi, and omega,
are linearly interpolated with respect to age when evaluating the 
derivatives of S and C.
*/
void SC_ode::Ode_ind(
	const ADdouble &a     ,
	const ADvector &sc    ,
	ADvector       &f_a   )
{	
# ifndef NDEBUG
	ADdouble epsilon = 1e-10 * (age_jp_ - age_j_); 
	assert( age_j_ <= a + epsilon && a - epsilon <= age_jp_ );
# endif

	ADdouble ratio_j  = (age_jp_ - a) / (age_jp_ - age_j_);
	ADdouble ratio_jp = (a - age_j_)  / (age_jp_ - age_j_);
	//
	ADdouble ratio_j_a  = - 1. / (age_jp_ - age_j_);
	ADdouble ratio_jp_a = + 1. / (age_jp_ - age_j_);
	//
	ADdouble S        = sc[0];
	ADdouble C        = sc[1];
	//
	ADdouble iota_a   = ratio_j_a * iota_j_ + ratio_jp_a * iota_jp_;  
	ADdouble rho_a    = ratio_j_a * rho_j_  + ratio_jp_a * rho_jp_;  
	ADdouble chi_a    = ratio_j_a * chi_j_  + ratio_jp_a * chi_jp_;  
	ADdouble omega_a  = ratio_j_a * omega_j_   + ratio_jp_a * omega_jp_;  
	//
	// ADdouble dS    = rho * C  - (iota + omega) * S;
	// ADdouble dC    = iota * S - (rho + omega + chi) * C;
	ADdouble dS_a     = rho_a  * C  - (iota_a + omega_a) * S;
	ADdouble dC_a     = iota_a * S  - (rho_a + omega_a + chi_a) * C;
	//
	f_a[0]            = dS_a; // partial f_0 (a, S, C) w.r.t a 
	f_a[1]            = dC_a; // partial f_1 (a, S, C) w.r.t a
}


// ------------------------------------------------------------------------
/*!
Compute the function 
\f[
	f_sc (a, S, C) = [ \partial_s f(a, S, C) , \partial_c f(a, S, C) ]
\f] 

\param [in] a
The value of age at which to evaluate the age partial of the ODE.

\param [in] sc
A vector of size 2 containing
the value of S(a) and C(a) in that order.

\param [in, out] f_sc
On input is a a vector of size 4 but the value of its elements does not matter.
On return, f_sc[0] is the partial of S(a) w.r.t S,
f_sc[1] is the partial of S'(a) w.r.t C,
f_sc[2] is the partial of S'(a) w.r.t S,
f_sc[3] is the partial of C'(a) w.r.t S,
f_sc[4] is the partial of C'(a) w.r.t C.

\par
Note that the other stochastic functions iota, rho, chi, and omega,
are linearly interpolated with respect to age when evaluating the 
derivatives of S and C.
*/
void SC_ode::Ode_dep(
	const ADdouble &a     ,
	const ADvector &sc    ,
	ADvector       &f_sc  )
{	
# ifndef NDEBUG
	ADdouble epsilon = 1e-10 * (age_jp_ - age_j_); 
	assert( age_j_ <= a + epsilon && a - epsilon <= age_jp_ );
# endif

	ADdouble ratio_j  = (age_jp_ - a) / (age_jp_ - age_j_);
	ADdouble ratio_jp = (a - age_j_)  / (age_jp_ - age_j_);
	//
	ADdouble S        = sc[0];
	ADdouble C        = sc[1];
	//
	ADdouble iota     = ratio_j * iota_j_ + ratio_jp * iota_jp_;  
	ADdouble rho      = ratio_j * rho_j_  + ratio_jp * rho_jp_;  
	ADdouble chi      = ratio_j * chi_j_  + ratio_jp * chi_jp_;  
	ADdouble omega    = ratio_j * omega_j_   + ratio_jp * omega_jp_;  
	//
	// ADdouble dS    = rho * C  - (iota + omega) * S;
	// ADdouble dC    = iota * S - (rho + omega + chi) * C;
	ADdouble dS_s     = - (iota + omega);
	ADdouble dS_c     = rho;
	ADdouble dC_s     = iota;
	ADdouble dC_c     = - (rho + omega + chi);
	//
	f_sc[0]           = dS_s; // partial f_0 (a, S, C) w.r.t S 
	f_sc[1]           = dS_c; // partial f_0 (a, S, C) w.r.t C 
	f_sc[2]           = dC_s; // partial f_1 (a, S, C) w.r.t S 
	f_sc[3]           = dC_c; // partial f_1 (a, S, C) w.r.t C 
}

DISMOD4_END_NAMESPACE
