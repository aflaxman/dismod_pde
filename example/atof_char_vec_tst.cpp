/*
$begin atof_char_vec_tst.cpp$$
$spell
	pde
	atof_char_vec
	Dismod
	bool
	csv
	tst
$$

$section Example and Test of Conversion from Ascii to Double$$
$index test, atof_char_vec$$
$index example, atof_char_vec$$


$head Syntax$$
$icode%ok% = atof_char_vec_tst()%$$

$head ok$$
The return value $icode ok$$ has prototype
$codei%
	bool %ok% 
%$$
If true, 
$cref atof_char_vec$$ passed this test.
Otherwise this return value is false.

$code
$verbatim%example/atof_char_vec_tst.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
# include <dismod_pde/atof_char_vec.hpp>
# include <cstring>

bool atof_char_vec_tst(void)
{	bool ok = true;
	double result;
	size_t i;

	// raw character data
	// index              012345678901234567
	const char* raw    = "12 43 inf -inf nan";
	const size_t n_raw = std::strlen(raw);

	// vector form
	dismod_pde::vector<char> char_vec(n_raw);
	for(i = 0; i < n_raw; i++)
		char_vec[i] = raw[i];

	// used for error reporting
	size_t line = 2;
	std::string file = "my.csv";

	std::pair<size_t,size_t> sub_vec;
	//
	sub_vec.first  = 0;
	sub_vec.second = 2;
	result = dismod_pde::atof_char_vec(char_vec, sub_vec, line, file);
	ok &= result == 12.;
	//
	sub_vec.first  = 3;
	sub_vec.second = 2;
	result = dismod_pde::atof_char_vec(char_vec, sub_vec, line, file);
	ok &= result == 43.;
	//
	sub_vec.first  = 6;
	sub_vec.second = 3;
	result = dismod_pde::atof_char_vec(char_vec, sub_vec, line, file);
	ok &= result == DISMOD4_PLUS_INFINITY;
	//
	sub_vec.first  = 10;
	sub_vec.second = 4;
	result = dismod_pde::atof_char_vec(char_vec, sub_vec, line, file);
	ok &= result == - DISMOD4_PLUS_INFINITY;
	//
	sub_vec.first  = 15;
	sub_vec.second = 3;
	result = dismod_pde::atof_char_vec(char_vec, sub_vec, line, file);
	ok &= dismod_pde::isnan(result);
	//
	return ok;
}
// END C++
