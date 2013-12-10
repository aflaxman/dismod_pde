# ifndef DISMOD4_READ_COVARIATE_IN_HPP
# define DISMOD4_READ_COVARIATE_IN_HPP

# include <dismod_pde/define.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file read_effect_in.hpp
Include file for \ref read_effect_in().
*/

void read_effect_in(
	const std::string&           file             ,
	const vector<std::string>&   covariate_name   ,
	vector<Effect_Input_Struct>& effect_in
);

DISMOD4_END_NAMESPACE
# endif
