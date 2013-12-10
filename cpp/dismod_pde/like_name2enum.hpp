# ifndef DISMOD4_LIKE_NAME2ENUM_HPP
# define DISMOD4_LIKE_NAME2ENUM_HPP

# include <dismod_pde/define.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file like_name2enum.hpp
Include file for \ref like_name2enum() routine.
*/
Likelihood_Enum like_name2enum(
	const std::string& name  , 
	size_t             line  ,
	const std::string& file  
);
DISMOD4_END_NAMESPACE
# endif
