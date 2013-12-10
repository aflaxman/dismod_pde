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
'get_started.sh.htm'
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
var list_down0 = [
'get_started.py.htm'
];
var list_current0 = [
'get_started.sh.htm#Syntax',
'get_started.sh.htm#Purpose',
'get_started.sh.htm#python_path',
'get_started.sh.htm#dismod_pde_path',
'get_started.sh.htm#parameter.csv',
'get_started.sh.htm#effect_in.csv',
'get_started.sh.htm#measure_in.csv',
'get_started.sh.htm#template.csv',
'get_started.sh.htm#sfun_in.csv',
'get_started.sh.htm#Running dismod_pde_csv',
'get_started.sh.htm#effect_out.csv',
'get_started.sh.htm#effect_out.csv.Check effect_out.csv',
'get_started.sh.htm#measure_out.csv',
'get_started.sh.htm#measure_out.csv.Check measure_out.csv',
'get_started.sh.htm#sfun_out.csv',
'get_started.sh.htm#sfun_out.csv.Check sfun_out.csv',
'get_started.sh.htm#Final Message'
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
