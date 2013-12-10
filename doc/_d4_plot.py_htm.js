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
'd4_plot.py.htm'
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
'd4_plot.py.htm#Syntax',
'd4_plot.py.htm#csv_file',
'd4_plot.py.htm#plot_file',
'd4_plot.py.htm#title',
'd4_plot.py.htm#--line, --points',
'd4_plot.py.htm#--line, --points.x_col',
'd4_plot.py.htm#--line, --points.y_col',
'd4_plot.py.htm#--line, --points.symbol',
'd4_plot.py.htm#--line, --points.alpha',
'd4_plot.py.htm#--axis',
'd4_plot.py.htm#--string',
'd4_plot.py.htm#--string.s_col',
'd4_plot.py.htm#--string.s_value',
'd4_plot.py.htm#--numeric',
'd4_plot.py.htm#--numeric.e_col',
'd4_plot.py.htm#--numeric.expr',
'd4_plot.py.htm#--connect',
'd4_plot.py.htm#--connect.c_col',
'd4_plot.py.htm#--connect.c_limit_j',
'd4_plot.py.htm#--logx',
'd4_plot.py.htm#--logy'
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
