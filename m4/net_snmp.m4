# SYNOPSIS
#
#   AX_CHECK_NET_SNMP(version, action-if, action-if-not)
#
# DESCRIPTION
#
#   Defines NET_SNMP_LIBS and NET_SNMP_CFLAGS. See net-snmp-config(1) man page.
#
# LICENSE
#
#   Copyright (c) 2013 Release14 Ltd.
#
#   Copying and distribution of this file, with or without modification, are
#   permitted in any medium without royalty provided the copyright notice
#   and this notice are preserved. This file is offered as-is, without any
#   warranty.

AU_ALIAS([AC_CHECK_NET_SNMP], [AX_CHECK_NET_SNMP])
AC_DEFUN([AC_CHECK_NET_SNMP], [
  succeeded=no

  if test -z "$NET_SNMP_CONFIG"; then
    AC_PATH_PROG(NET_SNMP_CONFIG, net-snmp-config, no)
  fi

  if test "$NET_SNMP_CONFIG" = "no" ; then
    echo "*** The net-snmp-config could not be found! ***"
  else
    NET_SNMP_VERSION=`$NET_SNMP_CONFIG --version`
    AC_MSG_CHECKING(for net-snmp >= $1)
        VERSION_CHECK=`expr $NET_SNMP_VERSION \>\= $1`
        if test "$VERSION_CHECK" = "1" ; then
            AC_MSG_RESULT(yes)
            succeeded=yes

            AC_MSG_CHECKING(NET_SNMP_CFLAGS)
            NET_SNMP_CFLAGS=`$NET_SNMP_CONFIG --base-cflags`
            AC_MSG_RESULT($NET_SNMP_CFLAGS)

            AC_MSG_CHECKING(NET_SNMP_LIBS)
            NET_SNMP_LIBS=`$NET_SNMP_CONFIG --libs`
            AC_MSG_RESULT($NET_SNMP_LIBS)
        else
            NET_SNMP_CFLAGS=""
            NET_SNMP_LIBS=""
            ## If we have a custom action on failure, don't print errors, but
            ## do set a variable so people can do so.
            ifelse([$3], ,echo "can't find net-snmp >= $1",)
        fi

        AC_SUBST(NET_SNMP_CFLAGS)
        AC_SUBST(NET_SNMP_LIBS)
  fi

  if test $succeeded = yes; then
     ifelse([$2], , :, [$2])
  else
     ifelse([$3], , AC_MSG_ERROR([Library requirements (net-snmp) not met.]), [$3])
  fi
])
