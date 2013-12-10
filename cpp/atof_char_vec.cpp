# include <cstdlib>
# include <cstring>
# include <dismod_pde/atof_char_vec.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file atof_char_vec.cpp
Implement the routine \ref atof_char_vec().
*/

/*!
Convert a sub-vector of a character vector to double.

\param [in] char_vec
is an vector of characters.
It must hold that <code>char_vec.size() >= start + length</code>.

\param [in] sub_vec
The index <code>start = sub_vec.first</code>
where the sub vector of characters starts.
The value <code>length = sub_vec.second</code>
is the number of characters in the sub-vector.
It must hold that <code>length < 100</code>.

\param [in] line
Line index in the file where the data value occured (one less
than the line number). This is used for error reporting.

\param [in] file
File name where the data occurred. 
This is used for error reporting.

\return
double precision verions of the sub-vector of characters:
\code
	char_vec[start] , char_vec[start+1] , ... , char_vec[start+length-1]
\endcode

\par nan
The characters \c nan are converted to the value <code>dismod_pde::nan()</code>.

\par inf
Note that the characters \c inf are converted to \c DISMOD4_PLUS_INFINITY,
the characters \c -inf are converted to <code>-DISMOD4_PLUS_INFINITY</code>,
and it is an error for any other value to have absolute value greater
than or equal \c DISMOD4_PLUS_INFINITY.

\par Example
The following is an example and test of \c atof_char_vec.
It returns true for success and false for failure.

\include atof_char_vec_tst.cpp
*/
double atof_char_vec(
	const vector<char>&              char_vec , 
	const std::pair<size_t,size_t>&  sub_vec  ,
	size_t                           line     ,
	const std::string&               file     )
{	char buffer[100];

	size_t start  = sub_vec.first;
	size_t length = sub_vec.second;
	assert( length < 100 );
	size_t i;
	for(i = 0; i < length; i++)
		buffer[i] = char_vec[start + i];
	buffer[length] = '\0';

	double r = 0.;
	if( std::strcmp(buffer, "nan") == 0 )
		r = dismod_pde::nan();
	else if( std::strcmp(buffer, "inf") == 0 )
		r = DISMOD4_PLUS_INFINITY;
	else if( std::strcmp(buffer, "-inf") == 0 )
		r = - DISMOD4_PLUS_INFINITY;
	else
	{	r = std::atof(buffer);
		if( fabs(r) >= DISMOD4_PLUS_INFINITY )
		{	std::ostringstream msg;
			msg << std::endl << "atof_char_vec: Error at line " << line + 1;
			msg << " in file " << file << std::endl;
			msg << "an absolute value is greater than or equal ";
			msg << DISMOD4_PLUS_INFINITY << std::endl;
			msg << "and the value is not inf or -inf" << std::endl;
			DISMOD4_ASSERT_MSG(false, msg.str());
		}
	}

	return r;
}


DISMOD4_END_NAMESPACE
