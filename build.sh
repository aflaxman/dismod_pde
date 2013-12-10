#! /bin/bash -e
# -----------------------------------------------------------------------
version='2013.1130'          # trunk version has form yyyy.mmdd
stable_version='2013.0000.1' # stable has form yyyy.#.#
IPOPT_PREFIX="$HOME/prefix/ipopt"
CPPAD_PREFIX="$HOME/prefix/cppad"
OMHELP_PREFIX="$HOME/prefix/omhelp"
DISMOD4_PREFIX="$HOME/prefix/dismod_pde"
DISMOD4_DEBUG='true'
if [ "$DISMOD4_DEBUG" == 'true' ]
then
	CXX_FLAGS="-g -Wall -ansi -pedantic-errors"
	ADD_LINK="-g"
else
	CXX_FLAGS="-O2 -DNDEBUG -Wall -ansi -pedantic-errors"
	ADD_LINK=""
fi
PYTHON_PREFIX=`which python | sed -e 's|/bin/python||'`
host=`hostname`
if [ "$host" == 'omak.ihme.washington.edu' ]
then
	PYTHON_PREFIX='/usr/local/epd_py25-4.3.0'
fi
# ----------------------------------------------------------------------
if [ $0 != "./build.sh" ]
then
	echo "./build.sh: must be executed from its directory"
	exit 1
fi
if [ "$2" != "" ]
then
	for option in $*
	do
		echo "========================================================="
		echo "begin: build.sh $option"
		./build.sh $option
	done
	echo "OK: build.sh $*"
	exit 0
fi
# -----------------------------------------------------------------------
# setup command to all options
#
# make sure external and build directory exist
for dir in external build
do
	if [ ! -e $dir ]
	then
		echo "mkdir $dir"
		mkdir $dir
	fi
done
# ----------------------------------------------------------------------
if [ "$1" == "external" ]
then
	log_file="external.log"
	#
	echo "bin/get_ipopt.sh $IPOPT_PREFIX > $log_file"
	bin/get_ipopt.sh $IPOPT_PREFIX > $log_file
	#
	echo "bin/get_cppad.sh $CPPAD_PREFIX $IPOPT_PREFIX >> $log_file"
	bin/get_cppad.sh $CPPAD_PREFIX $IPOPT_PREFIX >> $log_file
	#
	echo "bin/get_omhelp.sh $OMHELP_PREFIX >> $log_file"
	bin/get_omhelp.sh $OMHELP_PREFIX >> $log_file
	#
	echo "OK: build.sh $1"
	exit 0
fi
# ----------------------------------------------------------------------
if [ "$1" == "version" ]
then
	#
	# change version number in configure.ac
	echo "sed -i.old configure.ac ..."
	sed -i.old \
	-e "s/(dismod_pde, [0-9]\{4\}\.[0-9]\{4\} *,/(dismod_pde, $version,/" \
	configure.ac 
	#
	if ! grep "(dismod_pde, $version," configure.ac
	then
		echo "build.sh version: failed to change version in configure.ac."
		echo "Perhaps build.sh needs to be changed for a new stable verison."
		echo "Otherwise, change configure.ac by hand."
		exit 1
	fi
	#
	echo "diff configure.ac.old configure.ac"
	if diff configure.ac.old configure.ac
	then
		echo "	no difference found"
	fi
	#
	echo "rm configure.ac.old"
	rm configure.ac.old
	#
	# change version number in omh/install.omh
	# (need to be able to handle stable and release versions).
	sed -i.old \
		-e "s|dismod_pde-[0-9.]\{9\}\.tgz|dismod_pde-$version.tgz|g"\
		-e "s|dismod_pde-[0-9.]\{11\}\.tgz|dismod_pde-$stable_version.tgz|g"\
		-e "s|\([ -]\)[0-9.]\{9\}\([/\$]\)|\1$version\2|g"\
		-e "s|\([ -]\)[0-9.]\{11\}\([/\$]\)|\1$stable_version\2|g"\
		omh/install.omh 
	if ! grep "dismod_pde-$version.tgz" omh/install.omh
	then
		echo "build.sh verison: failed to change version in omh/install.omh"
		echo "Perhaps build.sh needs to be changed for a new stable verison."
		echo "Otherwise, change omh/install.omh by hand."
		exit 1
	fi
	echo "diff omh/install.omh.old omh/install.omh"
	if diff omh/install.omh.old omh/install.omh 
	then
		echo "	no difference found"
	fi
	#
	echo "rm omh/install.omh.old"
	rm omh/install.omh.old
	#
	echo "OK: build.sh $1"
	exit 0
