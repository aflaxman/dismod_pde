/*
$begin csv_read_tst.cpp$$
$spell
	pde
	Dismod
	bool
	csv
	tst
$$

$section Example and Test of dismod_pde csv_read Procedure$$
$index test, csv_read$$
$index example, csv_read$$


$head Syntax$$
$icode%ok% = csv_read_tst()%$$

$head ok$$
The return value $icode ok$$ has prototype
$codei%
	bool %ok% 
%$$
If true, 
the $cref csv_read$$ passed this test.
Otherwise this return value is false.

$code
$verbatim%example/csv_read_tst.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
# include <fstream>
# include <dismod_pde/csv_read.hpp>

bool csv_read_tst(void)
{	bool ok = true;
	size_t j, k, start, length;

	// name of temporary csv file
	std::string file_name = "csv_read_tst.tmp";

	// data in the temporary file	
	const char* data = 
		"name one,name two,name three\n"            // header, row 0 
		"value 11,value 12,value 13\n"              // row 1
		"value 21,value 22,value 23\n"              // row 2
		"\"value 31\",\"value,32\",\"value\n33\"\n" // quotes around values
		"value 41,value 42,\n";                     // end row with comma
	

	// create the temporary csv file
	std::fstream file;
	file.open(file_name.c_str(), std::fstream::out);
	file << data;
	file.close();

	// read the csv file file
	size_t n_field = 3;
	dismod_pde::vector<std::string> field(n_field);
	field[0] = "name three";
	field[1] = "name two";
	field[2] = "name one";
	dismod_pde::vector<char>   raw;
	dismod_pde::vector< std::pair<size_t,size_t> > sub_vec;
	dismod_pde::csv_read(file_name, field, raw, sub_vec);

	// check number of rows of data
	size_t n_row = (sub_vec.size() / n_field);
	ok           &= n_row == 5;

	// check the first row of the data file
	size_t row = 0;
	for(j = 0; j < n_field; j++)
	{	start   = sub_vec[ row * n_field + j].first;
		length  = sub_vec[ row * n_field + j].second;	
		ok &= field[j].size() == length;   
		for(k = 0; k < length; k++)
			ok &= (field[j][k] == raw[ start + k ] );
	}

	// check the second row
	row = 1;
	field[0] = "value 13";
	field[1] = "value 12";
	field[2] = "value 11";
	for(j = 0; j < n_field; j++)
	{	start   = sub_vec[ row * n_field + j].first;
		length  = sub_vec[ row * n_field + j].second;	
		ok &= field[j].size() == length;   
		for(k = 0; k < length; k++)
			ok &= (field[j][k] == raw[ start + k ] );
	}

	// check the third row
	row = 2;
	field[0] = "value 23";
	field[1] = "value 22";
	field[2] = "value 21";
	for(j = 0; j < n_field; j++)
	{	start   = sub_vec[ row * n_field + j].first;
		length  = sub_vec[ row * n_field + j].second;	
		ok &= field[j].size() == length;   
		for(k = 0; k < length; k++)
			ok &= (field[j][k] == raw[ start + k ] );
	}

	// check the third row
	row = 3;
	field[0] = "value\n33";
	field[1] = "value,32";
	field[2] = "value 31";
	for(j = 0; j < n_field; j++)
	{	start   = sub_vec[ row * n_field + j].first;
		length  = sub_vec[ row * n_field + j].second;	
		ok &= field[j].size() == length;   
		for(k = 0; k < length; k++)
			ok &= (field[j][k] == raw[ start + k ] );
	}

	// check the fourth row
	row = 4;
	field[0] = "";
	field[1] = "value 42";
	field[2] = "value 41";
	for(j = 0; j < n_field; j++)
	{	start   = sub_vec[ row * n_field + j].first;
		length  = sub_vec[ row * n_field + j].second;	
		ok &= field[j].size() == length;   
		for(k = 0; k < length; k++)
			ok &= (field[j][k] == raw[ start + k ] );
	}

	return ok;
}
// END C++
