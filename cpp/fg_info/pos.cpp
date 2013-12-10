# include <dismod_pde/fg_info.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file fg_info_pos.cpp
Implementaiton of the \ref FG_info member function \ref FG_info::check_pos(). 
*/
// ------------------------------------------------------------------------
/*!
Check that all the elements of a vector are positive during
CppAD forward mode calculations (if not report as an error).

\param [in] vec
This is the vector of AD values that we are checking to see 
if it they are greater than zero. 

\param [in] describe
Is a description for why the minimum value in the vector \c vec
should be greater than zero.
If this is not true, a messsage is printed including the text
<code>min(vec) = </code> following by the minimum.

\param [in] age_index
index that determines age for grid point
(used to report an error if \a value is not positive).

\param [in] cohort_index
index that determnes cohort for grid point
(used to report an error if \a value is not positive).

\param [in] state
Vector of stochastic fucntion values on the grid
(used to report an error if \a value is not positive).
*/
void FG_info::check_pos(
	const char*              describe           ,
	const vector<ADdouble>&  vec                ,
	size_t                   age_index          , 
	size_t                   cohort_index       , 
	const ADvector&          state              ) const
{	size_t i;
	ADdouble min = vec[0];
	for(i = 1; i < vec.size(); i++)
		min = ( min + vec[i] - abs(min - vec[i]) ) / 2.;
	std::string min_label("\n");
	min_label += describe;
	min_label += "\n min(vec) = "; 
	//
	size_t g_index;
# ifndef NDEBUG
	bool ok = grid_.pack(age_index, cohort_index, g_index);
# else
	grid_.pack(age_index, cohort_index, g_index);
# endif
	assert( ok );
	size_t s_index = g_index * n_stochastic_;
	ADdouble a = age_[age_index];
	ADdouble c = cohort_[cohort_index];
	// print during recording
	if( ! ( min > 0. ) )
	{	using std::cout;
		cout << min_label  << min                           << ", ";
		cout << "age ="    << a                             << ", ";
		cout << "cohort =" << c                             << ", ";
		cout << "S = "     << state[s_index + S_enum]       << ", ";
		cout << "C = "     << state[s_index + C_enum]       << ", ";  
		cout << "iota = "  << state[s_index + iota_enum]    << ", ";  
		cout << "rho = "   << state[s_index + rho_enum]     << ", ";  
		cout << "chi = "   << state[s_index + chi_enum]     << ", ";  
		cout << "omega = "    << state[s_index + omega_enum]      << "\n";  
		DISMOD4_ASSERT_MSG( false, "min(vec) <= 0");
	}
	PrintFor(min, min_label.c_str(),                min,        ", ");
	PrintFor(min, "age = ",                           a,        ", ");
	PrintFor(min, "cohort = ",                        c,        "\n");
	PrintFor(min, "S = ",       state[s_index + S_enum],        ", ");
	PrintFor(min, "C = ",       state[s_index + C_enum],        ", ");  
	PrintFor(min, "iota = ", state[s_index + iota_enum],        ", ");  
	PrintFor(min, "rho = ",   state[s_index + rho_enum],        ", ");  
	PrintFor(min, "chi = ",   state[s_index + chi_enum],        ", ");  
	PrintFor(min, "omega = ",     state[s_index + omega_enum],        "\n");  
}
DISMOD4_END_NAMESPACE
