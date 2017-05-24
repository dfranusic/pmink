#!/bin/bash

# get full dist dir
DIST=`readlink -f $1`

# get versions
PMINK_VER_PMINK=`cat $DIST/usr/share/pmink/VERSION|head -1|sed -e 's/^v//g'`

# create dirs
mkdir -p tmp_deb
mkdir -p tmp_deb/pmink-pmink_$PMINK_VER_PMINK/DEBIAN
mkdir -p tmp_deb/pmink-pmink_$PMINK_VER_PMINK/usr
mkdir -p tmp_deb/pmink-pmink_$PMINK_VER_PMINK/usr/local
mkdir -p tmp_deb/pmink-pmink_$PMINK_VER_PMINK/usr/local/pmink
mkdir -p tmp_deb/pmink-pmink_$PMINK_VER_PMINK/etc
mkdir -p tmp_deb/pmink-pmink_$PMINK_VER_PMINK/etc/init.d
mkdir -p tmp_deb/pmink-pmink_$PMINK_VER_PMINK/etc/default
mkdir -p tmp_deb/pmink-pmink_$PMINK_VER_PMINK/usr/share
mkdir -p tmp_deb/pmink-pmink_$PMINK_VER_PMINK/usr/share/doc
mkdir -p tmp_deb/pmink-pmink_$PMINK_VER_PMINK/usr/share/doc/pmink

# remove sticky bit if set on DEBIAN directory
chmod -s tmp_deb/pmink-pmink_$PMINK_VER_PMINK/DEBIAN

# create .deb control files 
sed "s/Version:/Version: $PMINK_VER_PMINK/g" control-pmink.tmpl > tmp_deb/pmink-pmink_$PMINK_VER_PMINK/DEBIAN/control

# copy files
cp -a $DIST/* tmp_deb/pmink-pmink_$PMINK_VER_PMINK

# copy init scripts and config files
cp ../init/debian/* tmp_deb/pmink-pmink_$PMINK_VER_PMINK/etc/init.d
cp ../init/debian/*.cfg tmp_deb/pmink-pmink_$PMINK_VER_PMINK/etc/default
rm tmp_deb/pmink-pmink_$PMINK_VER_PMINK/etc/init.d/*.cfg
find tmp_deb/pmink-pmink_$PMINK_VER_PMINK/etc/default -type f -name '*.cfg'| while read f; do mv "$f" "${f%.cfg}"; done

# create .deb conffiles files
ls tmp_deb/pmink-pmink_$PMINK_VER_PMINK/etc/init.d|sed 's/^/\/etc\/init.d\//' > tmp_deb/pmink-pmink_$PMINK_VER_PMINK/DEBIAN/conffiles
ls tmp_deb/pmink-pmink_$PMINK_VER_PMINK/etc/default|sed 's/^/\/etc\/default\//' >> tmp_deb/pmink-pmink_$PMINK_VER_PMINK/DEBIAN/conffiles

# create changelog
echo -e "pmink ($PMINK_VER_PMINK) unstable; urgency=low\n" > tmp_deb/pmink-pmink_$PMINK_VER_PMINK/DEBIAN/changelog.Debian
git log -10 --pretty --numstat --summary --no-merges | git2cl >> tmp_deb/pmink-pmink_$PMINK_VER_PMINK/DEBIAN/changelog.Debian
gzip --best tmp_deb/pmink-pmink_$PMINK_VER_PMINK/DEBIAN/changelog.Debian
mv tmp_deb/pmink-pmink_$PMINK_VER_PMINK/DEBIAN/changelog.Debian.gz tmp_deb/pmink-pmink_$PMINK_VER_PMINK/usr/share/doc/pmink

# create ld.so.conf.d config file
cp postinst tmp_deb/pmink-pmink_$PMINK_VER_PMINK/DEBIAN
cp postrm tmp_deb/pmink-pmink_$PMINK_VER_PMINK/DEBIAN

# crete .deb packages
dpkg --build tmp_deb/pmink-pmink_$PMINK_VER_PMINK .
rm -rf tmp_deb
