// ------------------------------------------------------- 
// Copyright (C) Bradley M. Bell 2003, All rights reserved 
// ------------------------------------------------------- 
Keyword = 
[
"dismod_pde  dismod_pde: Estimation of Disease Mortality as a Stochastic Function of Age and Cohort   mathml display latex top tree",
"whats_new  Changes and Additions to dismod_pde   changelog api feature bug fixes bugs",
"install  Installing dismod_pde   requirements download external packages externals documentation build test",
"csv  The CSV File Interface to dismod_pde   specification",
"dismod_pde_csv  dismod_pde Command Line Program   infinity parameter.csv grid age cohort measure_in.csv integrand limits upper lower time likelihood name zeta sigma covariates comment sfun_in.csv template input function column field order effect_in.csv x_ initial sfun_out.csv scaled residual measure_out.csv effect_out.csv measurement estimate",
"python_csv  dismod_pde Python CSV Helper Scripts  ",
"d4_bubble.py  Bubble Plots of Values in Csv File  ",
"d4_change_sfun_in.py  Change The dismod_pde Stochastic Function Input File   sfun_in.csv template.csv initial lower upper name zeta sigma mean order example",
"change_sfun_in.sh  d4_change_sfun_in.py: Example and Test  ",
"d4_compare_sfun.py  Compare sfun_out.csv and d4_simulate.py Solution File  ",
"d4_contour.py  Contour Plots of Values in Csv File  ",
"d4_cumulative.py  Plot the Cumulative Distribution for a Csv File Column  ",
"d4_plot.py  Line and Point Plots of CSV Files  ",
"d4_sfun_der.py  Create Derived Stochastic Function Output Files  ",
"d4_sfun_grid.py  Convert From Age-Cohort Grid to Age-Time Rectangular Grid   example",
"sfun_grid.sh  d4_sfun_grid.py: Example and Test  ",
"d4_simulate.py  Simulated dismod_pde Data and Solution Files  ",
"cpp  The C++ Interface to dismod_pde  ",
"optimize  Optimization of dismod_pde Objective Function   stochastic_enum likelihood_enum g value covariate x",
"ipopt_trace  Description of Ipopt Tracing Output  ",
"optimize_xam.cpp  Example and Test of dismod_pde C++ Optimize Procedure   n_age n_cohort likelihood",
"dismoditis.cpp  A dismod_pde C++ Optimize Example and Test  ",
"dismoditis.sh  Create C++ Files Corresponding to Dismod Data and Expected Results   example structure",
"create_grid  Create an Age-Cohort Grid For a Measurement Set   delta measure_in",
"pair2index  Convert Between Age-Cohort Pairs and a Single Grid Index   time min max grid.age grid.cohort grid.time_lower grid.time_upper grid.size notation kappa(j l) u) valid age_index cohort_index grid_pack age_unpack cohort_unpack grid_index to",
"pair2index.cpp  Example and Test of dismod_pde pair2index Procedure   grid",
"csv_read  Read an Entire CSV File Into Memory  ",
"csv_read_tst.cpp  Example and Test of dismod_pde csv_read Procedure  ",
"str_char_vec  Convert a Sub-vector of a Character Vector to a String  ",
"str_char_vec_tst.cpp  Example and Test of Conversion from Ascii to String  ",
"atof_char_vec  Convert a Sub-vector of a Character Vector to Double  ",
"atof_char_vec_tst.cpp  Example and Test of Conversion from Ascii to Double  ",
"example  dismod_pde Examples   dismod_pde_csv",
"get_started.sh  get_started.sh: Example and Test of dismod_pde_csv  ",
"get_started.py  get_started.py: Example and Test of dismod_pde_csv  ",
"diabetes.sh  A Diabetes Incidence Estimation Example  ",
"gaussian.sh  Simulate and Fit Where Incidence is Gaussian in Age an Cohort   simulated",
"wish_list  Wish List For Improvements to dismod_pde   which lower age limit age_lower measurement model integral approximation integrand estimate standard deviation accuracy of variance information matrix posterior distribution covariates",
"bug_list  List of Known Bugs  "
]

var MaxList = 100;
var Choice  = "";
var Nstring = -1;
var Nkeyword = Keyword.length;
Initialize();

function Initialize()
{
	var i;
	var line;
	for(i = 0; (i < Nkeyword) && (i < MaxList) ; i++)
	{
		line       = Keyword[i].split(/\s+/)
		line[0]    = line[0].toUpperCase();
		line       = line.join(" ");
		Keyword[i] = line;
	}
	UpdateList();
	document.search.string.focus();
}
function UpdateList(event)
{
	key = 0;
	if( window.event )
		key = window.event.keyCode;
	else if( event )
		key = event.which;
	if( key == 13 )
	{	Choose();
		return;
	}
	var string  = document.search.string.value;
	if( Nstring == string.length )
		return;
	Nstring     = string.length;

	var word    = string.match(/\S+/g);
	var nword   = 0;
	if(word != null )
		nword   = word.length;

	var pattern = new Array(nword);
	for(var j = 0; j < nword; j++)
		pattern[j] = new RegExp(word[j], "i");

	var nlist = 0;
	var list  = "";
	Choice    = "";

	for(i = 0; (i < Nkeyword) && (nlist < MaxList) ; i++)
	{
		var match = true;
		for(j = 0; j < nword; j++)
			match = match && pattern[j].test(Keyword[i]);

		if( match )
		{
			line     = Keyword[i].split(/\s+/);

			if( Choice == "" )
				Choice = line[0];

			line  = line.join(" ");
			list  = list + line + "\n";
			nlist = nlist + 1;
		}
	}
	document.search.choice.value  = Choice.toLowerCase();
	document.search.list.value    = list;
}
function Choose()
{
parent.location = document.search.choice.value.toLowerCase() + ".htm";
}
