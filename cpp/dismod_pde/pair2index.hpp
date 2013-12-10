# ifndef DISMOD4_PAIR2INDEX_HPP
# define DISMOD4_PAIR2INDEX_HPP

# include <dismod_pde/define.hpp>

/*
$begin pair2index$$
$latex \newcommand{\W}[1]{\; #1 \;}$$
$latex \newcommand{\R}[1]{{\rm #1}}$$
$spell
	pde
	sfun
	indices
	bool
	const
	dismod
$$


$section Convert Between Age-Cohort Pairs and a Single Grid Index$$
$index grid, convert pair$$
$index grid, convert index$$
$index pair, grid convert$$
$index index, grid pair$$
$index age-cohort, grid index$$

$head Syntax$$
$codei%dismod_pde::pair2index %grid%$$
$codei%(%age%, %cohort%, %time_lower%, %time_upper%)%$$
$codei%
%a%      = %grid%.age()
%c%      = %grid%.cohort()
%lower%  = %grid%.time_lower()
%upper%  = %grid%.time_lower()
%n_grid% = %grid%.size()
%valid%  = %grid%.pack(%age_index%, %cohort_index%, %grid_pack%)
%grid%.unpack(%age_unpack%, %cohort_unpack%, %grid_index%)
%$$

$head Purpose$$
The stochastic differential equations in dismod_pde 
naturally expressed on a age-cohort grid
(and that is how the computation is done).
On the other hand, the user is interested in computing the
stochastic function values in a rectangular region in age and time.
The $code pair2index$$ routines aid in computing the correspondence
between age-cohort pairs to a unique single grid index.

$head age$$
$index age$$
The argument $icode age$$ has prototype
$codei%
	const dismod_pde::vector<double>& %age%
%$$
It specifies the grid of age values.
We use $icode n_age$$ to denote $icode%age%.size()%$$ which
must be greater than or equal two.
The vector $icode age$$ and must be strictly monotone increasing; i.e.,
$codei%
	%for% %j% = 1 ,%...%, %n_age% - 1,  %age%[%j% - 1] < %age%[%j%]
%$$

$head cohort$$
$index cohort$$
The argument $icode cohort$$ has prototype
$codei%
	const dismod_pde::vector<double>& %cohort%
%$$
It specifies the grid of cohort values.
We use $icode n_cohort$$ to denote $icode%cohort%.size()%$$
which must be greater than or equal two.
The vector $icode cohort$$ must be strictly monotone increasing; i.e.,
$codei%
	%for% %k% = 1 ,%...%, %n_cohort% - 1,  %cohort%[%k% - 1] < %cohort%[%k%]
%$$
The cohort grid must start at a low enough value so that
$codei%
	%cohort%[0] + %age%[%n_age% - 1] <= %time_lower%
%$$
The cohort grid must end at a high enough value so that
$codei%
	%time_upper% <= %cohort%[%n_cohort% - 1] + %age%[0]
%$$ 

$head time_lower$$
$index time, min$$
$index min, time$$
The argument $icode time_lower$$ has prototype
$codei%
	double %time_lower%
%$$
It specifies the lower time limit for the time values that must be 
surrounded by the valid age-cohort grid pairs.

$head time_upper$$
$index time, max$$
$index max, time$$
The argument $icode time_upper$$ has prototype
$codei%
	double %time_upper%
%$$
It specifies the upper time limit for the time values that must be 
surrounded by the valid age-cohort grid pairs.
Furthermore, $icode time_upper$$ must satisfy
$codei%
	%time_lower% <= %time_upper%
%$$

$head pair2index$$
Constructs a $code pair2index$$ object called $icode grid$$.

$comment ------------------------------------------------------------$$

$head grid.age$$
$index grid.age$$
The return value for this function has prototype
$codei%
	const dismod_pde::vector<double>& %a%
%$$ 
It is a copy of the $icode age$$ vector used to construct $icode grid$$.  

$head grid.cohort$$
$index grid.cohort$$
The return value for this function has prototype
$codei%
	const dismod_pde::vector<double>& %c%
%$$ 
It is a copy of the $icode cohort$$ vector used to construct $icode grid$$.

$head grid.time_lower$$
$index grid.time_lower$$
The return value for this function has prototype
$codei%
	double %lower%
%$$
It is a copy of the $icode time_lower$$ value used to construct $icode grid$$.

$head grid.time_upper$$
$index grid.time_upper$$
The return value for this function has prototype
$codei%
	double %upper%
%$$
It is a copy of the $icode time_upper$$ value used to construct $icode grid$$.

$head grid.size$$
$index grid.size$$
The return value of this function has prototype
$codei%
	size_t %n_grid%
%$$
Its value is the number of valid grid pairs
which is also equal to the number if single grid index values.

$comment ------------------------------------------------------------$$

$head Notation$$
$index grid, notation$$
$index notation, grid$$
We use $latex J$$ to denote $icode n_age$$,
$latex K$$ to denote $icode n_cohort$$,
$latex t_l$$ to denote $icode time_lower$$, and
$latex t_u$$ to denote $icode time_upper$$,
For $latex 0 = 1 , \ldots , J-1$$, 
we use $latex a_j$$ to denote $icode%age%[%j%]%$$.
For $latex k = 0 , \ldots , K-1$$, 
we use $latex c_k$$ to denote $icode%cohort%[%k%]%$$.

$subhead kappa$$
$index kappa(j, l)$$
$index kappa(j, u)$$
For $latex j = 0 , \ldots , J-1$$, we define
$latex \[
\begin{array}{rcl}
\kappa(j, l) 
& = & 
\max\{ k \W{:} 0 \leq k < K \W{\R{and}} c_k + a_j \leq t_l \}
\\
\kappa(j, u) 
& = & 
\min\{ k \W{:} 0 \leq k < K \W{\R{and}} c_k + a_1 \geq t_u \}
\end{array}
\] $$
It follows from the assumptions on the 
$cref/age/pair2index/age/$$ and 
$cref/cohort/pair2index/cohort/$$ vectors that
the sets corresponding to the maximums above are non-empty.

$subhead Computational Grid$$
$index valid, grid pair$$
$index pair, valid grid$$
The set of valid grid pairs (the computational grid) is
$index grid, valid pair$$
$latex \[
G 
=
\bigcup_{j=0}^{J-1}
	\bigcup_{k=\kappa(j,l)}^{\kappa(j,u)}
		\{ (j,k) \}
\] $$

$head age_index$$
$index age_index$$
The argument $icode age_index$$ has prototype
$codei%
	size_t %age_index%
%$$
It specifies an age index ($latex j$$ above).

$head cohort_index$$
$index cohort_index$$
The argument $icode cohort_index$$ has prototype
$codei%
	size_t %cohort_index%
%$$
It specifies a cohort index ($latex k$$ above).

$head valid$$
$index valid, grid pair$$
$index pair, valid grid$$
$index grid, valid pair$$
The return value $icode valid$$ has prototype
$codei%
	bool %valid%
%$$
The age-cohort pair $codei%(%age_index%, %cohort_index%)%$$ is 
in the computational grid (defined above) if and only if
$icode valid$$ is true.

$head grid_pack$$
$index grid_pack$$
$index pack, index$$
The argument $icode grid_pack$$ has prototype
$codei%
	size_t& %grid_pack%
%$$
Its input value does not matter.
If $icode valid$$ is true,
the return value of $icode grid_pack$$ is the single index 
corresponding to the specified grid pair,
$icode%grid_pack% < %n_grid%$$.
The single index is different for every valid grid pair.
$pre

$$
If $icode valid$$ is false,
the return value of $icode grid_pack$$ is not specified.


$comment ------------------------------------------------------------$$

$head age_unpack$$
$index age_unpack$$
$index unpack, age$$
The argument $icode age_unpack$$ has prototype
$codei%
	size_t& %age_unpack%
%$$
Its input value does not matter.
Upon return, it is the age index corresponding to $icode grid_index$$,
$icode%age_unpack% < %n_age%$$.

$head cohort_unpack$$
$index cohort_unpack$$
$index unpack, cohort$$
The argument $icode cohort_unpack$$ has prototype
$codei%
	size_t& %cohort_unpack%
%$$
It input value does not matter.
Upon return, it is the cohort index corresponding to $icode grid_index$$,
$icode%cohort_unpack% < %n_cohort%$$.

$head grid_index$$
$index grid_index, convert to pair$$
The argument $icode grid_index$$ has prototype
$codei%
	size_t %grid_index%
%$$
it is a single index that is a packed version of a valid age-cohort grid pair
and hence must be less that $icode%n_grid%$$.

$children%
	example/pair2index.cpp
%$$
$head Example$$
The file $cref pair2index.cpp$$ 
contains an example and test of $code pair2index$$.
It returns true if $code pair2index$$ passes the test and false
otherwise.

$end
*/


