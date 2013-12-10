# ifndef DISMOD4_STR_CHAR_VEC_HPP
# define DISMOD4_STR_CHAR_VEC_HPP

/*
$begin str_char_vec$$
$spell
	pde
	dismod
	str_char_vec
	const
	std
$$

$section Convert a Sub-vector of a Character Vector to a String$$

$head Syntax$$
$icode%str% = str_char_vec(%char_vec%, %sub_vec%)%$$

$head char_vec$$
this argument has prototype
$codei%
	const dismod_pde::vector<char>& %char_vec%
%$$
It is a character vector containing this value (and possibly other values).


$head sub_vec$$
this argument has prototype
$codei%
	const std::pair<size_t, size_t>& %sub_vec%
%$$
The first character of the string is
$icode%char_vec%[ %sub_vec%.first ]%$$.
The number of character in the string is
$icode%sub_vec%.second%$$.

$head str$$
The return value has prototype
$codei%
	std::string %str%
%$$
and is the string that has the corresponding characters 
in the vector $icode char_vec$$.

$children%
	example/str_char_vec_tst.cpp
%$$

$head Example$$
The file $cref str_char_vec_tst.cpp$$ contains
and example and test of $code str_char_vec$$.
It returns true for success and false for failure.

$end
*/

# include <string>
# include <dismod_pde/define.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file str_char_vec.hpp
Include file for \ref str_char_vec().
*/

std::string str_char_vec(
	const vector<char>&              char_vec ,
	const std::pair<size_t,size_t>&  sub_vec
);

DISMOD4_END_NAMESPACE
# endif