fi
# ----------------------------------------------------------------------
if [ "$1" == "doxydoc" ]
then
	# create the file ./doxyfile 
	error_file='doxygen.err'
	output_directory='doxydoc'
	bin/doxyfile.sh "$version" "$error_file" "$output_directory"
	#
	log_file="doxydoc.log"
	echo "cd build"
	cd build
	log_path="../$log_file"
	#
	#
	if [ ! -e "$output_directory" ]
	then
		echo mkdir "$output_directory"
		mkdir "$output_directory"
	fi
	#
	#
	echo "doxygen ../doxyfile >& $log_file"
	doxygen ../doxyfile       >& $log_path
	#
	if grep -i ': Warning:' $log_path
	then
		cat $erorr_file
		exit 1
	fi
	if [ -s "$error_file" ]
	then
		cat $error_file
		exit 1
	fi
	#
	echo "OK: build.sh $1"
	exit 0
fi
# ----------------------------------------------------------------------
if [ "$1" == "automake" ]
then
	echo "aclocal"
	aclocal
	echo "autoheader"
	autoheader
	echo "autoconf"
	autoconf
	echo "automake --add-missing"
	automake --add-missing
	#
	echo "OK: build.sh $1"
	exit 0
fi
# ----------------------------------------------------------------------
if [ "$1" == "configure" ]
then
	if [ ! -e makefile.in ]
	then
		echo "build.sh configure: must run build.sh automake first"
		exit 1
	fi
	#
	log_file="configure.log"
	#
	echo "cd build"
	cd build
	log_path="../$log_file"
	#
	cat << EOF
../configure \\
	--prefix="$DISMOD4_PREFIX" \\ 
	PYTHON_PREFIX="$PYTHON_PREFIX" \\
	CPPAD_PREFIX="$CPPAD_PREFIX" \\
	IPOPT_PREFIX="$IPOPT_PREFIX" \\
	CXX_FLAGS="$CXX_FLAGS" \\
	ADD_LINK="$ADD_LINK" \\      
	> $log_file
EOF
	../configure \
		--prefix="$DISMOD4_PREFIX" \
		PYTHON_PREFIX="$PYTHON_PREFIX" \
		CPPAD_PREFIX="$CPPAD_PREFIX" \
		IPOPT_PREFIX="$IPOPT_PREFIX" \
		CXX_FLAGS="$CXX_FLAGS" \
		ADD_LINK="$ADD_LINK" \
		> $log_path
	#
	echo "OK: build.sh $1"
	exit 0
fi
# ----------------------------------------------------------------------
if [ "$1" == "make" ]
then
	if [ ! -e build/makefile ]
	then
		echo "build.sh make: must run build.sh configure first"
		exit 1
	fi
	#
	if [ -e build/cpp/dismod_pde_csv ]
	then
		echo "rm build/cpp/dismod_pde_csv"
		rm build/cpp/dismod_pde_csv
	fi
	#
	log_file="make.log"
	#
	echo "cd build"
	cd build
	log_path="../$log_file"
	#
	echo "make > $log_file"
	make > $log_path
	#
	echo "OK: build.sh $1"
	exit 0
