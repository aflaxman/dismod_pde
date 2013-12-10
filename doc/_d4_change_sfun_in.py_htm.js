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
'd4_change_sfun_in.py.htm'
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
var list_down0 = [
'change_sfun_in.sh.htm'
];
var list_current0 = [
'd4_change_sfun_in.py.htm#Syntax',
'd4_change_sfun_in.py.htm#Expressions',
'd4_change_sfun_in.py.htm#input.csv',
'd4_change_sfun_in.py.htm#output.csv',
'd4_change_sfun_in.py.htm#sfun',
'd4_change_sfun_in.py.htm#comment',
'd4_change_sfun_in.py.htm#lower',
'd4_change_sfun_in.py.htm#initial',
'd4_change_sfun_in.py.htm#upper',
'd4_change_sfun_in.py.htm#name',
'd4_change_sfun_in.py.htm#zeta',
'd4_change_sfun_in.py.htm#sigma',
'd4_change_sfun_in.py.htm#mean',
'd4_change_sfun_in.py.htm#order',
'd4_change_sfun_in.py.htm#Example'
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
