#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([wrp-utils],
	m4_esyscmd([./build-aux/package-version]),
	[william.r.pursell@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AM_PROG_AR
LT_INIT
AC_CONFIG_HEADERS([config.h])

AC_PROG_CC
AC_CHECK_HEADERS([unistd.h])
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

AC_FUNC_REALLOC
AC_SEARCH_LIBS([pow],[m])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
