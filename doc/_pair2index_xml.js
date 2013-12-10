var list_across0 = [
'_contents_xml.htm',
'_reference.xml',
'_index.xml',
'_search_xml.htm',
'_external.xml'
];
var list_up0 = [
'dismod_pde.xml',
'cpp.xml',
'pair2index.xml'
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
'optimize.xml',
'create_grid.xml',
'pair2index.xml',
'csv_read.xml',
'str_char_vec.xml',
'atof_char_vec.xml'
];
var list_down0 = [
'pair2index.cpp.xml'
];
var list_current0 = [
'pair2index.xml#Syntax',
'pair2index.xml#Purpose',
'pair2index.xml#age',
'pair2index.xml#cohort',
'pair2index.xml#time_lower',
'pair2index.xml#time_upper',
'pair2index.xml#pair2index',
'pair2index.xml#grid.age',
'pair2index.xml#grid.cohort',
'pair2index.xml#grid.time_lower',
'pair2index.xml#grid.time_upper',
'pair2index.xml#grid.size',
'pair2index.xml#Notation',
'pair2index.xml#Notation.kappa',
'pair2index.xml#Notation.Computational Grid',
'pair2index.xml#age_index',
'pair2index.xml#cohort_index',
'pair2index.xml#valid',
'pair2index.xml#grid_pack',
'pair2index.xml#age_unpack',
'pair2index.xml#cohort_unpack',
'pair2index.xml#grid_index',
'pair2index.xml#Example'
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
