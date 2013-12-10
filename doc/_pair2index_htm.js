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
'pair2index.htm'
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
'pair2index.cpp.htm'
];
var list_current0 = [
'pair2index.htm#Syntax',
'pair2index.htm#Purpose',
'pair2index.htm#age',
'pair2index.htm#cohort',
'pair2index.htm#time_lower',
'pair2index.htm#time_upper',
'pair2index.htm#pair2index',
'pair2index.htm#grid.age',
'pair2index.htm#grid.cohort',
'pair2index.htm#grid.time_lower',
'pair2index.htm#grid.time_upper',
'pair2index.htm#grid.size',
'pair2index.htm#Notation',
'pair2index.htm#Notation.kappa',
'pair2index.htm#Notation.Computational Grid',
'pair2index.htm#age_index',
'pair2index.htm#cohort_index',
'pair2index.htm#valid',
'pair2index.htm#grid_pack',
'pair2index.htm#age_unpack',
'pair2index.htm#cohort_unpack',
'pair2index.htm#grid_index',
'pair2index.htm#Example'
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
