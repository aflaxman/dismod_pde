dnl    package   version               bug-report
AC_INIT(dismod_pde, 2013.1130, bradbell@washington.edu)
AC_MSG_NOTICE([--- Begin checking configure command line arguments ---])

dnl make sure PYTHON_PREFIX is specified
AC_MSG_CHECKING([PYTHON_PREFIX])
AC_ARG_VAR(PYTHON_PREFIX, 
	[required: directory containing bin/python that dismod_pde will use]
)
if test "$PYTHON_PREFIX" != "" ; then
	AC_MSG_RESULT([$PYTHON_PREFIX])
else
	AC_MSG_ERROR([PYTHON_PREFIX=value, missing from configure command line])
fi

dnl make sure CPPAD_PREFIX is specified
AC_MSG_CHECKING([CPPAD_PREFIX])
AC_ARG_VAR(CPPAD_PREFIX, 
	[required: directory containing cppad source and libcppad_ipopt.a]
)
if test "$CPPAD_PREFIX" != "" ; then
	AC_MSG_RESULT([$CPPAD_PREFIX])
else
	AC_MSG_ERROR([CPPAD_PREFIX=value, missing from configure command line])
fi

dnl make sure IPOPT_PREFIX is specified
AC_MSG_CHECKING([IPOPT_PREFIX])
AC_ARG_VAR(IPOPT_PREFIX,
	[required: prefix directory during Ipopt installation]
)
if test "$IPOPT_PREFIX" != "" ; then
	AC_MSG_RESULT([$IPOPT_PREFIX])
else
	AC_MSG_ERROR([IPOPT_PREFIX=value, missing from configure command line])
fi

dnl check if CXX_FLAGS is specified
AC_MSG_CHECKING([CXX_FLAGS])
AC_ARG_VAR(CXX_FLAGS,
	[CXX_FLAGS: used for C++ compiler flags, CXXFLAGS is ignored]
)
if test "$CXX_FLAGS" != "" ; then
	AC_MSG_RESULT([$CXX_FLAGS])
else
	AC_MSG_RESULT([no])
fi
AC_SUBST(CXXFLAGS, "")

dnl check if ADD_LINK is specified
AC_MSG_CHECKING([ADD_LINK])
AC_ARG_VAR(ADD_LINK,
	[optional: extra flags during linking]
)
if test "$ADD_LINK" != "" ; then
	AC_MSG_RESULT([$ADD_LINK])
else
	AC_MSG_RESULT([no])
fi

AC_MSG_NOTICE([---- End checking configure command line arguments ----])


dnl Set the default install prefix to the installers home directory
dnl cygwin begin delete:
AC_PREFIX_DEFAULT(${HOME})

dnl By defalut disable maintainer mode when running configure;
dnl --enable-maintainer-mode will enable it.
dnl This avoids the autoconf and automake tools being launched by make
AM_MAINTAINER_MODE

dnl Run any macros required for proper operation of generated makefiles
dnl nostdinc: Do not define standard include directories in generated makefiles
AM_INIT_AUTOMAKE(nostdinc)

dnl Determine if ranlib is present and where it is if present (set RANLIB)
AC_PROG_RANLIB

dnl AC_PROG_CXX([compiler-search-list]) outputs CXX as C++ compiler to use
AC_PROG_CXX

dnl check for fortran compiler
AC_PROG_FC

dnl determine FCLIBS, flags necessary for linking C++ with modern fortran
AC_FC_LIBRARY_LDFLAGS

dnl Have configure check source directory for correctness
AC_CONFIG_SRCDIR(cpp/dismod_pde/dismod_pde.hpp)

dnl Use config.h instead of make file command line arguments
AC_CONFIG_HEADER(cpp/dismod_pde/config.h)

dnl Check for pkg-config program
AC_CHECK_PROG(HAVE_PKG_CONFIG, [pkg-config], [yes], [no])

dnl check for dl library
AC_CHECK_LIB(
	[dl],
	[dlopen],
	[DL_LIB="-ldl"],
	[DL_LIB=""]
)

