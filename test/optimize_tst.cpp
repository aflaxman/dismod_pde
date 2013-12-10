# include <iostream>
# include <iomanip>
# include <dismod_pde/dismod_pde.hpp>
# include <cmath>

// case in the example directory
extern bool optimize_xam(
     size_t n_age                                         ,
     size_t n_cohort                                      ,
     dismod_pde::vector<dismod_pde::Likelihood_Enum> likelihood
);

bool optimize_tst(void)
{	bool all_ok = true;
	using std::cout;
	using std::endl;
	using std::setw;
	size_t i, j, k;

	dismod_pde::vector<const char*> meas_like(3);
	meas_like[ dismod_pde::laplace_enum ]  = "L";
	meas_like[ dismod_pde::gaussian_enum ] = "G";

	dismod_pde::vector<dismod_pde::Likelihood_Enum> likelihood(6);
	for(i = 0; i < 6; i++)
		likelihood[i] = dismod_pde::laplace_enum;

	//
	size_t n_age      = 4;
	size_t n_cohort   = 7;
	size_t i_like     = 0;
	cout << setw(27) << "n_age" << setw(10) << "n_cohort";
	cout << setw(12) <<  "likelihoods" << endl;
	for(i = 0; i < 3; i++)
	{	bool ok;
		if( i == 0 )
			j = 3;
		else	j = 2;

		while(j--)
		{
			// ok  = optimize_xam(
		 	ok  = optimize_xam(n_age, n_cohort, likelihood);
			all_ok &= ok;
			//
			if( ok )
				cout << "optimize:      OK";
			else	cout << "optimize:   Error";
			cout << setw(10) << n_age << setw(10) << n_cohort;
			cout << "      ";
			for(k = 0; k < 6; k++)
				cout << meas_like[ likelihood[k] ];
			cout << endl;
			// next problems parameters
			if( i_like < 6 )
				likelihood[i_like++] = dismod_pde::gaussian_enum;
			if( j % 2 == 0 )
				n_age++;
			else	n_cohort--;
		}
	}
	return all_ok;
}
