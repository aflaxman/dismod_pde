#! /bin/bash -e
# $Id: search.sh 2691 2012-12-30 23:39:57Z bradbell $
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     GNU General Public License Version 3
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ ! -e "bin/search.sh" ]
then
	echo "bin/search.sh: must be executed from its parent directory"
	exit 1
fi
if [ "$1" == "" ]
then
	echo 'usage: bin/search.sh pattern'
	echo '       Search for pattern in the cppad source code'
	exit 1
fi
pattern="$1"
extensions='.ac .am .cpp .hpp .in .omh .sh .py'
directories='
	.
	bin
	cpp
	cpp/dismod_pde
	cpp/fg_info
	example
	example/gaussian
	example/get_started
	example/diabetes
	omh
	python
	test
'
#
find_files.sh "$pattern" "$extensions" "$directories" | \
	 sed -e '/\/makefile.in/d'