dnl check for pthread library
AC_CHECK_LIB(
	[pthread],
	[pthread_create],
	[PTHREAD_LIB="-lpthread"],
	[PTHREAD_LIB=""]
)

dnl Determine link line for ipopt
temp="-L $IPOPT_PREFIX/lib -L $IPOPT_PREFIX/lib64 -lipopt $PTHREAD_LIB $DL_LIB"
AC_SUBST(IPOPT_LIBS, "$temp")
if test "$HAVE_PKG_CONFIG" == "yes"; then
	dnl set IPOPT_LIBS
	temp="$IPOPT_PREFIX/lib/pkgconfig:$IPOPT_PREFIX/lib64/pkgconfig"
	export PKG_CONFIG_PATH="$temp"
	if pkg-config --libs ipopt >& /dev/null ; then
		IPOPT_LIBS=`pkg-config --libs ipopt`
	else
		dnl check for blas library
		AC_LANG_PUSH([Fortran])
		AC_CHECK_LIB(
			[blas],
			[ddot],
			[IPOPT_LIBS="$IPOPT_LIBS -lblas"],
		)
		dnl check for lapack library
		AC_CHECK_LIB(
			[lapack],
			[dgesv],
			[IPOPT_LIBS="$IPOPT_LIBS -llapack"],
		)
		AC_LANG_POP([Fortran])
	fi
else
	dnl check for blas library
	AC_LANG_PUSH([Fortran])
	AC_CHECK_LIB(
		[blas],
		[ddot],
		[IPOPT_LIBS="$IPOPT_LIBS -lblas"],
	)
	dnl check for lapack library
	AC_CHECK_LIB(
		[lapack],
		[dgesv],
		[IPOPT_LIBS="$IPOPT_LIBS -llapack"],
	)
	AC_LANG_POP([Fortran])
fi
AC_MSG_RESULT([ipopt libraries: $IPOPT_LIBS])

dnl -----------------------------------------------------------------------
dnl AC_CONFIG_FILES for each file in the list, configure will 
dnl read file.in, do its substitutions, and create file
AC_CONFIG_FILES([
	test/age2_from0.sh
	test/bilinear.sh
	test/dismoditis.sh
	test/constant.sh
	test/gaussian.sh
	test/initial_C.sh
	example/diabetes/diabetes.sh
	example/gaussian/gaussian.sh
	example/get_started/get_started.py
	example/get_started/get_started.sh
	python/d4_simulate.py
	python/d4_sfun_grid.py
	python/d4_sfun_der.py
	python/d4_plot.py
	python/d4_cumulative.py
	python/d4_contour.py
	python/d4_compare_sfun.py
	python/d4_change_sfun_in.py
	python/d4_bubble.py 
	cpp/makefile
	example/makefile
	example/diabetes/makefile
	example/gaussian/makefile
	example/get_started/makefile
	makefile
	python.sh
	test/makefile
])
AC_OUTPUT_COMMANDS(
	[chmod +x test/age2_from0.sh]
	[chmod +x test/bilinear.sh]
	[chmod +x test/constant.sh]
	[chmod +x test/dismoditis.sh]
	[chmod +x test/gaussian.sh]
	[chmod +x test/initial_C.sh]
	[chmod +x example/diabetes/diabetes.sh]
	[chmod +x example/gaussian/gaussian.sh]
	[chmod +x example/get_started/get_started.py]
	[chmod +x example/get_started/get_started.sh]
	[chmod +x python/d4_simulate.py]
	[chmod +x python/d4_sfun_grid.py]
	[chmod +x python/d4_sfun_der.py]
	[chmod +x python/d4_plot.py]
	[chmod +x python/d4_cumulative.py]
	[chmod +x python/d4_contour.py]
	[chmod +x python/d4_compare_sfun.py]
	[chmod +x python/d4_change_sfun_in.py]
	[chmod +x python/d4_bubble.py]
	[chmod +x python.sh]
)

dnl create all the requested output files
AC_OUTPUT
