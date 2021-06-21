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

# aclocal m4 dir
ACLOCAL_AMFLAGS = -I m4

# subdirs
SUBDIRS = lib/libantlr3c-3.4 \
	  lib/libiconv-1.14

# check for cpp14
if ENABLE_CPP14
GCC_CPP14_FLAGS = -std=c++14
FGN_CPP14_FLAGS = -DFGN_CPP14 -Ilib/spirit/include
endif

# check if using bundled luajit
if ENABLE_LUAJITBUNDLED
SUBDIRS += lib/LuaJIT-2.0.3
endif

# check for debug mode [--enable-debug]
if ENABLE_DEBUG
CXXFLAGS += -O${OLEVEL} -Wall -g -pthread -pipe -Wno-unused-label -Wformat=0 -Wno-comment
else
CXXFLAGS += -O${OLEVEL} -Wall -pthread -pipe -Wno-unused-label -Wformat=0 -Wno-comment
endif

# check for pmdebug mode [--enable-pmdebug]
if ENABLE_PMDEBUG
CPPFLAGS += -DPMDEBUG
endif
CPPFLAGS += -DBOOST_UUID_RANDOM_GENERATOR_COMPAT

# compiler and linkerflags
BOOST_FLAGS = -DBOOST_CB_DISABLE_DEBUG $(BOOST_CPPFLAGS) -DBOOST_UUID_RANDOM_GENERATOR_COMPAT
BOOST_LDLIBS = $(BOOST_REGEX_LIB) $(BOOST_SYSTEM_LIB) $(BOOST_LDFLAGS)
ANTLR_FLAGS = -Ilib/libantlr3c-3.4/include -Ilib/libantlr3c-3.4
ANTLR_LDLIBS = -Llib/libantlr3c-3.4/.libs -lantlr3c
ICONV_FLAGS = -Ilib/libiconv-1.14/include
ICONV_LDLIBS = -Llib/libiconv-1.14/lib/.libs -liconv
COMMON_INCLUDES = -Isrc/pool \
		  -Isrc/spscq \
		  -Isrc/ring \
		  -Isrc/atomic \
		  -Isrc/utils \
		  -Isrc/r14p \
		  -Isrc/asn1 \
		  -Isrc/asn1/r14p \
		  -Isrc/stats \
		  -Isrc/daemon \
		  -Isrc/config \
		  -Isrc/cli \
		  -Isrc/sctp \
		  -Isrc/sccp \
		  -Isrc/m3ua \
		  -Isrc/mtp2 \
		  -Isrc/mtp3 \
		  -Isrc/smpp \
		  -Isrc/smstpdu \
		  -Isrc/routing_rating \
		  -Isrc/tcp_ip \
		  -Isrc/db \
		  -Isrc/pcap \
		  -Isrc/asn1/gsm_map \
		  -Isrc/asn1/itu_tcap \
		  -Isrc/asn1/tcap_dialogue

# pMINK directories
confdir = $(sysconfdir)/pmink
luadir = $(pkgdatadir)/lua
dbdir = $(pkgdatadir)/db

# pMINK data
conf_DATA = src/config/pmink_def.pmcfg \
	    src/cli/cli_default.pmcfg
lua_DATA = src/lua/pmink.lua
doc_DATA = doc/smsf.pdf
nodist_pkgdata_DATA = VERSION CHANGELOG AUTHORS
db_DATA = scripts/db/pmink_db.sql

# pMINK git generated files
BUILT_SOURCES = VERSION CHANGELOG AUTHORS
CLEANFILES = VERSION CHANGELOG AUTHORS
VERSION:
	./version.sh
	echo "Configured with: $(CONFIG_FLAGS)" >> VERSION

CHANGELOG:
	./changelog.sh

AUTHORS:
	./authors.sh


# pMINK convenience libraries
noinst_LTLIBRARIES = libpmutils.la \
		     libpmnet.la \
		     libpmconfig.la \
		     libantlrutils.la \
		     libpmdaemon.la \
		     libr14putils.la \
		     libr14pstats.la \
		     libpmcli.la \
		     libpmdb.la \
		     librrp.la \
		     libtcpip.la \
		     libgsm.la


