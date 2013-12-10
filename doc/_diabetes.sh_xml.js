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
'diabetes.sh.xml'
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
'diabetes.sh.xml#Syntax',
'diabetes.sh.xml#Purpose',
'diabetes.sh.xml#python_path',
'diabetes.sh.xml#dismod_pde_path',
'diabetes.sh.xml#grid function',
'diabetes.sh.xml#Global Parameters',
'diabetes.sh.xml#parameter.csv',
'diabetes.sh.xml#effect_in.csv',
'diabetes.sh.xml#measure_in.csv',
'diabetes.sh.xml#template.csv',
'diabetes.sh.xml#sfun_in.csv',
'diabetes.sh.xml#Running dismod_pde_csv',
'diabetes.sh.xml#prevalence_residual',
'diabetes.sh.xml#sfun_grid.csv',
'diabetes.sh.xml#sfun_der.csv',
'diabetes.sh.xml#prevalence_compare',
'diabetes.sh.xml#Prevalence Contour Plot',
'diabetes.sh.xml#Rate Contour Plots',
'diabetes.sh.xml#Rate Contour Plots.omega_estimate',
'diabetes.sh.xml#Rate Contour Plots.chi_estimate',
'diabetes.sh.xml#Rate Contour Plots.iota_estimate',
'diabetes.sh.xml#Final Message'
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
