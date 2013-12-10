# include <iostream>

namespace {
	// function that runs one test
	int run_ok_count    = 0;
	int run_error_count = 0;
	bool run(bool TestOk(void), std::string name)
	{    bool ok	            = true;
		std::streamsize width =  20;
		std::cout.width( width );
		std::cout.setf( std::ios_base::left );
		std::cout << name;
		//
		ok &= name.size() < size_t(width);
		ok &= TestOk();
		if( ok )
		{    std::cout << "OK" << std::endl;
		     run_ok_count++;
		}
		else
		{    std::cout << "Error" << std::endl;
		     run_error_count++;
		}
		return ok;
	}
}

extern bool atof_char_vec_tst(void);
extern bool csv_read_tst(void);
extern bool optimize_xam(void);
extern bool pair2index(void);
extern bool str_char_vec_tst(void);

int main(void)
{	
     // check for errors
     using std::cout;
     using std::endl;

	run( atof_char_vec_tst, "atof_char_vec_tst" );
	run( csv_read_tst,      "csv_read_tst"      );
	run( pair2index,        "pair2index"        );
	run( optimize_xam,      "optimize_xam"      );
	run( str_char_vec_tst,  "str_char_vec_tst"  );

     if( run_error_count == 0 )
          cout << "All " << run_ok_count << " tests passed." << endl;
     else cout << run_error_count << " tests failed." << endl;

     return run_error_count;
}
