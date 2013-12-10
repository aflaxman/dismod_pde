/*
$begin pair2index.cpp$$
$spell
	pde
	Dismod
	bool
$$

$section Example and Test of dismod_pde pair2index Procedure$$
$index test, pair2index$$
$index example, pair2index$$
$index grid, example$$ 
$index example grid$$


$head Syntax$$
$icode%ok% = pair2index()%$$

$head ok$$
The return value $icode ok$$ has prototype
$codei%
	bool %ok% 
%$$
If true, the $cref pair2index$$ procedure returned a successful status; i.e.,
estimated values agreed with the simulated values.

$code
$verbatim%example/pair2index.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/

// BEGIN C++
# include <dismod_pde/dismod_pde.hpp>

bool pair2index(void)
{	// initialize return value
	bool ok = true;

	// Example in Lexis Diagram in dismod_pde Numerical Methods Paper
	double time_lower = 1950.;
	double time_upper = 2000.;
	size_t j, J = 3;
	size_t k, K = 5;
	dismod_pde::vector<double> age(J), cohort(K);
	for(j = 0; j < J; j++)
		age[j] = 15 + j * 25.;
	for(k = 0; k < K; k++)
		cohort[k] = 1880 + k * 30.; 
	dismod_pde::pair2index grid(age, cohort, time_lower, time_upper);

	// Enumerate all the valid grid points in Lexis Diagram in paper
	size_t n_grid = 0;
	size_t age_index, cohort_index, grid_pack;
	size_t age_unpack, cohort_unpack, grid_index;
	bool   valid;
	age_index = 0;
	for(k = 1; k < 5; k++)
	{	cohort_index = k;	
		valid = grid.pack(age_index, cohort_index, grid_pack); 
		grid_index = grid_pack;
		grid.unpack(age_unpack, cohort_unpack, grid_index);
		ok   &= age_unpack == age_index;
		ok   &= cohort_unpack == cohort_index;
		ok   &= valid;
		n_grid++;
	}
	age_index = 1;
	for(k = 1; k < 4; k++)
	{	cohort_index = k;	
		valid = grid.pack(age_index, cohort_index, grid_pack); 
		grid_index = grid_pack;
		grid.unpack(age_unpack, cohort_unpack, grid_index);
		ok   &= age_unpack == age_index;
		ok   &= cohort_unpack == cohort_index;
		ok   &= valid;
		n_grid++;
	}
	age_index = 2;
	for(k = 0; k < 3; k++)
	{	cohort_index = k;	
		valid = grid.pack(age_index, cohort_index, grid_pack); 
		grid_index = grid_pack;
		grid.unpack(age_unpack, cohort_unpack, grid_index);
		ok   &= age_unpack == age_index;
		ok   &= cohort_unpack == cohort_index;
		ok   &= valid;
		n_grid++;
	}
	// check that these are all the valid grid points
	ok &= n_grid == grid.size();

	return ok;
}
// END C++
