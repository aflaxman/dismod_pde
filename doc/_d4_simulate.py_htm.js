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
'python_csv.htm',
'd4_simulate.py.htm'
];
var list_down3 = [
'whats_new.htm',
'install.htm',
'csv.htm',
'cpp.htm',
'example.htm',
'wish_list.htm',
'bug_list.htm'
];
var list_down2 = [
'dismod_pde_csv.htm',
'python_csv.htm'
];
var list_down1 = [
'd4_bubble.py.htm',
'd4_change_sfun_in.py.htm',
'd4_compare_sfun.py.htm',
'd4_contour.py.htm',
'd4_cumulative.py.htm',
'd4_plot.py.htm',
'd4_sfun_der.py.htm',
'd4_sfun_grid.py.htm',
'd4_simulate.py.htm'
];
var list_current0 = [
'd4_simulate.py.htm#Syntax',
'd4_simulate.py.htm#Purpose',
'd4_simulate.py.htm#Convention',
'd4_simulate.py.htm#measure_in.csv',
'd4_simulate.py.htm#solution.csv',
'd4_simulate.py.htm#age_init',
'd4_simulate.py.htm#age_step',
'd4_simulate.py.htm#age_min, age_max',
'd4_simulate.py.htm#age_bound',
'd4_simulate.py.htm#time_add',
'd4_simulate.py.htm#time_step',
'd4_simulate.py.htm#time_min, time_max',
'd4_simulate.py.htm#time_bound',
'd4_simulate.py.htm#expressions',
'd4_simulate.py.htm#expressions.iota_expr',
'd4_simulate.py.htm#expressions.rho_expr',
'd4_simulate.py.htm#expressions.chi_expr',
'd4_simulate.py.htm#expressions.omega_expr',
'd4_simulate.py.htm#measure',
'd4_simulate.py.htm#measure.Offset Log Transform',
'd4_simulate.py.htm#measure.integrand',
'd4_simulate.py.htm#measure.number',
'd4_simulate.py.htm#measure.like',
'd4_simulate.py.htm#measure.sim_zeta',
'd4_simulate.py.htm#measure.fit_zeta',
'd4_simulate.py.htm#measure.sim_cv',
'd4_simulate.py.htm#measure.fit_cv',
'd4_simulate.py.htm#Testing dismod_pde_csv',
'd4_simulate.py.htm#Testing optimize'
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
