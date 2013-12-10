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
'atof_char_vec.htm'
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
var list_down0 = [
'atof_char_vec_tst.cpp.htm'
];
var list_current0 = [
'atof_char_vec.htm#Syntax',
'atof_char_vec.htm#char_vec',
'atof_char_vec.htm#sub_vec',
'atof_char_vec.htm#line',
'atof_char_vec.htm#file',
'atof_char_vec.htm#dbl',
'atof_char_vec.htm#nan',
'atof_char_vec.htm#inf',
'atof_char_vec.htm#Example'
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
