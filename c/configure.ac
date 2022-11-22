#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([C-examples], m4_esyscmd([{ git describe --tags --dirty || echo unknown; } | tr -d \\n]),
	[william.r.pursell@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AM_PROG_AR
LT_INIT
AC_CONFIG_SRCDIR([prctl.c])
AC_CONFIG_HEADERS([config.h])

AC_PROG_CC
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h sys/time.h sys/types.h unistd.h])
AC_CHECK_HEADERS([unistd.h sys/mman.h math.h sys/prctl.h])
AC_CHECK_MEMBERS([struct stat.st_mtim, struct stat.st_mtimespec])
AC_CHECK_DECLS([MAP_POPULATE])
AC_CHECK_DECLS([PROT_READ])
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_SEARCH_LIBS([pow],[m],[ncursesw])

AC_CHECK_FUNCS([getdelim gettimeofday strtol arc4random_uniform])

AC_LANG_PUSH(C)
AC_RUN_IFELSE(dnl
	[AC_LANG_PROGRAM(
		[#include <stdio.h>],
		[char s[32]; sprintf(s, "%.3'Lg", 2.0);
			return (s[0] == '%');]
	)],
	AC_DEFINE([HAVE_COMMA_FORMAT],[1],[Whether printf format strings honor single quote])
)
AC_LANG_POP(C)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
