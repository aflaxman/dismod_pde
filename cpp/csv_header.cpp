# include <cstdio>
# include <string>
# include <cstring>
# include <utility>
# include <dismod_pde/csv_header.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file csv_header.cpp
Get the header information from a csv file; i.e., the field names.
*/

/*!
The following specifications for a CSV file come from omh/csv.omh:

The following specifications for a CSV file come from omh/csv.omh:

\li
It is an ascii file that does not contain any of the following characters:
single quote, back quote, or semi-colon.

\li
The comma is a special character because it separates fields in the file.
There is no comma after the last field in a line.

\li
The newline is a special character becasue it separates lines in the file.

\li
The carriage return is a special character because on Windows systems,
it is combined with the newline to form an end of line.
Therefore, a carriage return newline will be the same as a newline.
On MAC OS, the carraige return is used in place of a newline,
so if a carriage return is not followed by a newline, it is converted 
to a newline.;

\li
The double quote is a special character because,
when surrounded by a pair of double quote characters, 
the special meaning of the comma and newline (see above) are supressed.
When present,
the double quote must be both the first and last character in the field
and are not included in the field data.

\param [in] file
is the name of the file to read.

\param header
The input size and contents of this vector do not matter.
Upon return, it is
is a vector of field names that are present in the file.
It is an error for any of the field names to be empty.

\par Example
The following is an example and test of \c csv_header. 
It returns true for success and false for failure.

\include csv_header_tst.cpp
*/
void csv_header(
	const std::string&              file    ,
	vector<std::string>&            header )
{
	// make sure header starts as empty
	header.resize(0);

	// section of message that is the same for all errors 
	using std::endl;
	std::ostringstream msg;
	msg << endl << "csv_header: Error while reading file " << file << endl;

	// open the file
	FILE *fp    = std::fopen( file.c_str() , "r" );
	if( fp == NULL )
	{	msg << "Cannot open the file for reading." << endl;
		DISMOD4_ASSERT_MSG(false, msg.str())
	}

	int ch = '\0';
	while( (ch != '\n') & (ch != '\r') & (ch != EOF)  )
	{	std::string name;
		ch = std::getc(fp);

		// find the end of this field
		if( ch == '"' )
		{	ch = std::getc(fp);
			while((ch != '"') & (ch != '\n') & (ch != '\r') & (ch != EOF)) 
			{	name.push_back( static_cast<char>(ch) );
				ch = std::getc(fp);
			}
			if( ch != '"' )
			{	msg << "Double quote error in file header." << endl;
				DISMOD4_ASSERT_MSG(false, msg.str())
			}
			ch = std::getc(fp);
			if( (ch != ',') & (ch != '\n') & (ch != '\r') & (ch != EOF) )
			{	msg << "Double quote error in file header." << endl;
				DISMOD4_ASSERT_MSG(false, msg.str())
			}
		}
		else
		{	while((ch != ',') & (ch != '\n') & (ch != '\r') & (ch != EOF)) 
			{	name.push_back( static_cast<char>(ch) );
				ch = std::getc(fp);
				if( ch == '"' )
				{	msg << "Double quotes \" used in header, but" << endl;
					msg << "do not surround an entire field name." << endl;
					DISMOD4_ASSERT_MSG(false, msg.str());
				}
			}
		}
		header.push_back( name );
	}
	// close the file
	std::fclose(fp);
}

DISMOD4_END_NAMESPACE
