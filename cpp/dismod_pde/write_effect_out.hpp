# ifndef DISMOD4_WRITE_COVARIATE_OUT_HPP
# define DISMOD4_WRITE_COVARIATE_OUT_HPP

# include <dismod_pde/define.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file write_effect_out.hpp
Include file for \ref write_effect_out() routine.
*/
void write_effect_out(
	const std::string&                 file           ,
	const vector<std::string>&         covariate_name ,
	const vector<Effect_Input_Struct>& effect_in      ,
	const vector<double>&              estimate 
);

DISMOD4_END_NAMESPACE
# endif
