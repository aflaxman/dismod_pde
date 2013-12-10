#! /bin/bash -e
#
# tarball="http://www.coin-or.org/download/source/Ipopt/Ipopt-3.8.3.tgz"
# tarball="http://www.coin-or.org/download/source/Ipopt/Ipopt-3.9.2.tgz"
# stable="3.10"
stable=''
tarball='http://www.coin-or.org/download/source/Ipopt/Ipopt-3.11.1.tgz'
# --------------------------------------------------------------------------
if [ $0 != "bin/get_ipopt.sh" ]
then
	echo "bin/get_ipopt.sh: must be executed from its parent directory"
	exit 1
fi
if [ "$1" == "" ]
then
	echo "usage: bin/get_ipopt.sh install_prefix"
	exit 1
fi
prefix="$1"
#
if [ "$stable" != "" ]
then
	version=$stable
else
	version=`echo $tarball | sed -e 's|.*/Ipopt-\(.*\)\.tgz|\1|'`
fi
#
if [ ! -e external ]
then
	echo "mkdir external"
	mkdir external
fi
echo "cd external"
cd external
#
#
if [ -e Ipopt-$version ]
then
	echo "Using existing external/Ipopt-$version"
else
	if [ "$stable" != "" ]
	then
		web_page="https://projects.coin-or.org/svn/Ipopt"
		echo "svn checkout $web_page/stable/$version Ipopt-$version"
		svn checkout $web_page/stable/$version Ipopt-$version
	else
		if [ -e Ipopt-$version.tgz ]
		then
			echo "Using existing external/Ipopt-$version.tgz"
		else
			echo "wget $tarball"
			wget $tarball
		fi
		echo "tar -xzf Ipopt-$version.tgz"
		tar -xzf Ipopt-$version.tgz
	fi
fi
#
echo "cd Ipopt-$version"
cd Ipopt-$version
# ----------------------------------------------------------------------------
# patch Ipopt source so that it does not print message every time it runs
file="Ipopt/src/Algorithm/IpIpoptAlg.cpp"
comment="// Bradley M. Bell 2009-07-01: Suppress printing startup message"
if ! grep 'options.GetBoolValue("sb" *,' $file > /dev/null
then
	if ! grep "$comment" $file > /dev/null
	then
		echo "sed -istamp $file ..."
		sed -istamp $file -e \
		"s|\(.* bool.*[ _]message_printed *= *\)false;|$comment\n\/\/&\n\1 true;|" 
	fi
	if ! grep "$comment" $file > /dev/null
	then
		echo "Suppression of message printing failed"
		exit 1
	else
		echo "Suppression of message printing succeeded"
	fi
fi
#
for file in ThirdParty/Blas/get.Blas ThirdParty/Lapack/get.Lapack
do
	sed -e 's|ftp:\(\/\/www.netlib.org\/\)|http:\1|' \
		-i $file
done
# --------------------------------------------------------------------
if [ -e ThirdParty/HSL ]
then
	echo "rm -rf ThirdParty/HSL"
	rm -rf ThirdParty/HSL
fi
#
echo "cd ThirdParty"
cd ThirdParty
#
echo "cd Blas"
cd Blas
#
if [ -e saxpy.f ]
then
	echo "Using existing external/Ipopt-$version/ThirdParty/Blas/*.f"
else
	echo "./get.Blas"
	./get.Blas
fi
echo "cd ../Lapack"
cd ../Lapack
#
if [ -e LAPACK ]
then
	echo "Using existing external/Ipopt-$version/ThirdParty/Lapack/LAPACK"
else
	echo "./get.Lapack"
	./get.Lapack
fi
echo "cd ../Mumps"
cd ../Mumps
#
if [ -e MUMPS ]
then
	echo "Using existing external/Ipopt-$version/ThirdParty/Mumps/MUMPS"
else
	echo "./get.Mumps"
	./get.Mumps
fi
echo "cd ../Metis"
cd ../Metis
#
if (ls | grep metis-[0-9] > /dev/null)
then
	metis_version=`ls | grep metis-[0-9]`
	echo "Using existing external/Ipopt-$version/ThirdParty/Metis/$metis_version"
else
	if [ "$version" == 3.9.2 ]
	then
		echo "Removing patch in get.Metis for Ipopt-3.9.2"
		sed -i \
-e 's|^patch -p0 < metis.patch|# Bradley M. Bell: patch longer needed: &|' \
		get.Metis
	else
		echo "Not Ipopt-3.9.2"
	fi
	echo "./get.Metis"
	./get.Metis
fi
#
echo "cd ../.."
cd ../..
#
if [ ! -e build ]
then
	echo "mkdir build"
	mkdir build
fi
#
echo "cd build"
cd build
#
options="--prefix=$prefix"
options="$options --enable-debug"
options="$options --enable-static=yes"
options="$options --enable-shared=no"
echo "../configure $options"
../configure $options
#
echo "make install"
make install