DISMOD4_BEGIN_NAMESPACE
/*!
file pair2index.hpp
Include file for class that packs and upacks gridded Data in a vector
*/

/*!
Class that packs and upacks valid age-cohort pairs in a single index
*/
class pair2index {
private:
	/// grid values for age
	const vector<double> age_;

	/// grid values for cohort 
	const vector<double> cohort_;

	/// lower time limit for this grid
	const double         time_lower_;

	/// upper time limit for this grid
	const double         time_upper_;

	/// total number of grid index values required to pack the data
	/// (Effectively const becasue only set in constructor.)
	size_t               size_;

	/// mapping from grid index to the corresponding corresponding age index
	/// (effectively const becasue only set in constructor)
	vector<size_t>       grid2age_index_;    

	/// mapping from age index to grid index where data for the specified
	/// age starts (effectively const becasue only set in constructor)
	vector<size_t>       age2grid_index_;

	/// mapping from age index to first cohort for the corresponding age.
	/// (Effectively const becasue only set in constructor.)
	vector<size_t>       age2first_cohort_index_;

	/// mapping from age index to last cohort for the corresponding age
	/// (effectively const because only set in constructor)
	vector<size_t>       age2last_cohort_index_;

	/// mapping from cohort index to first valid age index that is
	/// greater than zero or n_age if no such index exists
	/// (effectively const because only set in constructor)
	vector<size_t>       cohort2first_age_index_;

public:
	// ------------------------------------------------------------
	// constructor
	pair2index(
		const vector<double>& age           ,
		const vector<double>& cohort        ,
		double                time_lower    ,
		double                time_upper
	);
	// access constructor information
	const vector<double>& age(void)        const;
	const vector<double>& cohort(void)     const;
	double                time_lower(void) const;
	double                time_upper(void) const;
	// number of points in the grid
	size_t                size(void)       const;
	// conver from pair to single
	bool pack(
		size_t                age_index     ,
		size_t                cohort_index  ,
		size_t&               grid_index
	) const;
	void unpack(
		size_t&               age_index     ,
		size_t&               cohort_index  ,
		size_t                grid_index
	) const;
};


DISMOD4_END_NAMESPACE


# endif
