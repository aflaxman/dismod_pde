# include <dismod_pde/read_effect_in.hpp>
# include <dismod_pde/csv_read.hpp>
# include <dismod_pde/atof_char_vec.hpp>
# include <dismod_pde/str_char_vec.hpp>


DISMOD4_BEGIN_NAMESPACE
/*!
\file read_effect_in.cpp
Implementation of the \ref read_effect_in() routine.
*/
/*!
Routine that reads the csv file \c effect_in.csv as documented in 
dismod_csv.cpp.

\param [in] file
is the name of the \c effect_in.csv file.

\param [in] covariate_name
Is a vector with size equal to the number of covariates and
\c n_covariate, and <code>covariate_name[i]</code> is the name of the
i-th covariate field. 

\param [out] effect_in
The input size and contents of this vector do not matter.
Upon return, it has size equal to the number of covariates \c n_covariate.
For the i-th covariate we have,
<code>effect_in[i].lower</code> is the lower limit for its effect,
<code>effect_in[i].initial</code> is its initial value, and
<code>effect_in[i].upper</code> is its upper limit.
*/
void read_effect_in(
	const std::string&           file             ,
	const vector<std::string>&   covariate_name   ,
	vector<Effect_Input_Struct>& effect_in        )
{
	std::ostringstream msg;
	using std::endl;

	using dismod_pde::vector;
	using dismod_pde::atof_char_vec;
	using dismod_pde::str_char_vec;

	// some temporary indices
	std::string str;
	size_t i, n, m;

	// integrand names
	const vector<std::string> integrand_name = dismod_pde::integrand_enum.name();
	size_t n_integrand = integrand_name.size();

	// number of covariates 
	size_t n_covariate = covariate_name.size();

	// effect_in.csv file field names
	const size_t n_field   = 5;
	vector<std::string> field(n_field);
	field[0] = "integrand";
	field[1] = "covariate";
	field[2] = "lower";
	field[3] = "initial";
	field[4] = "upper";

	// call csv_read
	vector<char>   raw;
	vector< std::pair<size_t,size_t> > sub_vec;
	dismod_pde::csv_read(file, field, raw, sub_vec);
	assert( sub_vec.size() % n_field == 0 );
	size_t n_effect = sub_vec.size() / n_field - 1;

	// size the return values
	std::string name;
	effect_in.resize(n_effect);

	// set return values
	vector<bool> match(n_integrand * n_covariate);
	for(i = 0; i < n_integrand; i++)
		for(n = 0; n < n_covariate; n++)
			match[i * n_covariate + n] = false;
	for(m = 0; m < n_effect; m++)
	{	size_t line     = m + 2;
		size_t index    = (m + 1) * n_field;
		// ----------------------------------------------------------------
		name            = str_char_vec(raw, sub_vec[index++]  );
		size_t i_match  = n_integrand;
		for(i = 0; i < n_integrand; i++)
		{	if( name == integrand_name[i] )
				i_match = i;
		}
		if( i_match == n_integrand )
		{	msg << endl;
			msg << "dismod_pde_csv: error in effect_in.csv = ";							msg << file << endl;
			msg << name << " is not a valid integrand name" << endl;
			DISMOD4_ASSERT_MSG(false, msg.str());
		}
		// ----------------------------------------------------------------
		name            = str_char_vec(raw, sub_vec[index++]  );
		size_t n_match  = n_covariate;
		for(n = 0; n < n_covariate; n++)
		{	if( name == covariate_name[n] )
				n_match = n;
		}
		if( n_match == n_covariate )
		{	msg << endl;
			msg << "dismod_pde_csv: error in effect_in.csv = ";							msg << file << endl;
			msg << name << " is not a covariate in measure_in.csv" << endl;
			DISMOD4_ASSERT_MSG(false, msg.str());
		}
		// ----------------------------------------------------------------
		if( match[ i_match * n_covariate + n_match ] )
		{	msg << endl;
			msg << "dismod_pde_csv: error in effect_in.csv = ";
			msg << file << endl;
			msg << "Multiple lines with integrand, covariate = ";
			msg << integrand_name[i_match] << ", " ;
			msg << covariate_name[n_match] << endl;
			DISMOD4_ASSERT_MSG(false, msg.str());
		}
		double lower   = atof_char_vec(raw, sub_vec[index++], line, file );
		double initial = atof_char_vec(raw, sub_vec[index++], line, file );
		double upper   = atof_char_vec(raw, sub_vec[index++], line, file );
		if( lower > initial )
		{	msg << endl;
			msg << "dismod_pde_csv: error at line " << (i + 1);
			msg << " in effect_in.csv =  " << file << endl;
			msg << "lower > initial" << endl;
			msg << "lower = " << effect_in[i].lower << endl;
			msg << "initial = " << effect_in[i].initial << endl;
			DISMOD4_ASSERT_MSG(false, msg.str());
		}
		if( initial > upper )
		{	msg << endl;
			msg << "dismod_pde_csv: error at line " << (i + 1);
			msg << " in effect_in.csv = " << file << endl;
			msg << "initial > upper" << endl;
			msg << "initial = " << effect_in[i].initial << endl;
			msg << "upper = " << effect_in[i].upper << endl;
			DISMOD4_ASSERT_MSG(false, msg.str());
		}
		// ----------------------------------------------------------------
		effect_in[m].integrand = dismod_pde::Integrand_Enum(i_match);
		effect_in[m].covariate = n_match;
		effect_in[m].lower   = lower;
		effect_in[m].initial = initial;
		effect_in[m].upper   = upper;
	}
	return;
}

DISMOD4_END_NAMESPACE
