# include <dismod_pde/like_name2enum.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file like_name2enum.cpp
Implementation of the \ref like_name2enum() routine.
*/
/*!
Routine that maps a likelihood name to the corresponding enum value.

\param [in] name
name for this likelihood.

\param [in] line
line number where this likelihood name occurred (used for error reporting).

\param [in]  file
file where this likelihood name occured (used for error reporting).

\return
Likelihood_Enum value corresponding to \c name.
*/

Likelihood_Enum like_name2enum(
	const std::string& name  , 
	size_t             line  ,
	const std::string& file  )
{	std::ostringstream msg;
	using std::endl;

	if( name == "gaussian" )
		return dismod_pde::gaussian_enum;
	else if( name == "laplace" )
		return dismod_pde::laplace_enum;
	else
	{	msg << endl;
		msg << "dismod_pde_csv: error at line " << line;
		msg << " in file " << file << endl;
		msg << name << " is not a valid likelihood name" << endl;
		DISMOD4_ASSERT_MSG(false, msg.str());
	}
	// will never get here but avoid compiler warnings
	return dismod_pde::gaussian_enum;
}

DISMOD4_END_NAMESPACE
