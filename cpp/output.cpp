# include <cstring>
# include <dismod_pde/define.hpp>
# include <dismod_pde/output.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file output.cpp
Implement the routine \ref output().
*/

/*!
Output a double precision value to a stream.

\param [in, out] os
is the stream that the value is written to
(using its current precision and other settings).
In the special case where <code>dismod_pde::isnan(value)</code> is true,
the output value is \c "nan".
In the special cases plus and minus 
\c DISMOD4_PLUS_INFINITY
are output as the strings \c "inf" and \c "-inf" respectively.

\param [in] value
is the value to be output.
*/
void output(std::ostream& os, double value)
{
	if( isnan(value) )
		os << "nan";
	else if( value == DISMOD4_PLUS_INFINITY )
		os << "inf";
	else if( value == - DISMOD4_PLUS_INFINITY )
		os << "-inf";
	else	os << value;
}
DISMOD4_END_NAMESPACE
