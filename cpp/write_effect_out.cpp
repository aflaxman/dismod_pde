# include <fstream>
# include <dismod_pde/read_effect_in.hpp>
# include <dismod_pde/output.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file write_effect_out.cpp
Implementation of the \ref write_effect_out() routine.
*/
/*!
Routine that writes the csv file \c effect_out.csv as documented in 
dismod_csv.cpp.

\param [in] file
is the name of the \c effect_out.csv file.

\param [in] covariate_name
has size equal to the number of covariates \c n_covariate, 
and <code>covariate_name[n]</code> is the name of the n-th covariate field. 

\param [in] effect_in
has size equal to the number of measurement effects \c n_effect,
and 
<code>effect_in[m].integrand</code>
and
<code>effect_in[m].covariate</code>
are the integrand and covariate for the \c m-th measurement effect.

\param [out] estimate
has size equal to the number of measurement effects \c n_effect, 
and <code>estimate[m]</code> is the optimal value for the 
m-th measurement effect. 
*/
void write_effect_out(
	const std::string&                 file            ,
	const vector<std::string>&         covariate_name  ,
	const vector<Effect_Input_Struct>& effect_in       ,
	const vector<double>&              estimate        )
{
	using dismod_pde::vector;
	using dismod_pde::output;

	// some temporary indices
	size_t j, m;

	// some constants 
	const vector<std::string> integrand_name = dismod_pde::integrand_enum.name();
	const size_t n_effect    = effect_in.size();
	const size_t n_field = 3;

	// effect_out.csv fields
	const char* field[] = {
		"integrand",
		"covariate",
		"estimate"
	};

     // open the file and discard any current contents (truncate)
     std::ofstream fs( file.c_str(), std::ios_base::trunc );

     // write field names on first line of file
     for(j = 0; j < n_field; j++)
     {    fs << field[j];
          if( j != n_field - 1 )
               fs << ",";
     }
     fs << std::endl;

	// write out data
	for(m = 0; m < n_effect; m++)
	{	fs << integrand_name[ effect_in[m].integrand ] << ",";
		fs << covariate_name[ effect_in[m].covariate ] << ",";
		output(fs, estimate[m]);   fs << std::endl;
	}
	fs.close();
}

DISMOD4_END_NAMESPACE
