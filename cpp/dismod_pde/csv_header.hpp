# ifndef DISMOD4_CSV_HEADER_HPP
# define DISMOD4_CSV_HEADER_HPP

# include <dismod_pde/define.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file csv_header.hpp
Include file for \ref csv_header().
*/

void csv_header(
	const std::string&               file    ,
	vector<std::string>&             header
);

DISMOD4_END_NAMESPACE
# endif