# m3ua, sccp, smstpdu, smpp, tcap,  gsm map
libgsm_la_SOURCES = src/smstpdu/smstpdu.cpp \
		    src/m3ua/m3ua.cpp \
		    src/sccp/sccp.cpp \
		    src/smpp/smpp.cpp \
		    src/asn1/itu_tcap/itu_tcap.cpp \
		    src/asn1/tcap_dialogue/DialoguePDUs.cpp \
		    src/asn1/gsm_map/GSMMAP.cpp \
		    src/asn1/gsm_map/MAP-BS-Code.cpp \
		    src/asn1/gsm_map/MAP-CH-DataTypes.cpp \
		    src/asn1/gsm_map/MAP-CommonDataTypes.cpp \
		    src/asn1/gsm_map/MAP-ER-DataTypes.cpp \
		    src/asn1/gsm_map/MAP-ExtensionDataTypes.cpp \
		    src/asn1/gsm_map/MAP-MS-DataTypes.cpp \
		    src/asn1/gsm_map/MAP-OM-DataTypes.cpp \
		    src/asn1/gsm_map/MAP-SM-DataTypes.cpp \
		    src/asn1/gsm_map/MAP-SS-Code.cpp \
		    src/asn1/gsm_map/MAP-SS-DataTypes.cpp \
		    src/asn1/gsm_map/MAP-TS-Code.cpp

libgsm_la_CPPFLAGS = $(COMMON_INCLUDES)

# tcp ip
libtcpip_la_SOURCES = src/tcp_ip/tcp_ip.cpp
libtcpip_la_CPPFLAGS = $(COMMON_INCLUDES)

# rrp
librrp_la_SOURCES = src/routing_rating/rrp.cpp \
                    src/routing_rating/rrp_events.cpp
librrp_la_CPPFLAGS = $(COMMON_INCLUDES) $(BOOST_FLAGS)

# db
libpmdb_la_SOURCES = src/db/pmdb.cpp
libpmdb_la_CPPFLAGS = $(COMMON_INCLUDES) $(BOOST_FLAGS)

# pmink_net
libpmnet_la_SOURCES = src/utils/pmink_net.cpp
libpmnet_la_CPPFLAGS = $(COMMON_INCLUDES)

# pmink_utils
libpmutils_la_SOURCES = src/utils/pmink_utils.cpp
libpmutils_la_CPPFLAGS = $(COMMON_INCLUDES)

# antlr_utils
libantlrutils_la_SOURCES = src/utils/antlr_utils.cpp
libantlrutils_la_CPPFLAGS = $(COMMON_INCLUDES) \
	  		    $(ANTLR_FLAGS)

# pmink daemon
libpmdaemon_la_SOURCES = src/daemon/daemon.cpp
libpmdaemon_la_CPPFLAGS = $(COMMON_INCLUDES)

# pmink config
libpmconfig_la_SOURCES = src/config/config.cpp \
			 src/config/config_r14p.cpp
libpmconfig_la_CPPFLAGS = $(COMMON_INCLUDES) \
			  $(ANTLR_FLAGS)

# r14p utils
libr14putils_la_SOURCES = src/r14p/r14p_utils.cpp
libr14putils_la_CPPFLAGS = $(COMMON_INCLUDES)

# cli
libpmcli_la_SOURCES = src/cli/cli.cpp
libpmcli_la_CPPFLAGS = $(COMMON_INCLUDES) \
		       $(ANTLR_FLAGS)
# r14p stats
libr14pstats_la_SOURCES = src/stats/r14p_stats.cpp
libr14pstats_la_CPPFLAGS = $(COMMON_INCLUDES)


# pMINK libraries and plugins
lib_LTLIBRARIES = libr14p.la \
		  libpmcfg.la  \
		  libpmpcap.la

pkglib_LTLIBRARIES = pm_plg_cli_cfg.la

