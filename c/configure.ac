#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([C-examples], [0.0.1], [william.r.pursell@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_SRCDIR([prctl.c])
AC_CONFIG_HEADERS([config.h])

AC_PROG_CC
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h sys/time.h unistd.h])
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_CHECK_FUNCS([getdelim gettimeofday strtol])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
