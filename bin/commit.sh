#! /bin/bash -e
#
# replacement text for this commit
cat << EOF > bin/commit.$$
This is a template file for making commits to the dismod_pde repository.
Lines with no 'at' characters, are general comments not connected to 
a specific file. Lines containing an 'at' character are "file name" 
followed by comment. Lines before the first 'at' character are preserved
during
	bin/commit.sh edit 
for example this entire paragraph is preserved.

bin/commit.sh@ For this example, bin/commit.sh is the only file committed.
EOF
# -----------------------------------------------------------------------------
if [ $0 != "bin/commit.sh" ]
then
	echo "bin/commit.sh: must be executed from its parent directory"
	rm bin/commit.$$
	exit 1
fi
# -----------------------------------------------------------------------
if [ "$1" != 'list' ] && [ "$1" != 'edit' ] && [ "$1" != 'run' ]
then
cat << EOF
usage: bin/commit.sh list
       bin/commit.sh edit
       bin/commit.sh run

list:
output a list of the files that have changes svn knows about.

edit:
Edit the file list of files at the top of bin/commit.sh to be the same as 
	bin/commit.sh list
would output. In addition, it displays the changes to bin/commit.sh. This 
will include the new files in the list since the last edit of bin/commit.sh. 
You should then edit bin/commit.sh by hand, to add comments about the changes 
before running the command
	bin/commit.sh run

run:
commits changes to the list of files in bin/commit.sh 
(provided that you reply y to the [y/n] prompt that bin/commit.sh generates).
The file bin/commit.sh cannot be commited this way; use
	svn commit -m "your log message" bin/commit.sh
to commit this file.
EOF
	rm bin/commit.$$
	exit 1
fi
# -----------------------------------------------------------------------
# check for abort do to unknown files
unknown=`svn status | sed -n \
	-e '/[/ ]junk\./d'  \
	-e '/[/]test_one.cpp/d' \
	-e '/[/]test_one.sh/d' \
	-e '/^[?].*\.am$/p'  \
	-e '/^[?].*\.cpp$/p'  \
	-e '/^[?].*\.hpp$/p'  \
	-e '/^[?].*\.in$/p'  \
	-e '/^[?].*\.omh$/p'  \
	-e '/^[?].*\.sh$/p'   | sed -e 's/^[?]//'`
msg="aborting because the following files are unknown to svn"
print_msg="no"
for file in $unknown
do
	if [ ! -e "$file.in" ]
	then
		if [ "$print_msg" == "no" ]
		then
			echo "bin/commit.sh: $msg"
			print_msg="yes"
		fi
		echo $file
	fi
done
if [ "$print_msg" == "yes" ]
then
	rm bin/commit.$$
	exit 1
fi
if [ "$1" == 'list' ] || [ "$1" == 'edit' ]
then
	# -------------------------------------------------
	# list of files that changed
	svn status | sed -n -e '/^[ADMRC] /p' | \
		sed -e 's/^[ADMRC] [+ ]*//' \
			-e '/^bin\/commit.sh$/d' | \
		sort -u > bin/commit.$$
	# -------------------------------------------------
	if [ "$1" == 'list' ]
	then
		cat bin/commit.$$
		rm bin/commit.$$
		exit 0
	fi
	#
	#
	echo "mv bin/commit.sh bin/commit.sh.old"
	mv bin/commit.sh bin/commit.sh.old
	#
	echo "creating new bin/commit.sh"
	sed -n -e '1,/@/p' bin/commit.sh.old | sed -e '/@/d' > bin/commit.sh
	sed bin/commit.$$ -e 's/$/@/'                       >> bin/commit.sh
	sed -n -e '/^EOF/,$p' bin/commit.sh.old             >> bin/commit.sh
	#
	echo "------------------------------------"
	echo "diff bin/commit.sh.old bin/commit.sh"
	if diff    bin/commit.sh.old bin/commit.sh
	then
		echo "bin/commit.sh edit: no changes to bin/commit.sh"
	fi
	echo "------------------------------------"
     echo "chmod +x bin/commit.sh"
           chmod +x bin/commit.sh
	#
	rm bin/commit.$$
	exit 0
fi
# -----------------------------------------------------------------------
list=`sed -e '/@/! d' -e 's/@.*//' bin/commit.$$`
msg=`sed -e '/@ *$/d' -e 's|.*/\([^/]*@\)|\1|' -e 's|@|:|' bin/commit.$$`
if (echo $list | grep '^bin/commit.sh$' > /dev/null)
then
	echo "bin/commit.sh: cannot be used to commit changes to itself."
	echo "remove it from the list of files in bin/commit.sh"
	exit 1
fi
rm bin/commit.$$
echo "svn commit -m \""
echo "$msg"
echo "\" \\"
echo "$list"
read -p "is this ok [y/n] ?" response
if [ "$response" != "y" ]
then
	exit 1
fi
#
if ! svn commit -m "$msg" $list
then
	echo "bin/commit.sh: commit failed"
	exit 1
fi
#
echo "mv bin/commit.sh bin/commit.sh.old"
mv bin/commit.sh bin/commit.sh.old
#
echo "svn revert bin/commit.sh"
svn revert bin/commit.sh
