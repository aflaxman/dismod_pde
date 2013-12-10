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
'd4_sfun_grid.py.xml'
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
var list_down0 = [
'sfun_grid.sh.xml'
];
var list_current0 = [
'd4_sfun_grid.py.xml#Syntax',
'd4_sfun_grid.py.xml#Purpose',
'd4_sfun_grid.py.xml#sfun_out.csv',
'd4_sfun_grid.py.xml#sfun_out.csv.age vector',
'd4_sfun_grid.py.xml#sfun_out.csv.time bounds',
'd4_sfun_grid.py.xml#sfun_grid.csv',
'd4_sfun_grid.py.xml#time_start',
'd4_sfun_grid.py.xml#time_end',
'd4_sfun_grid.py.xml#n_time',
'd4_sfun_grid.py.xml#Example'
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