#+-------------------------+
#| pMINK CLI config plugin |
#+-------------------------+
pm_plg_cli_cfg_la_CPPFLAGS = $(COMMON_INCLUDES) \
			     -Isrc/plugins/config \
			     $(ANTLR_FLAGS) \
			     $(BOOST_FLAGS) \
			     $(NCURSES_CFLAGS)

pm_plg_cli_cfg_la_SOURCES = src/plugins/config/plg_config.cpp \
			    src/plugins/config/plgcfg_events.cpp

pm_plg_cli_cfg_la_LDFLAGS = -version-info 1:0:0 \
			    -export-dynamic \
			    -shared \
			    -module

pm_plg_cli_cfg_la_LIBADD = $(BOOST_LDLIBS) \
			   $(NCURSES_LIBS) \
			   libr14p.la \
			   libpmconfig.la \
			   libpmdaemon.la \
			   -ldl


#+-----------------------+
#| R14P protocol library |
#+-----------------------+
libr14p_la_CPPFLAGS = $(COMMON_INCLUDES) $(BOOST_FLAGS)
libr14p_la_SOURCES = src/r14p/r14p.cpp \
		     src/r14p/r14p_reg_events.cpp \
		     src/asn1/asn1.cpp \
		     src/asn1/r14p/r14p_asn1.cpp \
		     src/sctp/sctp.cpp \
		     src/sctp/chunk.cpp
libr14p_la_LDFLAGS = -version-info 1:0:0
libr14p_la_LIBADD = -lsctp


#+------------------------------------+
#| pMINK configuration parser library |
#+------------------------------------+
libpmcfg_la_CPPFLAGS = $(ANTLR_FLAGS)
libpmcfg_la_SOURCES = src/config/pminkLexer.c \
		      src/config/pminkParser.c
libpmcfg_la_LDFLAGS = -version-info 1:0:0


#+------------------------------+
#| pMINK packet capture library |
#+------------------------------+
libpmpcap_la_CPPFLAGS = $(COMMON_INCLUDES) $(PF_RING_CFLAGS)
libpmpcap_la_SOURCES = src/pcap/pmpcap.cpp
libpmpcap_la_LDFLAGS = -version-info 1:0:0
libpmpcap_la_LIBADD = -lpcap $(PF_RING_LIBS)

#+------------------+
#| pMINK unit tests |
#+------------------+
check_PROGRAMS = pmink_test
TESTS = pmink_test
pmink_test_SOURCES = test/pmink_test.cpp \
		     test/vparam.cpp \
		     test/fgn_basic_syntax.cpp \
		     src/daemons/fgn/fgn.cpp \
		     src/daemons/fgn/fgn_stats.cpp \
		     src/daemons/fgn/cfg_events.cpp \
		     src/daemons/fgn/fgnd_events.cpp

pmink_test_CPPFLAGS = $(FGN_CPP14_FLAGS) \
		      -DFLOGIC_PAUSE_RESUME \
		      $(COMMON_INCLUDES) \
		      -Itest \
		      -Isrc/daemons/fgn \
		      -Isrc/daemons/sgn \
		      $(ANTLR_FLAGS) \
		      $(LUAJIT_CFLAGS) \
		      $(BOOST_FLAGS) \
		      $(ICU_CPPFLAGS) \
		      $(NCURSES_CFLAGS)

pmink_test_LDADD = $(BOOST_LDLIBS) \
		   $(NCURSES_LIBS) \
		   libr14p.la \
		   libpmdaemon.la \
		   libpmconfig.la \
		   libpmutils.la \
		   libantlrutils.la \
		   libpmcli.la \
		   libr14pstats.la \
		   libr14putils.la \
		   librrp.la \
		   libpmcfg.la \
		   -ldl \
		   $(ANTLR_LDLIBS) \
		   $(LUAJIT_LIBS) \
		   $(BOOST_UNIT_TEST_FRAMEWORK_LIB) \
		   $(ICU_LIBS)

# - extra "-rpath" must be set for LuaJIT since it isn't using libtool
#   for building libluajit shared library; pmink_test libtool generated
#   wrapper script will not include a path to LuaJIT shared library
# - if we didn't set extra rpath for LuaJIT, pmink_test binary would
#   not be able to start on "make check"
if ENABLE_LUAJITBUNDLED
pmink_test_LDFLAGS = -rpath `readlink -f lib/LuaJIT-2.0.3/src`
endif

