# ifndef DISMOD4_CSV_READ_HPP
# define DISMOD4_CSV_READ_HPP
/*
$begin csv_read$$
$spell
	pde
	csv
	vec
	const
	std
	dismod
$$

$section Read an Entire CSV File Into Memory$$

$head Syntax$$
$codei%csv_read(%file%, %field%, %raw%, %sub_vec%)%$$

$head file$$
Is the name of the file we are reading and has prototype
$codei%
	const std::string& %file%
%$$

$head field$$
Is a vector of field names that are expected to be present in the file
and has prototype
$codei%
	const dismod_pde::vector<std::string>& %field%
%$$
We use $icode%n_field% = %field%.size()%$$.
For $icode%j% = 0 , %...% , %n_field%$$
it is assumed that there is not leading or trailing which space
in any of the filed names; e.g., 
$icode%field%[%j%][0] != ' '%$$.
In addition, all the field names are non-empty; i.e.,
$icode%field%[%j%]size() != 0%$$.

$head raw$$
This argument has prototype
$codei%
	dismod_pde::vector<char>&  %raw%
%$$
The input size and content of this vector does not matter.
Upon return, it contains the data read in from the file,
including the first line of the file; i.e., the field names.

$head sub_vec$$
This argument has prototype
$codei%
	dismod_pde::vector< std::pair<size_t, size_t> >& %sub_vec%
%$$
The input size and content of this argument does not matter.
We use $icode n_row$$  for the number of rows of data in the file,
including the first row which contains the field names.
Upon return, the size of $icode sub_vec$$ is 
$icode%n_row% * %n_field%$$ and for
$icode%i% = 0 , %...% , %n_row%-1%$$,
$icode%j% = 0 , %...% , %n_field%-1%$$,
if 
$codei%
	%start% = %sub_vec%[ %i% * %n_field% + %j% ].first
	%length% = %sub_vec%[ %i% * %n_field% + %j% ].second
%$$
then $icode%raw%[%start%]%$$ is the first character of the corresponding value
and $icode length$$ is the number of characters in the corresponding value.

$children%
	example/csv_read_tst.cpp
%$$

$head Example$$
The file $cref csv_read_tst.cpp$$ contains
and example and test of $code csv_read$$.
It returns true for success and false for failure.

$end
*/

# include <dismod_pde/define.hpp>
# include <utility>


DISMOD4_BEGIN_NAMESPACE
/*!
\file csv_read.hpp
Include file for \ref csv_read().
*/

void csv_read(
	const std::string&                     file    ,
	const vector<std::string>&             field   ,
	vector<char>&                          raw     , 
	vector< std::pair<size_t, size_t> >&   sub_vec
);

DISMOD4_END_NAMESPACE
# endif
