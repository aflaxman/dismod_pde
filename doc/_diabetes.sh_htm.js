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
'diabetes.sh.htm'
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
'diabetes.sh.htm#Syntax',
'diabetes.sh.htm#Purpose',
'diabetes.sh.htm#python_path',
'diabetes.sh.htm#dismod_pde_path',
'diabetes.sh.htm#grid function',
'diabetes.sh.htm#Global Parameters',
'diabetes.sh.htm#parameter.csv',
'diabetes.sh.htm#effect_in.csv',
'diabetes.sh.htm#measure_in.csv',
'diabetes.sh.htm#template.csv',
'diabetes.sh.htm#sfun_in.csv',
'diabetes.sh.htm#Running dismod_pde_csv',
'diabetes.sh.htm#prevalence_residual',
'diabetes.sh.htm#sfun_grid.csv',
'diabetes.sh.htm#sfun_der.csv',
'diabetes.sh.htm#prevalence_compare',
'diabetes.sh.htm#Prevalence Contour Plot',
'diabetes.sh.htm#Rate Contour Plots',
'diabetes.sh.htm#Rate Contour Plots.omega_estimate',
'diabetes.sh.htm#Rate Contour Plots.chi_estimate',
'diabetes.sh.htm#Rate Contour Plots.iota_estimate',
'diabetes.sh.htm#Final Message'
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
