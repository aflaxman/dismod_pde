# include <string>
# include <dismod_pde/str_char_vec.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file str_char_vec.cpp
Implement the routine \ref str_char_vec().
*/

/*!
Convert a sub-vector of a character vector to a standard string.

\param [in] char_vec
is an vector of characters.
It must hold that <code>char_vec.size() >= start + length</code>.

\param [in] sub_vec
The index <code>start = sub_vec.first</code>
where the sub vector of characters starts.
The value <code>length = sub_vec.second</code>
is the number of characters in the sub-vector.

\return
string verions of the sub-vector of characters:
\code
	char_vec[start] , char_vec[start+1] , ... , char_vec[start+length-1]
\endcode


\par Example
The following is an example and test of \c str_char_vec.
It returns true for success and false for failure.

\include str_char_vec_tst.cpp
*/
std::string str_char_vec(
	const vector<char>&              char_vec , 
	const std::pair<size_t,size_t>&  sub_vec  )
{	size_t start  = sub_vec.first;
	size_t length = sub_vec.second;
	std::string str(length, ' ');
	size_t i;
	for(i = 0; i < length; i++)
		str[i] = char_vec[start + i];

	return str;
}


DISMOD4_END_NAMESPACE
