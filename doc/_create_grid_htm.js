var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'dismod_pde.htm',
'cpp.htm',
'create_grid.htm'
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
'optimize.htm',
'create_grid.htm',
'pair2index.htm',
'csv_read.htm',
'str_char_vec.htm',
'atof_char_vec.htm'
];
var list_current0 = [
'create_grid.htm#Syntax',
'create_grid.htm#delta_age',
'create_grid.htm#delta_cohort',
'create_grid.htm#measure_in',
'create_grid.htm#grid'
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
