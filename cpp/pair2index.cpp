# include <sstream>
# include <dismod_pde/define.hpp>
# include <dismod_pde/pair2index.hpp>

DISMOD4_BEGIN_NAMESPACE

/*!
\file pair2index.cpp
Implementaiton of a class that converts between,
pairs of indices in the dismod_pde computational grid,
and a single index in a linear vector.
*/

/// Constructor
pair2index::pair2index(
	/// value of age_
	const vector<double>& age           ,
	/// value of cohort_
	const vector<double>& cohort        ,
	/// value of time_lower_
	double               time_lower     ,
	/// value of time_upper_
	double               time_upper     )
: 
       age_(age)                         ,
    cohort_(cohort)                      ,
time_lower_(time_lower)                  ,
time_upper_(time_upper)
{
	size_t j, k, grid_index;
	size_t n_age    = age.size();
	size_t n_cohort = cohort.size();
# ifndef NDEBUG
	std::ostringstream msg;
	using std::endl;
	// check time limits
	DISMOD4_ASSERT_MSG( time_lower <= time_upper  ,
		"pair2index: time_upper < time_lower"
	);
     // check size of age and cohort vectors
	DISMOD4_ASSERT_MSG(
		n_age >= 2 && n_cohort >= 2,
		"pair2index: age.size() or cohort.size() is less than two"
	);
	// check age vector is monotone
	msg << endl;
	for(j = 1; j < n_age; j++) if( age[j-1] >= age[j] ) 
	{	msg << "pair2index: age is not monotone increasing" << endl
		<< "age[" << j-1 << "] = " << age[j-1]              << ", "
		<< "age[" << j << "] = " << age[j] << endl; 
		DISMOD4_ASSERT_MSG(false , msg.str() );
	}
	// check cohort vector is monotone
	for(k = 1; k < n_cohort; k++) if( cohort[k-1] >= cohort[k] ) 
	{	msg << "pair2index: cohort is not monotone increasing" << endl
		<< "cohort[" << k-1   << "] = " << cohort[k-1]         << ", "
		<< "cohort[" << k << "] = " << cohort[k] << endl;
		DISMOD4_ASSERT_MSG(false  , msg.str() );
	}
	// check first cohort
	double check  = cohort[0] + age[n_age-1];
	if( time_lower < check )
	{	msg << "pari2index: time_lower < cohort[0] + age[J-1]" << endl
		<< "time_lower = "  << time_lower<< ", "
		<< "cohort[0] = "   << cohort[0] << ", "
		<< "age_[J-1] = "  << age[n_age-1] << endl;
		DISMOD4_ASSERT_MSG(false , msg.str() );
	}
	// check second cohort
	check  = cohort[1] + age[n_age-1];
	if( check <= time_lower )
	{	msg << "pari2index: cohort[0] not needed because "
		<< "cohort[1] + age[J-1] <= time_lower" << endl
		<< "time_lower = "  << time_lower<< ", "
		<< "cohort[1] = "   << cohort[1] << ", "
		<< "age_[J-1] = "  << age[n_age-1] << endl;
		DISMOD4_ASSERT_MSG(false , msg.str() );
	}
	// check last cohort
	check  = cohort[n_cohort-1] + age[0];
	if( check < time_upper )
	{	msg << "pari2index: cohort[K-1] + age[0] < time_upper" << endl
		<< "time_upper = "  << time_upper<< ", "
		<< "cohort[K-1] = "   << cohort[n_cohort-1] << ", "
		<< "age_[0] = "  << age[0] << endl;
		DISMOD4_ASSERT_MSG(false, msg.str() );
	}
	// check next to last cohort
	check  = cohort[n_cohort-2] + age[0];
	if( time_upper <= check )
	{	msg << "pari2index: cohort[K-1] not needed because "
		<< "time_upper <= cohort[K-2] + age[0]" << endl
		<< "time_upper = "   << time_upper<< ", "
		<< "cohort[K-2] = "  << cohort[n_cohort-2] << ", "
		<< "age_[0] = "      << age[0] << endl;
		DISMOD4_ASSERT_MSG(false , msg.str() );
	}
# endif
	// member data that we now know the size of 
	age2grid_index_.resize(n_age);
	age2first_cohort_index_.resize(n_age);
	age2last_cohort_index_.resize(n_age);
	cohort2first_age_index_.resize(n_cohort);

	// initialize as no valid age index for each cohort
	for(k = 0; k < n_cohort; k++)
		cohort2first_age_index_[k] = n_age;
	//
	grid_index          = 0;
	for(j = 0; j < n_age; j++)
	{	// -----------------------------
		// start of grid values for this age
		age2grid_index_[j] = grid_index;

		// -------------------------------------------------
		// determine first cohort index for this age
		k = 0;
		while( age[j] + cohort[k+1] <= time_lower && k < n_age - 2)
			++k;
		age2first_cohort_index_[j] = k;
		// -------------------------------------------------
		// determine last cohort index for this age
		k = n_cohort - 1;
		while( time_upper <= age[j] + cohort[k-1] && 0 < k)
			--k;
		age2last_cohort_index_[j] = k;
		// -------------------------------------------------------
		assert( age2last_cohort_index_[j] > age2first_cohort_index_[j] );
		size_t difference = age2last_cohort_index_[j]
		                  - age2first_cohort_index_[j];
		grid_index += (difference + 1);
		//
		k = age2first_cohort_index_[j];
		while( k <= age2last_cohort_index_[j] )
		{	if( cohort2first_age_index_[k] == n_age )
			{	// ---------------------------	
				cohort2first_age_index_[k] = j;
				// ---------------------------	
			}
			k++;
		}
	}
	// ----------------
	size_ = grid_index;
	// ----------------
	grid2age_index_.resize(size_);
	grid_index = 0;
	for(j = 0; j < n_age; j++)
	{	k = age2first_cohort_index_[j];
		while( k <= age2last_cohort_index_[j] )
		{	// -------------------------------
			grid2age_index_[grid_index++] = j;
			// -------------------------------
			k++;
		}
	}
	assert( grid_index == size_ );
}

