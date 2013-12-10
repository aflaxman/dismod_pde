# include <string>
# include <cstring>
# include <fstream>
# include <dismod_pde/write_measure_out.hpp>
# include <dismod_pde/output.hpp>
# include <dismod_pde/csv_header.hpp>
# include <dismod_pde/csv_read.hpp>
# include <dismod_pde/str_char_vec.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file write_measure_out.cpp
Implementation of the \ref write_effect_out() routine.
*/
/*!
Routine that writes the csv file \c measure_out.csv as documented in
dismod_csv.cpp.

\param [in] file_in
is the name of the csv file \c measure_in.csv which contains information
that is copied to \c file_out.

\param [in] file_out
is the name of the csv file \c measure_out.csv that we are writing.

\param [in] measure_in
The measurement input information.

\param [in] measure_out
The measurement output information.
*/

void write_measure_out(
	const std::string&                                     file_in     , 
	const std::string&                                     file_out    , 
	const dismod_pde::vector<dismod_pde::Measure_Input_Struct>&  measure_in  ,
	const dismod_pde::vector<dismod_pde::Measure_Output_Struct>& measure_out )
{	using dismod_pde::vector;
	using std::cerr;
	using std::endl;
	using dismod_pde::output;
	size_t i, j;
	std::ostringstream msg;



	// get a copy of the header for the measure_in file
	vector<std::string> header_in;
	dismod_pde::csv_header(file_in, header_in);
	size_t n_in = header_in.size();

	// read the entire measure_in file
	vector<char> raw_in;
	vector< std::pair<size_t,size_t> > sub_vec_in;
	dismod_pde::csv_read(file_in, header_in, raw_in, sub_vec_in);
	assert( sub_vec_in.size() % n_in == 0 );
	size_t n_measure = sub_vec_in.size() / n_in - 1;
	assert( measure_in.size() == measure_out.size() );
	if( n_measure != measure_out.size() )
	{	msg << endl;
		msg << "dismod_pde_csv: the file " << file_in << endl;
		msg << "changed while dismod_pde_csv was running." << endl;
		DISMOD4_ASSERT_MSG(false, msg.str());	
	}

	// Initial fields in the output file
	const char* header_begin[] = {
		"age",
		"cohort",
		"time",
		"integrand",
		"meas_value",
		"adjusted",
		"model",
		"residual",
		"scaled"
	};
	const size_t n_begin = 
		sizeof(header_begin) / sizeof(header_begin[0]);

	// initialize total output header
	vector<std::string> header_out(n_begin);
	for(j = 0; j < n_begin; j++)
		header_out[j] = header_begin[j];

	// add columns in header_in that are not in header_begin
	vector<bool> duplicate( n_in );
	for(i = 0; i < n_in; i++)
	{	duplicate[i] = false;
		for(j = 0; j < n_begin; j++)
			duplicate[i] |= header_in[i].compare(header_begin[j]) == 0;
		bool warning = duplicate[i];
		warning     &= header_in[i].compare("integrand") != 0;
		warning     &= header_in[i].compare("meas_value") != 0; 
		if( warning )
		{	cerr << "dismod_pde warning: column " << header_in[i] << " ";
			cerr << "appears in file " << file_in << "." << endl;
			cerr << "It will be different in " << file_out << "." << endl;
		}
	}
	for(i = 0; i < n_in; i++) if( ! duplicate[i] )
		header_out.push_back( header_in[i] );

	// open the file and discard any current contents (truncate)
	std::ofstream fs( file_out.c_str(), std::ios_base::trunc );

	// write header on first line of file
	for(i = 0; i < header_out.size(); i++)
	{	fs << header_out[i];
		if( i != header_out.size() - 1 )
			fs << ",";
	}
	fs << std::endl;

	// write out data
	const vector<std::string> integrand_name = dismod_pde::integrand_enum.name();
	size_t ell;
	for(ell = 0; ell < n_measure; ell++)
	{	double a = 
			(measure_in[ell].age_lower + measure_in[ell].age_upper) / 2.;
		double t = 
			(measure_in[ell].time_lower + measure_in[ell].time_upper) / 2.;
		double c = t - a;
		assert(size_t( measure_in[ell].integrand ) < integrand_name.size());

		fs << a << ",";
		fs << c << ",";
		fs << t << ",";
		fs << integrand_name[ measure_in[ell].integrand ]   << ",";
		output(fs, measure_in[ell].meas_value); fs << ",";
		output(fs, measure_out[ell].adjusted); fs << ",";
		output(fs, measure_out[ell].model    ); fs << ",";
		output(fs, measure_out[ell].residual ); fs << ",";
		output(fs, measure_out[ell].scaled   );

		// now copy to columns, that are not duplicates, file_in
		std::string str;
		for(i = 0; i < n_in; i++) if( ! duplicate[i] )
		{	str = str_char_vec(raw_in, sub_vec_in[ (ell+1) * n_in + i ] );
			fs << "," << str;
		}
		fs << endl;
	}
	fs.close();
}
DISMOD4_END_NAMESPACE
