/*
$begin str_char_vec_tst.cpp$$
$spell
	pde
	str_char_vec
	Dismod
	bool
	csv
	tst
$$

$section Example and Test of Conversion from Ascii to String$$
$index test, str_char_vec$$
$index example, str_char_vec$$


$head Syntax$$
$icode%ok% = str_char_vec_tst()%$$

$head ok$$
The return value $icode ok$$ has prototype
$codei%
	bool %ok% 
%$$
If true, 
$cref str_char_vec$$ passed this test.
Otherwise this return value is false.

$code
$verbatim%example/str_char_vec_tst.cpp%0%// BEGIN C++%// END C++%1%$$
$$
$end
*/
// BEGIN C++
# include <dismod_pde/str_char_vec.hpp>

bool str_char_vec_tst(void)
{	bool ok = true;

	// create the vector of characters
	// (0, 1, 2, 3, 4, 5, 6, 7, 8, 9)
	dismod_pde::vector<char> char_vec(10);
	size_t i;
	for(i = 0; i < 10; i++)
		char_vec[i] = char( '0' + i );

	std::pair<size_t,size_t> sub_vec;
	std::string              str;
	sub_vec.first  = 2;
	sub_vec.second = 2;
	str            = "23";
	ok &= ( dismod_pde::str_char_vec(char_vec, sub_vec) == str );
	sub_vec.first  = 2;
	sub_vec.second = 3;
	str            = "234";
	ok &= ( dismod_pde::str_char_vec(char_vec, sub_vec) == str );

	return ok;
}
// END C++
