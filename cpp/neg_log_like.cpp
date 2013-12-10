# include <dismod_pde/neg_log_like.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
file neg_log_like.cpp
Implementation of the \ref neg_log_like() routine.
*/

/*!
Evaluate the negative log-likelihood corresponding to a single residual.

\param [in] scaled
scaled residual 

\param [in] likelihood
which variance one likelihood function to use.

\return
Value of the term in the negative log likelihood that depends on the 
residual value.
The normalizing term in the likelihood,
which is a constant added to the negative log likelihood,
is not included.
*/
double neg_log_like(
	double scaled              ,
	Likelihood_Enum likelihood )
{	
	double ret  = 0.;
	switch( likelihood )
	{	
		case gaussian_enum:
		ret = scaled * scaled / 2.;
		break;

		case laplace_enum:
		ret = std::sqrt(2.) * std::fabs(scaled);
		break;

		default:
		assert(false);
	}
	return ret;
}
DISMOD4_END_NAMESPACE