# pMINK programs
bin_PROGRAMS = configd \
	       fgnd \
	       r14ptrapc \
	       cli_service \
	       asn1c \
	       routingd \
	       configdc \
	       sgnd \
	       drd \
	       stpd \
	       pdnd

#+-----------------------------------------+
#| pMINK MNO Pattern Detection Node daemon |
#+-----------------------------------------+
pdnd_SOURCES = src/daemons/pdn/pdnd.cpp \
	       src/daemons/pdn/pdn.cpp \
	       src/daemons/pdn/pdnd_events.cpp \
	       src/daemons/pdn/cfg_events.cpp \
	       src/daemons/pdn/pdn_stats.cpp

pdnd_CPPFLAGS = $(COMMON_INCLUDES) \
		-Isrc/daemons/pdn \
		$(ANTLR_FLAGS) \
		$(BOOST_FLAGS) \
		$(NCURSES_CFLAGS) \
		$(ICU_CPPFLAGS)

pdnd_LDADD = $(BOOST_LDLIBS) \
	     $(NCURSES_LIBS) \
	     libr14p.la \
	     libpmutils.la \
	     libpmdaemon.la \
	     libr14putils.la \
	     libpmcli.la \
	     libpmconfig.la \
	     libantlrutils.la \
	     libr14pstats.la \
	     libpmcfg.la \
	     -ldl \
	     $(ANTLR_LDLIBS) \
	     $(ICU_LIBS)



#+--------------------------------------------+
#| pMINK MNO Signalling Transfer Point daemon |
#+--------------------------------------------+
stpd_SOURCES = src/daemons/stp/stpd.cpp \
	       src/daemons/stp/stp.cpp \
	       src/daemons/stp/stpd_events.cpp \
	       src/daemons/stp/cfg_events.cpp \
	       src/daemons/stp/stp_stats.cpp

stpd_CPPFLAGS = $(COMMON_INCLUDES) \
		-Isrc/daemons/stp \
		-Isrc/daemons/sgn \
		$(ANTLR_FLAGS) \
		$(BOOST_FLAGS) \
		$(NCURSES_CFLAGS) \
		$(ICU_CPPFLAGS)
	
stpd_LDADD = $(BOOST_LDLIBS) \
	     $(NCURSES_LIBS) \
	     libr14p.la \
	     libpmutils.la \
	     libpmdaemon.la \
	     libr14putils.la \
	     libpmcli.la \
	     libpmconfig.la \
	     libantlrutils.la \
	     libr14pstats.la \
	     libpmcfg.la \
	     -ldl \
	     $(ANTLR_LDLIBS) \
	     $(ICU_LIBS)


#+---------------------------------+
#| pMINK MNO Data Retention daemon |
#+---------------------------------+
drd_SOURCES = src/daemons/dr/drd.cpp \
	      src/daemons/dr/dr.cpp \
	      src/daemons/dr/drd_events.cpp \
	      src/daemons/dr/dr_stats.cpp

drd_CPPFLAGS = $(COMMON_INCLUDES) \
	       -Isrc/daemons/dr \
	       $(ANTLR_FLAGS) \
	       $(BOOST_FLAGS) \
	       $(NCURSES_CFLAGS) \
	       $(ICU_CPPFLAGS) \
	       $(LIBMYSQLCPPCONN_CXXFLAGS)

drd_LDADD = $(BOOST_LDLIBS) \
	    $(NCURSES_LIBS) \
	    libr14p.la \
	    libpmutils.la \
	    libpmdaemon.la \
	    libr14putils.la \
	    libpmcli.la \
	    libpmconfig.la \
	    libantlrutils.la \
	    libr14pstats.la \
	    libpmdb.la \
	    libpmcfg.la \
	    -ldl \
	    $(ANTLR_LDLIBS) \
	    $(ICU_LIBS) \
	    $(LIBMYSQLCPPCONN_LDFLAGS)



