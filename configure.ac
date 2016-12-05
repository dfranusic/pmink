#
# This file is part of Project MINK <http://www.release14.org>.
#
# Copyright (C) 2012 Release14 Ltd.
# http://www.release14.org/
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

AC_PREREQ([2.68])
AC_INIT([pmink], [1.0], [df@release14.org], , [http://www.release14.org/])
config_flags="$*"
AC_DEFINE_UNQUOTED([CONFIG_FLAGS],["$config_flags"],[Flags passed to configure])
AC_SUBST(CONFIG_FLAGS, "$config_flags")
AM_INIT_AUTOMAKE([foreign subdir-objects nostdinc])
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_LANG([C++])
# ********** arguments **********
# enable c++14
AC_ARG_ENABLE(cpp14, 
              [AS_HELP_STRING([--enable-cpp14], [Enable C++14 support [default=no]])])
AM_CONDITIONAL(ENABLE_CPP14, test "x$enable_cpp14" = "xyes")

# enable pfring
AC_ARG_WITH(pfring, 
            [AS_HELP_STRING([--with-pfring], [PF_RING installation directory])], [], [with_pfring=no])
AM_CONDITIONAL(ENABLE_PFRING, test "x$with_pfring" != "xno")

# enable DEBUG mode
AC_ARG_ENABLE(debug, 
	      [AS_HELP_STRING([--enable-debug], [enable DEBUG mode (include debug symbols and disable optimizations) [default=no]])])
AM_CONDITIONAL(ENABLE_DEBUG, test "x$enable_debug" = "xyes")

# optimization level
AC_ARG_WITH(olevel, 
	    [AS_HELP_STRING([--with-olevel], [Compiler optimization level [default=2]])],
	    [], [with_olevel="2"])
AC_SUBST(OLEVEL, "$with_olevel")

# pmink debug
AC_ARG_ENABLE(pmdebug, 
	      [AS_HELP_STRING([--enable-pmdebug], [enable pMINK DBUEG mode [default=no]])])
AM_CONDITIONAL(ENABLE_PMDEBUG, test "x$enable_pmdebug" = "xyes")

# bundled luajit
AC_ARG_ENABLE(luajitbundled, 
	      [AS_HELP_STRING([--enable-luajitbundled], [enable bundled LuaJIT 2.0.3 library [default=no]])])
AM_CONDITIONAL(ENABLE_LUAJITBUNDLED, test "x$enable_luajitbundled" = "xyes")

#+-----------------------+
#| run configure on libs |
#+-----------------------+

# pfring
if test "x$with_pfring" != "xno"; then
    AC_MSG_NOTICE([Enabling PF_RING runtime])
    AC_SUBST(PF_RING_CFLAGS, "-D__PFRING__ -DHAVE_RW_LOCK -I$with_pfring/include")
    AC_SUBST(PF_RING_LIBS, "-L$with_pfring/lib -lpfring")
fi

# antlr3c
AC_MSG_NOTICE([Enabling bundled antlr3c])
AC_CONFIG_SUBDIRS([lib/libantlr3c-3.4])

# libiconv with GSM7
AC_MSG_NOTICE([Enabling libiconv with GSM7 support])
AC_CONFIG_SUBDIRS([lib/libiconv-1.14])

# LuaJIT
if test "x$enable_luajitbundled" = "xyes"; then
    AC_MSG_NOTICE([Enabling LuaJIT])
    AC_CONFIG_SUBDIRS([lib/LuaJIT-2.0.3])
fi

# set default g++ compiler flags to null
: ${CFLAGS=}
: ${CXXFLAGS=}
AC_PROG_CXX
# ********** check for programs **********
AC_PROG_AWK
AC_PROG_CC
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB
AC_PROG_CPP
AC_PROG_SED
AC_USE_SYSTEM_EXTENSIONS
PKG_PROG_PKG_CONFIG
LT_INIT

# check if system is little endian
AC_C_BIGENDIAN([AC_MSG_ERROR([Big endian system is not supported by current version of pMINK!])], 
		AC_MSG_RESULT([Little endian system detected!]))
# check for 64bit operating system
AC_CHECK_SIZEOF([void*])
if test "$ac_cv_sizeof_voidp" -ne 8
then
    AC_MSG_ERROR([64bit operating system NOT detected, stopping!])
else
    AC_MSG_RESULT([64bit operating system detected!])
fi


# ********** check for libraries **********
# clock_gettime
AC_CHECK_LIB([rt], [clock_gettime], , AC_MSG_ERROR([clock_gettime not found!]))
# check for boost 1.49.0
AX_BOOST_BASE([1.49.0], , [AC_MSG_ERROR([Boost library not found!])])
# check for boost-regex
AX_BOOST_REGEX
# check for boost-system
AX_BOOST_SYSTEM
# check boost unit testing
AX_BOOST_UNIT_TEST_FRAMEWORK
# check libicu (icu-config)
AC_CHECK_ICU([4.4], , [AC_MSG_ERROR([Icu library >= 4.4 not found!])])
# check for debian
AC_PATH_PROG([dpkg_found], [dpkg], [0])
AC_CHECK_FILE([/etc/debian_version], [AC_SUBST(debian_version_found, "1")], [])
AM_CONDITIONAL(DEBIAN_FOUND, test "$dpkg_found" != "0" && test "$debian_version_found" != "0")
# check for openssl
AC_PATH_PROG([openssl_found], [openssl], [0])
AM_CONDITIONAL(OPENSSL_FOUND, test "$openssl_found" != "0")
# check for libcurl
PKG_CHECK_MODULES([CURL], [libcurl], [], [AC_MSG_ERROR([Curl library not found!])])
# check if using bundled luajit
if test "x$enable_luajitbundled" != "xyes"
then
    PKG_CHECK_MODULES([LUAJIT], [luajit >= 2.0.3], [], [AC_MSG_ERROR([Luajit library not found!])])
else
    AC_SUBST(LUAJIT_CFLAGS, "-Ilib/LuaJIT-2.0.3/src")
    AC_SUBST(LUAJIT_LIBS, "-Llib/LuaJIT-2.0.3/src -lluajit")
fi
# check for ncurses
PKG_CHECK_MODULES([NCURSES], [ncurses >= 5], [], [AC_MSG_ERROR([Ncurses library >= 5 not found!])])
# check for libpcap
AC_CHECK_LIB([pcap], [pcap_open_live], , AC_MSG_ERROR([libpcap library not found!]))
# pthread
AC_CHECK_LIB([pthread], [pthread_create], , AC_MSG_ERROR([pthread library not found!]))
# sctp
AC_CHECK_LIB([sctp], [sctp_send], , AC_MSG_ERROR([sctp library not found!]))
# net-snmp
AC_CHECK_NET_SNMP([5.4], , [AC_MSG_ERROR([Net-snmp library not found!])])
# libcap
AC_CHECK_LIB([cap], [cap_get_proc], , AC_MSG_ERROR([cap library not found!]))
# mycql c++ connector
AX_LIB_MYSQLCPPCONN
# Checks for header files.
AC_CHECK_HEADERS([	arpa/inet.h \
			fcntl.h  \
			inttypes.h \ 
			locale.h \
			netdb.h \
			netinet/in.h \
			netinet/sctp.h \ 
			stddef.h \
			stdint.h \
			stdlib.h \
			string.h \
			strings.h \
			sys/ioctl.h \ 
			sys/socket.h \
			sys/time.h \
			sys/timeb.h \
			syslog.h \
			pthread.h \
			unicode/utf8.h \
			unicode/unistr.h \
			unicode/stringpiece.h \
			unicode/ustream.h \
			unicode/ucnv.h \
			curses.h \
			unistd.h], , AC_MSG_ERROR([Header file missing!]))

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_MKTIME
AC_FUNC_STRNLEN
AC_FUNC_STRTOD
AC_CHECK_FUNCS([alarm \
		atexit \
		bzero \
		gethostname \
		gettimeofday \
		inet_ntoa \
		isascii \
		localtime_r \
		memmove \
		memset \
		mkdir \
		mkfifo \
		pow \
		select \
		poll \
		setenv \
		socket \
		strchr \
		strdup \
		strerror \
		strncasecmp \
		strstr \
		strtoul \
		strtoull \
		sysinfo], , AC_MSG_ERROR([System function missing!]))

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
