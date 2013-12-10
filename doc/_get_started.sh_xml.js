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
'get_started.sh.xml'
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
var list_down0 = [
'get_started.py.xml'
];
var list_current0 = [
'get_started.sh.xml#Syntax',
'get_started.sh.xml#Purpose',
'get_started.sh.xml#python_path',
'get_started.sh.xml#dismod_pde_path',
'get_started.sh.xml#parameter.csv',
'get_started.sh.xml#effect_in.csv',
'get_started.sh.xml#measure_in.csv',
'get_started.sh.xml#template.csv',
'get_started.sh.xml#sfun_in.csv',
'get_started.sh.xml#Running dismod_pde_csv',
'get_started.sh.xml#effect_out.csv',
'get_started.sh.xml#effect_out.csv.Check effect_out.csv',
'get_started.sh.xml#measure_out.csv',
'get_started.sh.xml#measure_out.csv.Check measure_out.csv',
'get_started.sh.xml#sfun_out.csv',
'get_started.sh.xml#sfun_out.csv.Check sfun_out.csv',
'get_started.sh.xml#Final Message'
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
