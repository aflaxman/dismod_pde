# include <cstdio>
# include <string>
# include <cstring>
# include <utility>
# include <dismod_pde/csv_read.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file csv_read.cpp
Implement a routine that reads a csv file.
*/

/*!
Read an entire csv file into memory.

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

\param field
is a vector of field names that are expected to be present in the file.
Each field name must match one of the values in the first row of the file.
The data for that field is in that column of the file.
We use the notation \c n_field for <code>field.size()</code>.
For each <code>j = 0 , ... , n_field - 1</code>,
it is assumed that there is no leading or trailing space or carriage return
in any of the field names; i.e.
\code
field[j][0] != ' '  && field[j] [ field[j].size() - 1 ] != ' '
field[j][0] != '\r' && field[j] [ field[j].size() - 1 ] != '\r'
\endcode
None of the field names are empty; i.e. <code>field[j].size() > 0</code>.

\param raw
The input size and contents of this argument do not matter.
Upon return, it contains the data read in from the file,
including the first line of the file; i.e., the field names.

\param sub_vec
The input size and contents of this argument do not matter.
We use the notaiton \c n_row for the number of data rows in the file.
Upon return, the size of \c sub_vec is <code>n_row * n_field</code> 
and for 
<code>row = 0 , ... , n_row - 1</code>, 
<code>j   = 0 , ... , n_field - 1</code>, 
The value 
\code
raw[ sub_vec[ row * n_field + j ].first ]
\endcode
is the first character of the data for field <code>field[j]</code>
that was in the specified row of the file.
The value 
\code
sub_vec[ row * n_field + j ].second
\endcode
is the number of character of the data for field <code>field[j]</code>
that was in the specified row of the file.

\par Example
The following is an example and test of \c csv_read_tst. 
It returns true for success and false for failure.

\include csv_read_tst.cpp
*/
void csv_read(
	const std::string&                    file    ,
	const vector<std::string>&            field   ,
	vector<char>&                         raw     , 
	vector< std::pair<size_t, size_t> >&  sub_vec )
{

	// section of message that is the same for all errors 
	using std::endl;
	std::ostringstream msg;
	msg << endl << "csv_read: Error while reading file: " << file << endl;

	// some temporaries	
	size_t i, j, k;

	// number of fields we are accessing
	const size_t n_field = field.size(); 

	// check for a zero length field
	for(j = 0; j < n_field; j++)
	{	if( field[j].size() == 0 )
		{	msg << "Program Error: file[j].size() = 0 for some j." << endl;
			DISMOD4_ASSERT_MSG(false, msg.str());
		}
	}

	// open the file
	FILE *fp    = std::fopen( file.c_str() , "r" );
	if( fp == NULL )
	{	msg << "Cannot open the file for reading." << endl;
		DISMOD4_ASSERT_MSG(false, msg.str());
	}

	// -----------------------------------------------------------------
	// read file, store in raw, check for forbidden characters,
	// check that there are an even number of double quotes,
	// remove, or convert, carriage returns at end of lines,
	// ensure raw ends with \n.
	int c;
	char ch = '\0';
	size_t line = 1;
	bool in_dquotes = false;
	raw.resize(0);
	for(c = std::getc(fp); c != EOF; c = std::getc(fp) )
	{	if( c == '\r' )
		{	c  = std::getc(fp);
			if( c != '\n' )
			{	raw.push_back('\n');
				++line;
			}
		}
		if( c != EOF );
		{	ch = c;
			raw.push_back(ch);
			if( ch == '\n' )
				++line;
			if( ch == '"' )
				in_dquotes = ! in_dquotes;
# ifndef NDEBUG
			const char* forbidden="'`;";
			if( std::strrchr(forbidden, ch) != NULL )
			{	msg << "The character " << ch << " appears in ";
				msg << "line " << line << " of file." << endl;
				DISMOD4_ASSERT_MSG(false, msg.str());
			}
# endif
		}
	}
	if( in_dquotes )
	{	msg << "An odd number of \" characters appear in file." << endl;
		DISMOD4_ASSERT_MSG(false, msg.str());
	}
	// make sure the is a '\n' at the end
	if( ch != '\n' )
		raw.push_back('\n');
	size_t n_raw = raw.size();
	// close the file
	std::fclose(fp);
		
	// ----------------------------------------------------------------------
	// Use header to determine mapping from field index to file column number 
	vector<size_t> field2column(n_field);
	for(j = 0; j < n_field; j++) 
		field2column[j] = n_raw + 1; // initialize as not yet found

	size_t column = 0;     // number of columns found
	size_t next   = 0;     // next input character
	ch            = '\0';  // avoid match if '\n' is first character in line
	assert(! in_dquotes );
	while( ch != '\n' ) 
	{
		// set the first character in this field
		size_t start = next;	 
		ch           = raw[next];

		// check for field surrounded by double quotes
		in_dquotes = ch == '"';

		// find the end of this field
		size_t end;
		if( in_dquotes )
		{	++start;
			ch = raw[++next];
			while(ch != '"' )
				ch = raw[++next];
			in_dquotes = false;
			end = next;
			ch = raw[++next];
			if( (ch != ',') & (ch != '\n') )  
			{	msg << "Double quote \" used in header, but" << endl;
				msg << "do not surround an entire field name." << endl;
				DISMOD4_ASSERT_MSG(false, msg.str());
			}
		}
		else
		{	while( (ch != ',') & (ch != '\n') )  
			{	ch = raw[++next];
				if( ch == '"' )
				{	msg << "Double quotes \" used in header, but" << endl;
					msg << "do not surround an entire field name." << endl;
					DISMOD4_ASSERT_MSG(false, msg.str());
				}
			}
			end = next;
		}
		// check if this column matches one of the fields
		for(j = 0;  j < n_field; j++)
		{	// check for a match
			k          = field[j].size();
			bool match = (k == end - start);
			if(match)
				while(k--)
					match &= (field[j][k] == raw[start + k]);
			if( match )
			{	if( field2column[j] != n_raw + 1 )
				{	msg << "Multiple colums in header with name \"";
					msg << field[j] << "\"" << endl;
					DISMOD4_ASSERT_MSG(false, msg.str());
				}
				field2column[j] = column;
			}
		}
		if( start == end )
		{	msg << "Empty field name appears in header." << endl;
			DISMOD4_ASSERT_MSG(false, msg.str());
		}
		// Point to first character in next field, but do not set ch yet
		++next;

		// count the number of columns in the data file
		++column;
	}
	size_t n_column = column;

	// initialize mapping from column number to field index as not valid
	vector<size_t> column2field(n_column);
	for(column = 0; column < n_column; column++)
		column2field[column] = n_field;

	// set the valid entries in the column2field
	for(j = 0; j < n_field; j++)
	{	if( field2column[j] == n_raw + 1 )
		{	msg << "Cannot find the column name \"";
			msg << field[j] << "\"." << endl << "Check for extra ";
			msg << "white space before or after name in header." << endl;
			DISMOD4_ASSERT_MSG(false, msg.str());
		}
		assert( field2column[j] < n_column );
		column2field[ field2column[j] ] = j;
	}

	// ----------------------------------------------------------------------
	// count the number of rows in the file
	size_t n_row  = 0;
	assert( ! in_dquotes );
	for(i = 0; i < n_raw; i++)
	{	if( raw[i] == '"' )
			in_dquotes = ! in_dquotes;
		if( (! in_dquotes) & (raw[i] == '\n') )
			++n_row;
	}

	// ----------------------------------------------------------------------
	// make final pass through the data setting values in sub_vec.
	sub_vec.resize(n_row * n_field);
	next        = 0;          // index of first character
	ch          = '\0';       // value that cannot match ch == '\n'
	line        = 1;          // first input line number
	size_t row  = 0;          // first data row index
	size_t last = n_raw - 1;
	assert( ! in_dquotes );
	while( next < last )
	{	// for each line in the file
		size_t column = 0;

		// loop over entries in this line
		while( (ch != '\n') & (column < n_column) )
		{
			// set the first character in this field
			size_t start = next;	 
			ch           = raw[next];

			// check for field surrounded by double quotes
			in_dquotes = ch == '"';

			// find the end of this field
			size_t end;
			if( in_dquotes )
			{	++start;
				ch = raw[++next];
				while(ch != '"' )
				{	ch = raw[++next];
					if( ch == '\n' )
						++line;
				}
				in_dquotes = false;
				end = next;
				ch = raw[++next];
				if( (ch != ',') & (ch != '\n') )  
				{	msg << "Double quote \" user with a field value";
					msg << endl << "but does not surround the value.";
					msg << endl;
					DISMOD4_ASSERT_MSG(false, msg.str());
				}
			}
			else
			{	while( (ch != ',') & (ch != '\n') )  
				{	ch = raw[++next];
					if( ch == '"' )
					{	msg << "Double quotes \" used with a field value";
						msg << endl << "but does not surround the value";
						msg << " in line " << line << "." << endl;
						DISMOD4_ASSERT_MSG(false, msg.str());
					}
				}
				end = next;
			}
			assert( (ch == ',') | (ch == '\n') );  

			// check if this column matches one of the fields
			j = column2field[column];
			if( j < n_field )
			{	assert( row < n_row );

				sub_vec[row * n_field + j].first = start;
				sub_vec[row * n_field + j].second = end - start;
			}
			// advance to start of next field, but do not set character yet
			if( ch != '\n' )
				++next;
			// set up for the next column in the data file
			++column;
		}
		if( ch != '\n' )
		{	msg << "Found more than " << n_column << " fields in line "; 
			msg << line <<  "." << endl;
			DISMOD4_ASSERT_MSG(false, msg.str());
		} 
		if( column != n_column )
		{	msg << "Found " << column << " fields in line "; 
			msg << line << ", and expected " << n_column << "." << endl;
			DISMOD4_ASSERT_MSG(false, msg.str());
		} 
		// set up for next row in data file
		if( next < last )
		{	++line;
			++next;
			ch = '\0';
		}
		++row;
	}
	assert( row == n_row );
}

DISMOD4_END_NAMESPACE