#+---------------------------------+
#| pMINK MNO SS7 signalling daemon |
#+---------------------------------+
sgnd_SOURCES = src/daemons/sgn/sgnd.cpp \
	       src/daemons/sgn/sgnd_events.cpp \
	       src/daemons/sgn/cfg_events.cpp \
	       src/daemons/sgn/sgn_stats.cpp \
	       src/daemons/sgn/sgn.cpp \
	       src/daemons/sgn/connection.cpp

sgnd_CPPFLAGS = $(COMMON_INCLUDES) \
		-Isrc/daemons/sgn \
		$(ANTLR_FLAGS) \
		$(ICONV_FLAGS) \
		$(PF_RING_CFLAGS) \
		$(BOOST_FLAGS) \
		$(NCURSES_CFLAGS) \
		$(ICU_CPPFLAGS)

sgnd_LDADD = $(BOOST_LDLIBS) \
	     $(NCURSES_LIBS) \
	     libr14p.la \
	     libpmutils.la \
	     libpmnet.la \
	     libpmdaemon.la \
	     libr14putils.la \
	     libpmcli.la \
	     libpmconfig.la \
	     libantlrutils.la \
	     libgsm.la \
	     libr14pstats.la \
	     libtcpip.la \
	     libpmcfg.la \
	     libpmpcap.la \
	     -ldl \
	     $(ANTLR_LDLIBS) \
	     $(ICU_LIBS) \
	     $(ICONV_LDLIBS)



#+----------------------------+
#| pMINK Config daemon client |
#+----------------------------+
configdc_SOURCES = src/config/configdcd.cpp \
		   src/config/configdc.cpp

configdc_CPPFLAGS = $(COMMON_INCLUDES) \
		    $(ANTLR_FLAGS) \
		    $(BOOST_FLAGS) \
		    $(NCURSES_CFLAGS)

configdc_LDADD = $(BOOST_LDLIBS) \
		 $(NCURSES_LIBS) \
		 libr14p.la \
		 libpmconfig.la \
		 libpmdaemon.la \
		 libpmutils.la \
		 libpmcfg.la \
		 -ldl \
		 $(ANTLR_LDLIBS)




#+----------------------------+
#| pMINK R14P routing daemon  |
#+----------------------------+
routingd_SOURCES = src/daemons/routing/routingd.cpp \
		   src/daemons/routing/routingd_events.cpp \
		   src/daemons/routing/cfg_events.cpp \
		   src/daemons/routing/routing.cpp

routingd_CPPFLAGS = $(COMMON_INCLUDES) \
		    -Isrc/daemons/routing \
		    $(ANTLR_FLAGS) \
		    $(BOOST_FLAGS) \
		    $(NCURSES_CFLAGS)

routingd_LDADD = $(BOOST_LDLIBS) \
		 $(NCURSES_LIBS) \
		 libr14p.la \
		 libpmutils.la \
		 libpmdaemon.la \
		 libpmconfig.la \
		 libantlrutils.la \
		 libpmcli.la \
		 libr14pstats.la \
		 libpmcfg.la \
		 -ldl \
		 $(ANTLR_LDLIBS)



#+--------------------------+
#| pMINK asn.1 c++ compiler |
#+--------------------------+
asn1c_SOURCES = src/asn1/asn1c/asn1Lexer.c \
		src/asn1/asn1c/asn1Parser.c \
		src/asn1/asn1c/asn1c.cpp

asn1c_CPPFLAGS = $(COMMON_INCLUDES) \
		 -Isrc/asn1/asn1c \
		 $(ANTLR_FLAGS) \
		 $(BOOST_FLAGS)

asn1c_LDADD = $(ANTLR_LDLIBS)


#+------------------------+
#| pMINK R14P CLI service |
#+------------------------+
cli_service_SOURCES = src/cli/cli_service.cpp

cli_service_CPPFLAGS = $(COMMON_INCLUDES) \
		       $(ANTLR_FLAGS) \
		       $(BOOST_FLAGS) \
		       $(NCURSES_CFLAGS)

