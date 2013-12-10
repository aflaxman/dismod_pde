# ifndef DISMOD4_DOUBLE2STREAM_HPP
# define DISMOD4_DOUBLE2STREAM_HPP

# include <dismod_pde/define.hpp>

DISMOD4_BEGIN_NAMESPACE

void double2stream(
	std::ostream&   os     ,
	size_t          width  ,
	double          value
);

DISMOD4_END_NAMESPACE
# endif
