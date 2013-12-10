# ifndef DISMOD4_ATOF_CHAR_VEC_HPP
# define DISMOD4_ATOF_CHAR_VEC_HPP

/*
$begin atof_char_vec$$
$spell
	pde
	inf
	dismod
	atof_char_vec
	const
	std
$$

$section Convert a Sub-vector of a Character Vector to Double$$

$head Syntax$$
$icode%dbl% = atof_char_vec(%char_vec%, %sub_vec%, %line%, %file%)%$$


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
The first character in the ascii representation is
$icode%char_vec%[ %sub_vec%.first ]%$$.
The number of character in the ascii representation is
$icode%sub_vec%.second%$$.
This number of characters must be less than 100.

$head line$$
this argument has prototype
$codei%
	size_t %line%
%$$
Is the line number corresponding to this value
(used for error reporting).

$head file$$
this argument has prototype
$codei%
	const std::string& %file%
%$$
Is the file that this data was read from
(used for error reporting).

$head dbl$$
the return value has prototype
$codei%
	double %dbl%
%$$
and is the double precision value corresponding to 
the ascii representation mentioned below 
$cref/sub_vec/atof_char_vec/sub_vec/$$.

$head nan$$
If the ascii representation is $code nan$$,
double precision value $code dbl$$ is also nan.

$head inf$$
If the ascii representation is $code inf$$ ($code -inf$$),
double precision value $icode dbl$$ is 
$code DISMOD4_PLUS_INFINITY$$ ( $code DISMOD4_PLUS_INFINITY$$ ).

$children%
	example/atof_char_vec_tst.cpp
%$$

$head Example$$
The file $cref atof_char_vec_tst.cpp$$ contains
and example and test of $code atof_char_vec$$.
It returns true for success and false for failure.
$end
*/

# include <dismod_pde/define.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file atof_char_vec.hpp
Include file for \ref atof_char_vec().
*/

double atof_char_vec(
	const vector<char>&              char_vec ,
	const std::pair<size_t,size_t>&  sub_vec  ,
	size_t                           line     ,
	const std::string&               file 
);

DISMOD4_END_NAMESPACE
# endif
