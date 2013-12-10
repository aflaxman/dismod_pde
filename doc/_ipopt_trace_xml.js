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
'optimize.xml',
'ipopt_trace.xml'
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
'optimize.xml',
'create_grid.xml',
'pair2index.xml',
'csv_read.xml',
'str_char_vec.xml',
'atof_char_vec.xml'
];
var list_down1 = [
'ipopt_trace.xml',
'optimize_xam.cpp.xml',
'dismoditis.cpp.xml'
];
var list_current0 = [
'ipopt_trace.xml#iter',
'ipopt_trace.xml#objective',
'ipopt_trace.xml#inf_pr',
'ipopt_trace.xml#inf_du',
'ipopt_trace.xml#lg(mu)',
'ipopt_trace.xml#||d||',
'ipopt_trace.xml#lg(rg)',
'ipopt_trace.xml#alpha_du',
'ipopt_trace.xml#alpha_pr',
'ipopt_trace.xml#ls',
'ipopt_trace.xml#Reference'
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
