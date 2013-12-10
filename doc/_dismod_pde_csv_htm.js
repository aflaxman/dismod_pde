var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'dismod_pde.htm',
'csv.htm',
'dismod_pde_csv.htm'
];
var list_down2 = [
'whats_new.htm',
'install.htm',
'csv.htm',
'cpp.htm',
'example.htm',
'wish_list.htm',
'bug_list.htm'
];
var list_down1 = [
'dismod_pde_csv.htm',
'python_csv.htm'
];
var list_current0 = [
'dismod_pde_csv.htm#Syntax',
'dismod_pde_csv.htm#Purpose',
'dismod_pde_csv.htm#CSV Files',
'dismod_pde_csv.htm#Infinity',
'dismod_pde_csv.htm#parameter.csv',
'dismod_pde_csv.htm#parameter.csv.age_grid',
'dismod_pde_csv.htm#parameter.csv.cohort_grid',
'dismod_pde_csv.htm#parameter.csv.debug_output',
'dismod_pde_csv.htm#measure_in.csv',
'dismod_pde_csv.htm#measure_in.csv.ell',
'dismod_pde_csv.htm#measure_in.csv.integrand',
'dismod_pde_csv.htm#measure_in.csv.age_lower, age_upper',
'dismod_pde_csv.htm#measure_in.csv.time_lower, time_upper',
'dismod_pde_csv.htm#measure_in.csv.meas_like',
'dismod_pde_csv.htm#measure_in.csv.meas_zeta',
'dismod_pde_csv.htm#measure_in.csv.meas_sigma',
'dismod_pde_csv.htm#measure_in.csv.meas_value',
'dismod_pde_csv.htm#measure_in.csv.Covariates',
'dismod_pde_csv.htm#measure_in.csv.Comments',
'dismod_pde_csv.htm#sfun_in.csv',
'dismod_pde_csv.htm#sfun_in.csv.Output Template',
'dismod_pde_csv.htm#sfun_in.csv.Input File',
'dismod_pde_csv.htm#sfun_in.csv.function',
'dismod_pde_csv.htm#sfun_in.csv.name',
'dismod_pde_csv.htm#sfun_in.csv.zeta',
'dismod_pde_csv.htm#sfun_in.csv.sigma',
'dismod_pde_csv.htm#sfun_in.csv.order',
'dismod_pde_csv.htm#effect_in.csv',
'dismod_pde_csv.htm#effect_in.csv.integrand',
'dismod_pde_csv.htm#effect_in.csv.covariate',
'dismod_pde_csv.htm#effect_in.csv.lower',
'dismod_pde_csv.htm#effect_in.csv.initial',
'dismod_pde_csv.htm#effect_in.csv.upper',
'dismod_pde_csv.htm#sfun_out.csv',
'dismod_pde_csv.htm#sfun_out.csv.Scaled Residuals',
'dismod_pde_csv.htm#measure_out.csv',
'dismod_pde_csv.htm#effect_out.csv',
'dismod_pde_csv.htm#effect_out.csv.integrand',
'dismod_pde_csv.htm#effect_out.csv.covariate',
'dismod_pde_csv.htm#effect_out.csv.effect',
'dismod_pde_csv.htm#Example'
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
