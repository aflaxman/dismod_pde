# include <cmath>
# include <dismod_pde/double2stream.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file double2stream.cpp
Implements the routine \ref double2stream()
*/

/*!
Choose between fixed and scientific notaiton to get maximum output precision.

\param [in,out] os
Stream that will be written to.

\param [in] width
Number of characters that the output must fit in
(must be greater than two).

\param [in] value
Value that is being output.
*/

void double2stream(
	std::ostream& os      , 
	size_t        width   , 
	double        value   ) 
{
	using std::ios_base;
	assert( width > 2 );

	std::streamsize      old_precision = os.precision();
	std::streamsize      old_width     = os.width();
	ios_base::fmtflags   old_flags     = os.flags();
	// 
	double log_value = std::log10(value);
	bool fixed       = false;
	size_t precision = 0;
	if( fabs(value) < 1e-99 )
	{	fixed     = true;
		precision = 0;
	}
	else if( width <= 7 )
	{	fixed     = true;
		if( log_value < 0. )
			precision = width - 2;
		else	if( log_value < width - 2 )
			precision = size_t( width - log_value - 2 );
		else	precision = 0;
	}
	else if( 0. <= log_value && 2. + log_value < width )
	{	fixed     = true;
		precision = size_t(width - log_value - 2.);
	}
	else if( 6. - log_value < width )
	{	fixed     = true;
		precision = width - 3;
	}
	else	precision = width - 7;
	os.precision(precision);
	os.width(width);
	if( fixed )
		os.setf(old_flags | ios_base::fixed | ios_base::right);
	else	os.setf(old_flags | ios_base::scientific | ios_base::right);
	os << value;
	//
	os.precision(old_precision);
	os.width(old_width);
	os.flags(old_flags);
}

DISMOD4_END_NAMESPACE
