# include <fstream>
# include <dismod_pde/csv_header.hpp>

bool csv_header_tst(void)
{	bool ok = true;
	std::cout << "csv_header:    ";

	// name of temporary csv file
	std::string file_name = "csv_header_tst.tmp";

	// create the temporary csv file
	std::fstream file;
	file.open(file_name.c_str(), std::fstream::out);
	file << "name one,\"name,two\",name three"   << std::endl;
	file.close();

	// Get the header in the csv file
	dismod_pde::vector<std::string> field;
	dismod_pde::csv_header(file_name, field);

	// check the number of fields
	ok &= field.size() == 3;
	ok &= field[0] == "name one";
	ok &= field[1] == "name,two";
	ok &= field[2] == "name three";

	if( ok )
		std::cout << "OK";
	else std::cout << "Error";
	std::cout << std::endl;

	return ok;
}
