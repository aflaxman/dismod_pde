# ifndef DISMOD4_WRITE_MEASURE_OUT_HPP
# define DISMOD4_WRITE_MEASURE_OUT_HPP

# include <dismod_pde/define.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file write_measure_out.hpp
Include file for \ref write_measure_out() routine.
*/
void write_measure_out(
	const std::string&                                     file_in     , 
	const std::string&                                     file_out    , 
	const dismod_pde::vector<dismod_pde::Measure_Input_Struct>&  measure_in  ,
	const dismod_pde::vector<dismod_pde::Measure_Output_Struct>& measure_out 
);

DISMOD4_END_NAMESPACE
# endif
