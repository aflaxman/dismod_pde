
# include <iostream>

extern bool optimize_tst(void);
extern bool csv_header_tst(void);
extern bool dismoditis(void);

int main(void)
{	bool ok = true;
	
     // check for errors
     using std::cout;
     using std::endl;

	ok &= csv_header_tst();
	ok &= dismoditis();

	// put off till last because take a long time
	ok &= optimize_tst();
	if( ok )
		cout << "All tests passed." << endl;
	else	cout << "At least one test failed." << endl;

     return int( ! ok );
}