cli_service_LDFLAGS = -export-dynamic

cli_service_LDADD = $(BOOST_LDLIBS) \
		    $(NCURSES_LIBS) \
		    libr14p.la \
		    libpmcli.la \
		    libpmutils.la \
		    libantlrutils.la \
		    libpmconfig.la \
		    libpmcfg.la \
		    -ldl \
		    $(ANTLR_LDLIBS)



#+------------------------+
#| pMINK R14P trap client |
#+------------------------+
r14ptrapc_SOURCES = src/stats/r14ptrapc.cpp

r14ptrapc_CPPFLAGS = $(COMMON_INCLUDES) \
		     $(ANTLR_FLAGS) \
		     $(BOOST_FLAGS) \
		     $(NCURSES_CFLAGS)

r14ptrapc_LDADD = $(BOOST_LDLIBS) \
		  $(NCURSES_LIBS) \
		  libr14p.la \
		  libpmutils.la \
		  -ldl \
		  $(ANTLR_LDLIBS)



#+----------------------------+
#| pMINK configuration daemon |
#+----------------------------+
configd_SOURCES = src/daemons/config/configd.cpp \
		  src/daemons/config/events.cpp

configd_CPPFLAGS = $(COMMON_INCLUDES) \
		   -Isrc/daemons/config \
		   $(ANTLR_FLAGS) \
		   $(BOOST_FLAGS) \
		   $(NCURSES_CFLAGS)

configd_LDADD = $(BOOST_LDLIBS) \
		$(NCURSES_LIBS) \
		libr14p.la \
		libpmutils.la \
		libantlrutils.la \
		libpmconfig.la \
		libpmcli.la \
		libpmdaemon.la \
		libpmcfg.la \
		-ldl \
		$(ANTLR_LDLIBS)


#+-----------------------------------+
#| MNO Filtering Gateway Node daemon |
#+-----------------------------------+
fgnd_SOURCES = src/daemons/fgn/fgnd.cpp \
	       src/daemons/fgn/fgn.cpp \
	       src/daemons/fgn/fgnd_events.cpp \
	       src/daemons/fgn/cfg_events.cpp \
	       src/daemons/fgn/fgn_stats.cpp \
	       src/daemons/fgn/lua_if.cpp

fgnd_CXXFLAGS = $(GCC_CPP14_FLAGS)

fgnd_LDADD = $(BOOST_LDLIBS) \
	     $(NCURSES_LIBS) \
	     libr14p.la \
	     libpmdaemon.la \
	     libpmconfig.la \
	     libpmutils.la \
	     libantlrutils.la \
	     libpmcli.la \
	     libr14pstats.la \
	     libr14putils.la \
	     librrp.la \
	     libpmcfg.la \
	     -ldl \
	     $(LUAJIT_LIBS) \
	     $(ANTLR_LDLIBS) \
	     $(ICU_LIBS)

fgnd_LDFLAGS = -export-dynamic

fgnd_CPPFLAGS = $(FGN_CPP14_FLAGS) \
		-DFLOGIC_PAUSE_RESUME \
		$(COMMON_INCLUDES) \
		-Isrc/daemons/fgn \
		-Isrc/daemons/sgn \
		$(ANTLR_FLAGS) \
		$(LUAJIT_CFLAGS) \
		$(BOOST_FLAGS) \
		$(ICU_CPPFLAGS) \
		$(NCURSES_CFLAGS)

# - extra "-rpath" must be set for LuaJIT since it isn't using libtool
#   for building libluajit shared library; fgnd libtool generated wrapper 
#   script will not include a path to LuaJIT shared library
# - if we didn't set extra rpath for LuaJIT, fgnd binary would
#   not be able to start without installing everything first with
#   "make install"
if ENABLE_LUAJITBUNDLED
fgnd_LDFLAGS += -rpath `readlink -f lib/LuaJIT-2.0.3/src`
endif


# post install actions
install-data-hook:
	cd $(DESTDIR)$(confdir) && \
	$(SED) -i -e "s#@PREFIX@#$(pkglibdir)#g" cli_default.pmcfg
