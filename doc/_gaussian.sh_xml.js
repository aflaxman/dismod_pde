var list_across0 = [
'_contents_xml.htm',
'_reference.xml',
'_index.xml',
'_search_xml.htm',
'_external.xml'
];
var list_up0 = [
'dismod_pde.xml',
'example.xml',
'gaussian.sh.xml'
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
'get_started.sh.xml',
'diabetes.sh.xml',
'gaussian.sh.xml'
];
var list_current0 = [
'gaussian.sh.xml#Syntax',
'gaussian.sh.xml#Purpose',
'gaussian.sh.xml#grid_step',
'gaussian.sh.xml#prevalence_cv',
'gaussian.sh.xml#python_path',
'gaussian.sh.xml#dismod_pde_path',
'gaussian.sh.xml#grid function',
'gaussian.sh.xml#Global Parameters',
'gaussian.sh.xml#parameter.csv',
'gaussian.sh.xml#effect_in.csv',
'gaussian.sh.xml#measure_in.csv',
'gaussian.sh.xml#template.csv',
'gaussian.sh.xml#sfun_in.csv',
'gaussian.sh.xml#Running dismod_pde_csv',
'gaussian.sh.xml#prevalence_residual',
'gaussian.sh.xml#sfun_grid.csv',
'gaussian.sh.xml#sfun_der.csv',
'gaussian.sh.xml#Prevalence Contour Plot',
'gaussian.sh.xml#Rate Contour Plots',
'gaussian.sh.xml#Rate Contour Plots.omega_estimate',
'gaussian.sh.xml#Rate Contour Plots.omega_true',
'gaussian.sh.xml#Rate Contour Plots.chi_estimate',
'gaussian.sh.xml#Rate Contour Plots.chi_true',
'gaussian.sh.xml#Rate Contour Plots.iota_estimate',
'gaussian.sh.xml#Rate Contour Plots.iota_true',
'gaussian.sh.xml#Check Solution',
'gaussian.sh.xml#Final Message'
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
