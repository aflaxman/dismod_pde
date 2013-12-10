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
'python_csv.xml',
'd4_simulate.py.xml'
];
var list_down3 = [
'whats_new.xml',
'install.xml',
'csv.xml',
'cpp.xml',
'example.xml',
'wish_list.xml',
'bug_list.xml'
];
var list_down2 = [
'dismod_pde_csv.xml',
'python_csv.xml'
];
var list_down1 = [
'd4_bubble.py.xml',
'd4_change_sfun_in.py.xml',
'd4_compare_sfun.py.xml',
'd4_contour.py.xml',
'd4_cumulative.py.xml',
'd4_plot.py.xml',
'd4_sfun_der.py.xml',
'd4_sfun_grid.py.xml',
'd4_simulate.py.xml'
];
var list_current0 = [
'd4_simulate.py.xml#Syntax',
'd4_simulate.py.xml#Purpose',
'd4_simulate.py.xml#Convention',
'd4_simulate.py.xml#measure_in.csv',
'd4_simulate.py.xml#solution.csv',
'd4_simulate.py.xml#age_init',
'd4_simulate.py.xml#age_step',
'd4_simulate.py.xml#age_min, age_max',
'd4_simulate.py.xml#age_bound',
'd4_simulate.py.xml#time_add',
'd4_simulate.py.xml#time_step',
'd4_simulate.py.xml#time_min, time_max',
'd4_simulate.py.xml#time_bound',
'd4_simulate.py.xml#expressions',
'd4_simulate.py.xml#expressions.iota_expr',
'd4_simulate.py.xml#expressions.rho_expr',
'd4_simulate.py.xml#expressions.chi_expr',
'd4_simulate.py.xml#expressions.omega_expr',
'd4_simulate.py.xml#measure',
'd4_simulate.py.xml#measure.Offset Log Transform',
'd4_simulate.py.xml#measure.integrand',
'd4_simulate.py.xml#measure.number',
'd4_simulate.py.xml#measure.like',
'd4_simulate.py.xml#measure.sim_zeta',
'd4_simulate.py.xml#measure.fit_zeta',
'd4_simulate.py.xml#measure.sim_cv',
'd4_simulate.py.xml#measure.fit_cv',
'd4_simulate.py.xml#Testing dismod_pde_csv',
'd4_simulate.py.xml#Testing optimize'
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
function choose_down3(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down3[index-1];
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
