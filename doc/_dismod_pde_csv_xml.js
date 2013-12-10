var list_across0 = [
'_contents_xml.htm',
'_reference.xml',
'_index.xml',
'_search_xml.htm',
'_external.xml'
];
var list_up0 = [
'dismod_pde.xml',
'csv.xml',
'dismod_pde_csv.xml'
];
var list_down2 = [
'whats_new.xml',
'install.xml',
'csv.xml',
'cpp.xml',
'example.xml',
'wish_list.xml',
'bug_list.xml'
];
var list_down1 = [
'dismod_pde_csv.xml',
'python_csv.xml'
];
var list_current0 = [
'dismod_pde_csv.xml#Syntax',
'dismod_pde_csv.xml#Purpose',
'dismod_pde_csv.xml#CSV Files',
'dismod_pde_csv.xml#Infinity',
'dismod_pde_csv.xml#parameter.csv',
'dismod_pde_csv.xml#parameter.csv.age_grid',
'dismod_pde_csv.xml#parameter.csv.cohort_grid',
'dismod_pde_csv.xml#parameter.csv.debug_output',
'dismod_pde_csv.xml#measure_in.csv',
'dismod_pde_csv.xml#measure_in.csv.ell',
'dismod_pde_csv.xml#measure_in.csv.integrand',
'dismod_pde_csv.xml#measure_in.csv.age_lower, age_upper',
'dismod_pde_csv.xml#measure_in.csv.time_lower, time_upper',
'dismod_pde_csv.xml#measure_in.csv.meas_like',
'dismod_pde_csv.xml#measure_in.csv.meas_zeta',
'dismod_pde_csv.xml#measure_in.csv.meas_sigma',
'dismod_pde_csv.xml#measure_in.csv.meas_value',
'dismod_pde_csv.xml#measure_in.csv.Covariates',
'dismod_pde_csv.xml#measure_in.csv.Comments',
'dismod_pde_csv.xml#sfun_in.csv',
'dismod_pde_csv.xml#sfun_in.csv.Output Template',
'dismod_pde_csv.xml#sfun_in.csv.Input File',
'dismod_pde_csv.xml#sfun_in.csv.function',
'dismod_pde_csv.xml#sfun_in.csv.name',
'dismod_pde_csv.xml#sfun_in.csv.zeta',
'dismod_pde_csv.xml#sfun_in.csv.sigma',
'dismod_pde_csv.xml#sfun_in.csv.order',
'dismod_pde_csv.xml#effect_in.csv',
'dismod_pde_csv.xml#effect_in.csv.integrand',
'dismod_pde_csv.xml#effect_in.csv.covariate',
'dismod_pde_csv.xml#effect_in.csv.lower',
'dismod_pde_csv.xml#effect_in.csv.initial',
'dismod_pde_csv.xml#effect_in.csv.upper',
'dismod_pde_csv.xml#sfun_out.csv',
'dismod_pde_csv.xml#sfun_out.csv.Scaled Residuals',
'dismod_pde_csv.xml#measure_out.csv',
'dismod_pde_csv.xml#effect_out.csv',
'dismod_pde_csv.xml#effect_out.csv.integrand',
'dismod_pde_csv.xml#effect_out.csv.covariate',
'dismod_pde_csv.xml#effect_out.csv.effect',
'dismod_pde_csv.xml#Example'
];
function choose_across0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_across0[index-1];
}
function choose_up0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_up0[index-1];
}
function choose_down2(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down2[index-1];
}
function choose_down1(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down1[index-1];
}
function choose_down0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down0[index-1];
}
function choose_current0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_current0[index-1];
}
