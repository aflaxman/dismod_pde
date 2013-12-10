#! /bin/bash -e
web_page="http://www.seanet.com/~bradbell"
# --------------------------------------------------------------------------
if [ $0 != "bin/get_omhelp.sh" ]
then
	echo "bin/get_omhelp.sh: must be executed from its parent directory"
	exit 1
fi
if [ "$1" == "" ]
then
	echo "usage: bin/get_omhelp.sh install_prefix"
	exit 1
fi
prefix="$1"
#
if [ ! -e external ]
then
	echo "mkdir external"
	mkdir external
fi
echo "cd external"
cd external
#
if [ -e OMhelp.unix.tar.gz ]
then
	echo "Using existing external/OMhelp.unix.tar.gz"
else
	echo "wget $web_page/OMhelp.unix.tar.gz"
	wget "$web_page/OMhelp.unix.tar.gz"
fi
#
version=`ls | sed -n '/omhelp-/p'`
if [ "$version" != "" ]
then
	echo "Using existing external/$version"
else
	echo "tar -xvzf OMhelp.unix.tar.gz"
	tar -xvzf OMhelp.unix.tar.gz
	#
	version=`ls | sed -n '/omhelp-/p'`
fi
#
echo "cd $version"
cd  $version
#
echo "./configure --prefix=$prefix"
./configure --prefix=$prefix
#
# remove old version of main.c in-case prefix has changed
# (this is a kludge and should be taken care of by OMhelp install process)
echo "rm src/main.c"
rm src/main.c
#
echo "make install"
make install
