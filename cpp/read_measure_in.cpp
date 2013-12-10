# include <dismod_pde/read_measure_in.hpp>
# include <dismod_pde/atof_char_vec.hpp>
# include <dismod_pde/str_char_vec.hpp>
# include <dismod_pde/like_name2enum.hpp>
# include <dismod_pde/csv_read.hpp>
# include <dismod_pde/csv_header.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file read_measure_in.cpp
Implementation of the \ref read_measure_in() routine.
*/
/*!
Routine that reads the csv file \c measure_in.csv as documented in
dismod_csv.cpp.

\param [in] file
is the name of the \c measure_in.csv file.

\param [out] covariate_name
The input size and contents of this vector do not matter.
Upon return, is the vector of covariate names that appear in the 
\c measuure_in.csv file; 
i.e., column names that begin with the characters \c x_.
The size of this vector is the number of covariates \c n_covariate.
We refer to the covariate with name <code>covariate_name[m]</code>
as the m-th covariate below.

\param [out] measure_in
The input size and contents of this vector do not matter.
Upon return, it has size equal to the number of measurements \c n_measure
and it contents correspond to the values in the \c measure_in.csv file
(with the exception of the covariate information). 

\param [out] covariate_value
For <code>ell = 0 , ... , n_measure-1</code>,
<code>i = 0 , ... , n_covariate-1</code>,
The value for the \c ell-th measurement and \c i-th covariate is
\code
	covariate_value[ ell * n_covariate + i ]
\endcode.
*/
void read_measure_in(
	const std::string&                     file                 ,             
	vector<std::string>&                   covariate_name       ,
	vector<dismod_pde::Measure_Input_Struct>& measure_in           ,
	vector<double>&                        covariate_value      )
{
	std::ostringstream msg;
	using std::endl;

	using dismod_pde::vector;
	using dismod_pde::atof_char_vec;
	using dismod_pde::str_char_vec;

	// some temporary indices
	size_t i, ell, q, match;
	std::string str;

	// get a copy of the header for the input file
	vector<std::string> header;
	dismod_pde::csv_header(file, header);

	// determine the covariate names
	covariate_name.resize(0);
	for(i = 0; i < header.size(); i++)
	{	if( header[i].substr(0, 2) == "x_" )
			covariate_name.push_back( header[i] );
	} 

	// some constants
	const vector<std::string> integrand_name = dismod_pde::integrand_enum.name();
	const size_t              n_integrand    = integrand_name.size();
	const size_t              n_covariate    = covariate_name.size();
	const size_t n_field                     = 9 + n_covariate;

	// measurement file field names
	vector<std::string> field(n_field);
	// double fields
	field[0]  = "meas_value";
	field[1]  = "age_lower";
	field[2]  = "age_upper";
	field[3]  = "time_lower";
	field[4]  = "time_upper";
	field[5]  = "meas_sigma";
	field[6]  = "meas_zeta";
	// string fields
	field[7]  = "integrand";
	field[8]  = "meas_like";

	// covariate fields
	for(i = 0; i < n_covariate; i++)
		field[9+i] = covariate_name[i]; 

	// call csv_read
	vector<char>   raw;
	vector< std::pair<size_t,size_t> > sub_vec;
	dismod_pde::csv_read(file, field, raw, sub_vec);
	assert( sub_vec.size() % n_field == 0 );
	size_t n_measure = sub_vec.size() / n_field - 1;

	// fill in the values in covariate_value
	covariate_value.resize( n_measure * n_covariate );
	for(ell = 0; ell < n_measure; ell++)
	{	for(i = 0; i < n_covariate; i++)
		{	size_t index = (ell + 1) * n_field + 9 + i;	
			covariate_value[ell * n_covariate + i] = atof_char_vec(
				raw, sub_vec[index], ell, file
			);
		}
	}

	// fill in the numeric values in measure_in
	measure_in.resize(n_measure);
	for(ell = 0; ell < n_measure; ell++) 
	{	bool ok = true;
		const char* error_message = CPPAD_NULL;
		size_t index = (ell + 1) * n_field + 0;
		measure_in[ell].meas_value = atof_char_vec(
			raw, sub_vec[index++], ell, file
		);
		measure_in[ell].age_lower  = atof_char_vec(
			raw, sub_vec[index++], ell, file
		);
		measure_in[ell].age_upper  = atof_char_vec(
			raw, sub_vec[index++], ell, file
		);
		measure_in[ell].time_lower = atof_char_vec(
			raw, sub_vec[index++], ell, file
		);
		measure_in[ell].time_upper = atof_char_vec(
			raw, sub_vec[index++], ell, file
		);
		measure_in[ell].meas_sigma       = atof_char_vec(
			raw, sub_vec[index++], ell, file
		);
		measure_in[ell].meas_zeta        = atof_char_vec(
			raw, sub_vec[index++], ell, file
		);
		if( measure_in[ell].age_upper < measure_in[ell].age_lower )
		{	error_message = "age_upper < age_lower";
			ok            = false;
		}
		if( measure_in[ell].time_upper < measure_in[ell].time_lower )
		{	error_message = "time_upper < time_lower";
			ok            = false;
		}
		if( measure_in[ell].meas_sigma < 0.0 )
		{	error_message = "sigma < 0";
			ok            = false;
		}
		if( measure_in[ell].meas_zeta < 0.0 )
		{	error_message = "sigma < 0";
			ok            = false;
		}
		if( ! ok )
		{	msg << endl;
			msg << "dismod_pde_csv: error at line " << ell + 1;
			msg << " in file " << file << endl;
			msg << error_message << str << endl;
			DISMOD4_ASSERT_MSG(false, msg.str());
		}
	}
	// set integrand values in measure_in
	for(ell = 0; ell < n_measure; ell++)
	{	str   = str_char_vec(raw, sub_vec[ (ell+1) * n_field + 7 ] );
		match = n_integrand;
		for(q = 0; q < n_integrand; q++)
		{	if( str == integrand_name[q] )
				match = q; 
		}
		if( match < n_integrand )
			measure_in[ell].integrand  = dismod_pde::Integrand_Enum(match);
		else
		{	msg << endl;
			msg << "dismod_pde_csv: error at line " << ell + 1;
			msg << " in file " << file << endl;
			msg << "integrand = " << str << endl;
			DISMOD4_ASSERT_MSG(false, msg.str());
		}
	}
	// set meas_like values in measure_in
	for(ell = 0; ell < n_measure; ell++)
	{	str = str_char_vec(raw, sub_vec[ (ell+1) * n_field + 8 ] );
		measure_in[ell].meas_like       = like_name2enum(str, ell+1, file);
	}
	// skip the comment
	return;
}

DISMOD4_END_NAMESPACE
