# ifndef DISMOD4_SC_ODE_HPP
# define DISMOD4_SC_ODE_HPP

# include <dismod_pde/define.hpp>
# include <dismod_pde/pair2index.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file sc_ode.hpp
Include file for class that defines the ODE for S(*) and C(*) w.r.t age.
*/

/*!
Class that defines the ODE for S(*) and C(*) w.r.t age.
*/
class SC_ode {
	typedef AD<double>             ADdouble;
	typedef vector< AD<double> >   ADvector;
private:
	/// Value of age at beginning of interval
	/// (effectively const because only set in constructor)
	ADdouble          age_j_;
	/// Value of age at end of interval
	/// (effectively const because only set in constructor)
	ADdouble          age_jp_;

	/// Value of iota at beginning of interval
	/// (effectively const because only set in constructor)
	ADdouble          iota_j_;
	/// Value of iota at end of interval
	/// (effectively const because only set in constructor)
	ADdouble          iota_jp_;

	/// Value of rho at beginning of interval
	/// (effectively const because only set in constructor)
	ADdouble          rho_j_;
	/// Value of rho at end of interval
	/// (effectively const because only set in constructor)
	ADdouble          rho_jp_;

	/// Value of chi at beginning of interval
	/// (effectively const because only set in constructor)
	ADdouble          chi_j_;
	/// Value of chi at end of interval
	/// (effectively const because only set in constructor)
	ADdouble          chi_jp_;

	/// Value of omega at beginning of interval
	/// (effectively const because only set in constructor)
	ADdouble          omega_j_;
	/// Value of omega at end of interval
	/// (effectively const because only set in constructor)
	ADdouble          omega_jp_;

public:
	/// constructor
	SC_ode(
		size_t                age_index            , 
		size_t                cohort_index         , 
		const vector<double>& age                  ,
		const pair2index&     grid                 ,
		const ADvector&       state
	);
	void Ode(
		const ADdouble &a     ,
		const ADvector &sc    ,
		ADvector       &sc_a
	);
	void Ode_ind(
		const ADdouble &a     ,
		const ADvector &sc    ,
		ADvector       &f_a
	);
	void Ode_dep(
		const ADdouble &a     ,
		const ADvector &sc    ,
		ADvector       &f_sc
	);
};

DISMOD4_END_NAMESPACE
# endif
