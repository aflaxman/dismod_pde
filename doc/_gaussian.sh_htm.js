var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'dismod_pde.htm',
'example.htm',
'gaussian.sh.htm'
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
'get_started.sh.htm',
'diabetes.sh.htm',
'gaussian.sh.htm'
];
var list_current0 = [
'gaussian.sh.htm#Syntax',
'gaussian.sh.htm#Purpose',
'gaussian.sh.htm#grid_step',
'gaussian.sh.htm#prevalence_cv',
'gaussian.sh.htm#python_path',
'gaussian.sh.htm#dismod_pde_path',
'gaussian.sh.htm#grid function',
'gaussian.sh.htm#Global Parameters',
'gaussian.sh.htm#parameter.csv',
'gaussian.sh.htm#effect_in.csv',
'gaussian.sh.htm#measure_in.csv',
'gaussian.sh.htm#template.csv',
'gaussian.sh.htm#sfun_in.csv',
'gaussian.sh.htm#Running dismod_pde_csv',
'gaussian.sh.htm#prevalence_residual',
'gaussian.sh.htm#sfun_grid.csv',
'gaussian.sh.htm#sfun_der.csv',
'gaussian.sh.htm#Prevalence Contour Plot',
'gaussian.sh.htm#Rate Contour Plots',
'gaussian.sh.htm#Rate Contour Plots.omega_estimate',
'gaussian.sh.htm#Rate Contour Plots.omega_true',
'gaussian.sh.htm#Rate Contour Plots.chi_estimate',
'gaussian.sh.htm#Rate Contour Plots.chi_true',
'gaussian.sh.htm#Rate Contour Plots.iota_estimate',
'gaussian.sh.htm#Rate Contour Plots.iota_true',
'gaussian.sh.htm#Check Solution',
'gaussian.sh.htm#Final Message'
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
