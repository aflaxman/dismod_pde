# ifndef DISMOD4_CREATE_GIRD_HPP
# define DISMOD4_CREATE_GIRD_HPP

/*
$begin create_grid$$
$spell
	pde
	const
	dismod
	Struct
$$

$section Create an Age-Cohort Grid For a Measurement Set$$
$index create, grid$$
$index grid, create$$

$head Syntax$$
$icode%grid% = create_grid(%delta_age%, %delta_cohort%, %measure_in%)%$$

$head delta_age$$
$index delta, age$$
$index age, delta$$
The argument $icode delta_age$$ has prototype
$codei%
	double %delta_age%
%$$
It must be greater than zero and 
specifies the step size used between age values in the grid.

$head delta_cohort$$
$index delta, cohort$$
$index cohort, delta$$
The argument $icode delta_cohort$$ has prototype
$codei%
	double %delta_cohort%
%$$
It must be greater than zero and 
specifies the step size used between cohort values in the grid.

$head measure_in$$
$index measure_in$$
The argument $icode measure_in$$ has prototype
$codei%
	const dismod_pde::vector<dismod_pde::Measure_Input_Struct>& %measure_in%
%$$
It has the exact same specifications as
$cref/measure_in/optimize/measure_in/$$ in the $code optimize$$ routine.

$head grid$$
$index grid$$
The return value $icode grid$$ has prototype
$codei%
	dismod_pde::pair2index %grid%
%$$
see $cref/pair2index/$$.
There is an age cohort grid that surrounds the measurement values
by a spacing of $icode delta_age$$ and $icode delta_cohort$$.
In addition, the age grid is expanded to start at age zero.

$end
*/

# include <dismod_pde/pair2index.hpp>

DISMOD4_BEGIN_NAMESPACE
/*!
\file create_grid.hpp
Include file for \ref create_grid().
*/

pair2index create_grid(
	double                               delta_age       ,
	double                               delta_cohort    ,
	const vector<Measure_Input_Struct>&  measure_in
);

DISMOD4_END_NAMESPACE
# endif
