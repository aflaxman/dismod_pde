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
'd4_plot.py.xml'
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
'd4_plot.py.xml#Syntax',
'd4_plot.py.xml#csv_file',
'd4_plot.py.xml#plot_file',
'd4_plot.py.xml#title',
'd4_plot.py.xml#--line, --points',
'd4_plot.py.xml#--line, --points.x_col',
'd4_plot.py.xml#--line, --points.y_col',
'd4_plot.py.xml#--line, --points.symbol',
'd4_plot.py.xml#--line, --points.alpha',
'd4_plot.py.xml#--axis',
'd4_plot.py.xml#--string',
'd4_plot.py.xml#--string.s_col',
'd4_plot.py.xml#--string.s_value',
'd4_plot.py.xml#--numeric',
'd4_plot.py.xml#--numeric.e_col',
'd4_plot.py.xml#--numeric.expr',
'd4_plot.py.xml#--connect',
'd4_plot.py.xml#--connect.c_col',
'd4_plot.py.xml#--connect.c_limit_j',
'd4_plot.py.xml#--logx',
'd4_plot.py.xml#--logy'
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