fi
# ----------------------------------------------------------------------
if [ "$1" == "doc" ]
then
	log_file="doc.log"
	log_path=`pwd`/$log_file
	if [ -e $log_file ]
	then
		echo "rm $log_file"
		rm $log_file
	fi
	#
	# ----------------------------------------------------------------------
	run="no"
	list="effect_out.csv measure_out.csv sfun_out.csv"
	for file in $list
	do
		if [ ! -e build/example/get_started/$file ] && [ "$run" == "no" ]
		then
			echo "build/example/get_started/$file is missing"
			run="yes"
		fi
	done
	if [ "$run" == "yes" ]
	then
		# create the file files in list
		echo "pushd build/example/get_started >> $log_file"
		pushd build/example/get_started >> $log_path
		#
 		echo "make test >> $log_file"
 		make test >> $log_path
		#
		echo "popd"
		popd
	fi
	# ----------------------------------------------------------------------
	run="no"
	list="
		parameter.csv 
		effect_out.csv 
		measure_in.csv 
		prevalence_compare.png
		prevalence_residual.png
		prevalence_estimate.png
		iota_estimate.png
	"
	for file in $list
	do
		if [ ! -e build/example/diabetes/$file ] && [ "$run" == "no" ]
		then
			echo "build/example/diabetes/$file is missing"
			run="yes"
		fi
	done
	if [ "$run" == "yes" ]
	then
		# create the file files in list
		echo "pushd build/example/diabetes >> $log_file"
		pushd build/example/diabetes >> $log_path
		#
 		echo "make test >> $log_file"
 		make test >> $log_path
		#
		echo "popd"
		popd
	fi
	# ----------------------------------------------------------------------
	run="no"
	list="
		parameter.csv 
		effect_out.csv 
		prevalence_residual.png
		prevalence_estimate.png
		iota_estimate.png
		iota_estimate-8-1.png
		iota_estimate-4-2.png
		iota_true.png
	"
	for file in $list
	do
		if [ ! -e build/example/gaussian/$file ] && [ "$run" == "no" ]
		then
			echo "build/example/gaussian/$file is missing"
			run="yes"
		fi
	done
	if [ "$run" == "yes" ]
	then
		# create the file files in list
		echo "pushd build/example/gaussian >> $log_file"
		pushd build/example/gaussian >> $log_path
		#
 		echo "make test >> $log_file"
 		make test >> $log_path
		#
		echo "popd"
		popd
	fi
	# ----------------------------------------------------------------------
	#
	if [ -e "doc" ]
	then
		echo "rm -r doc"
		rm -r doc
	fi
	echo "mkdir doc"
	mkdir  doc
	#
	echo "cd doc"
	cd doc
	#
	cmd="$OMHELP_PREFIX/bin/omhelp ../dismod_pde.omh -noframe -debug"
	#
	echo "$cmd -xml -printable >> $log_file"
	$cmd -xml -printable       >> $log_path
	#
	echo "$cmd -printable > /dev/null"
	$cmd -printable       > /dev/null
	#
	echo "$cmd > /dev/null"
	$cmd       > /dev/null
	#
	echo "$cmd -xml >> $log_file"
	$cmd -xml       >> $log_path
	#
	if grep "^OMhelp Warning:" $log_path
	then
		echo "aborting build.sh doc"
		exit 1
	fi
	#
	# make default page the xml version of the documentation
	if [ ! -e 'dismod_pde.xml' ]
	then
		echo "build.sh: can't make dismod_pde.xml default documentation page."
		exit 1
	fi
cat << EOF > index.htm
<html><head><script>
window.location.href="dismod_pde.xml";
</script></head></html>
EOF
	#
	echo "OK: build.sh $1"
	exit 0
fi
# ----------------------------------------------------------------------
if [ "$1" = "dist" ] 
then
	stable_year=`echo $stable_version | sed -e 's|\..*||'`
	stable_dir="$HOME/dismod_pde/stable/$stable_year"
	if [ ! -e "$stable_dir" ]
	then
		echo "build.sh dist: must first checkout $stable_dir"
		exit 1
	fi
	stable_tarball="$stable_dir/build/dismod_pde-$stable_version.tgz"
	if [ ! -e $stable_tarball ]
	then
		echo "build.sh dist: must first create $stable_tarball"
		exit 1
	fi
	log_file="dist.log"
	if [ ! -e makefile.in ]
	then
		echo "build.sh dist: must run build.sh configure first"
		exit 1
	fi
	if [ -e $log_file ]
	then
		echo "rm $log_file"
		rm  $log_file
	fi
	#
	echo "cd build"
	cd build
	log_path="../$log_file"
	#
	for name in dismod_pde-$version dismod_pde-$version.tgz
	do
		if [ -e "$name" ]
		then
			echo "rm -r $name >> $log_file"
			rm -r $name >> $log_path
		fi
	done
	#
	echo "make dist >> $log_file"
	make dist >> $log_path
	#
	if [ ! -e dismod_pde-$version.tar.gz ]
	then
		echo "dismod_pde-$version.tar.gz does not exist,"
		echo "perhaps version is out of date."
		#
		exit 1
	fi
	# change *.tar.gz to *.tgz
	echo "mv dismod_pde-$version.tar.gz dismod_pde-$version.tgz >> $log_file"
	echo "mv dismod_pde-$version.tar.gz dismod_pde-$version.tgz" >> $log_path
	mv dismod_pde-$version.tar.gz dismod_pde-$version.tgz
	#
	# put a copy of this version tarball in documentation directory
	echo "cp dismod_pde-$version.tgz ../doc/dismod_pde-$version.tgz"
	cp dismod_pde-$version.tgz ../doc/dismod_pde-$version.tgz
	#
	# put a copy of stable tarball in documentation directory
	echo "cp $stable_tarball ../doc/dismod_pde-$stable_version.tgz"
	cp $stable_tarball ../doc/dismod_pde-$stable_version.tgz
	#
	echo "OK: build.sh $1"
	exit 0
