# ifndef DISMOD4_READ_MEASURE_IN_HPP
# define DISMOD4_READ_MEASURE_IN_HPP

# include <dismod_pde/define.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file read_measure_in.hpp
Include file for \ref read_measure_in().
*/

void read_measure_in(
	const std::string&                     file                 ,
	vector<std::string>&                   covariate_name       ,
	vector<dismod_pde::Measure_Input_Struct>& measure_in           ,
	vector<double>&                        measured_covariate
);

DISMOD4_END_NAMESPACE
# endif
