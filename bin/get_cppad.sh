#! /bin/bash -e
#
version="20131130"  # version of CppAD
revision="3016"     # corresponding revision of the repository
web_page="https://projects.coin-or.org/svn/CppAD/trunk"
# --------------------------------------------------------------------------
if [ $0 != "bin/get_cppad.sh" ]
then
	echo "bin/get_cppad.sh: must be executed from its parent directory"
	exit 1
fi
# exit immediately if a simple command exits with a non-zero status
if [ "$2" == "" ]
then
	echo "usage: bin/get_cppad.sh cppad_prefix ipopt_prefix"
	exit 1
fi
cppad_prefix="$1"
ipopt_prefix="$2"
#
if [ ! -e external ]
then
	echo "mkdir external"
	mkdir external
fi
echo "cd external"
cd external
#
if [ ! -e cppad-$version ]
then
	echo "svn checkout -r $revision $web_page cppad-$version"
	      svn checkout -r $revision $web_page cppad-$version
fi
#
echo "cd cppad-$version"
cd cppad-$version
#
if [ ! -e work ]
then
	echo "mkdir work"
	mkdir work
fi
echo "cd work"
cd work
#
echo "../configure --prefix=$cppad_prefix IPOPT_DIR=$ipopt_prefix"
../configure --prefix=$cppad_prefix IPOPT_DIR=$ipopt_prefix
#
echo "make"
make
#
echo "make test"
make test
#
echo "make install"
make install