fi
# ----------------------------------------------------------------------
if [ "$1" == "test" ]
then
	log_file="test.log"
	log_dir=`pwd`
	#
	echo "cd build"
	cd build
	log_path="../$log_file"
	#
	echo "make test_not_doc > $log_file"
	echo "To see tests that succeeded, in another window use the commands:"
	echo "	cd $log_dir"
	echo "	grep OK $log_file"
	make test_not_doc > $log_path 
	#
	echo "OK: build.sh $1"
	exit 0
fi
# ----------------------------------------------------------------------
if [ "$1" == "all" ]
then
	./build.sh version doxydoc automake configure make doc dist
	#
	log_file="all.log"
	config_file="configure.ac"
	echo "cd build"
	cd build
	log_path="../$log_file"
	config_path="../$config_file"
	#
	if [ -e dismod_pde-$version ]
	then
		echo "rm -r dismod_pde-$version"
		rm -r dismod_pde-$version
	fi
	#
	echo "tar -xzf dismod_pde-$version.tgz"
	tar -xzf dismod_pde-$version.tgz
	#
	echo "cd dismod_pde-$version"
	cd dismod_pde-$version
	log_path="../$log_path"
	#
	echo "./build.sh configure"
	./build.sh configure
	#
	echo "cat configure.log > $log_file"
	cat configure.log > $log_path
	#
	echo "./build.sh test"
	./build.sh test
	#
	echo "cat test.log >> $log_file"
	cat test.log >> $log_path
	#
	echo "OK: build.sh $1"
	exit 0
fi
# ----------------------------------------------------------------------
if [ "$1" == "internet" ]
then
	if [ "$USER" != "bradbell" ]
	then
		echo "Only Brad Bell can copy a new version to the internet."
		exit 1
	fi
	file="doc/dismod_pde-$version.tgz"
	remote_machine='moby'
	remote_directory='/var/www/moby.ihme.washington.edu/bradbell'
	if [ ! -e "$file" ]
	then
		echo "The file $file is missing."
		exit 1
	fi
	#
	echo "tar -czf doc.tgz doc"
	tar -czf doc.tgz doc
	#
	echo "ssh.sh $remote_machine -p doc.tgz"
	ssh.sh $remote_machine -p doc.tgz
	#
cat << EOF
------------------------------
Enter the following commands: 
	ssh.sh $remote_machine -l
	rm -rf doc
	tar -xzf doc.tgz
	rm -r $remote_directory/dismod_pde
	cp -r doc $remote_directory/dismod_pde
	exit
------------------------------
EOF
	exit 0
fi
# ----------------------------------------------------------------------
if [ "$1" == "clean" ]
then
	#
	name_list="
		.deps

		all.log
		cppad.log
		configure.log
		dist.log
		doxydoc.log
		external.log
		ipopt.log
		make.log
		doc.log
		test.log

		aclocal.m4
		autom4te.cache
		build
		config.h.in
		commit.sh.old
		commit.[0-9][0-9]*
		configure
		depcomp
		INSTALL
		install-sh
		install.old
		junk
		junk.sh
		makefile.in
		missing
		temp
	"
	dir_list="
		.
		cpp
		cpp/dismod_pde
		example
		example/diabetes
		example/gaussian
		example/get_started
		python
		test
	"
	for dir in $dir_list
	do
		for name in $name_list
		do
			if [ -d "$dir/$name" ]
			then
				echo "rm -r $dir/$name"
				rm -r $dir/$name
			elif [ -e "$dir/$name" ]
			then
				echo "rm $dir/$name"
				rm $dir/$name
			fi
		done
	done
	#
	echo "OK: build.sh $1"
	exit 0
fi
# ----------------------------------------------------------------------
if [ "$1" != "" ]
then
	echo "$1 is not a valid option"
fi
cat << EOF
usage: ./build.sh option_1 option_2 ...

Options
-------
external:  build ipopt and cppad libraries, and omhelp executable.

version:   change the dismod_pde version to the current date.
doxydoc:   build the developer documentation in ./doxydoc
automake:  run the autotools to create a new configure file.
configure: configure dismod_pde for this system.
make:      compile and link build/cpp/dismod_pde_csv
doc:       build the user documentation in ./doc
dist:      create the dismod_pde distribution tarball for current version.
test:      compile and run all test cases (excepting doc test cases).
all:       run the commands above (in order) excluding the external command;
           in addition, use the tarball distribution to run the test command.

internet:  replace internet copy of the distribution.
clean:     clean the working copy of the repository (use to rerun user tests).

EOF
exit 1
