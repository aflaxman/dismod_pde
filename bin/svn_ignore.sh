#! /bin/bash -e
# $Id: svn_ignore.sh 6192 2013-11-25 22:18:52Z bradbell $
#  --------------------------------------------------------------------------
# dismod_ode: MCMC Estimation of Disease Rates as Functions of Age
#           Copyright (C) 2013 University of Washington
#              (Bradley M. Bell bradbell@uw.edu)
# 
# This program is distributed under the terms of the 
# 	     GNU Affero General Public License version 3.0 or later
# see http://www.gnu.org/licenses/agpl.txt
# -------------------------------------------------------------------------- */
if [ $0 != "bin/svn_ignore.sh" ]
then
	echo "bin/svn_ignore.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
cat << EOF > bin/svn_ignore.$$
autom4te.cache
build
doc
doc.tgz
doxyfile
external
junk
junk.*
*.log
new
temp
EOF
svn propset svn:ignore --recursive -F bin/svn_ignore.$$ .
rm bin/svn_ignore.$$
#
exit 0 