/*!
Return age vector for this grid.
*/
const vector<double>& pair2index::age(void) const
{	return age_; }

/*!
Return cohort vector for this grid.
*/
const vector<double>& pair2index::cohort(void) const
{	return cohort_; }

/*!
Return lower time limit for this grid.
*/
double pair2index::time_lower(void) const
{	return time_lower_; }


/*!
Return upper time limit for this grid.
*/
double pair2index::time_upper(void) const
{	return time_upper_; }

/*!
Return the number of indices
required in a pack all the valid grid pairs into a grid index.
*/
size_t pair2index::size(void) const
{	return size_; }

/*!
Determine the grid index corresponding to a grid pair.

\param [in] age_index
specifies the age value for a corresponding grid point.

\param [in] cohort_index
specifies the cohort value for a corresponding grid point.

\param [out] grid_index
The input value of grid_index does not matter.
If the return for \c pack is true,
the return value of \a grid_index is the index in a linear vector
corresponding to the specified grid point,
<code> grid_index < grid.size() </code>.
If the return value for \c pack is false,
the return value of \a grid_index is not specified.

\return
True if and only if this is a valid grid pair; i.e.
<code>age_index  < age.size() </code>
<code>cohort_index < cohort.size() </code>
and this grid point is required to surround the limits
for age and time.
*/
bool pair2index::pack(
     size_t                age_index     ,
     size_t                cohort_index  ,
     size_t&               grid_index  ) 
const
{
	if( age_index >= age_.size() )
		return false;
	if( cohort_index < age2first_cohort_index_[age_index] )
		return false;
	if( cohort_index > age2last_cohort_index_[age_index] )
		return false;

	grid_index = age2grid_index_[age_index] 
	           + cohort_index - age2first_cohort_index_[age_index];
	return true;
}

/*!
Determine the grid pair that corresponds to a grid index.

\param [out] age_index
specifies the age value for a corresponding grid index.

\param [out] cohort_index
specifies the cohort value for a corresponding grid index.

\param [in] grid_index$$
The value of grid_index is the packed grid index 
corresponding to the grid point,
<code> grid_index < grid.size() </code>.
*/
void pair2index::unpack(
     size_t&               age_index     ,
     size_t&               cohort_index  ,
     size_t                grid_index  ) 
const
{	assert( grid_index < size() );

	age_index     = grid2age_index_[ grid_index ];
	cohort_index  = grid_index - age2grid_index_[age_index] 
	              + age2first_cohort_index_[age_index];
	return;
}

DISMOD4_END_NAMESPACE
