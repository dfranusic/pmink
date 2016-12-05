#!/bin/bash

# get full dist dir
DIST=`readlink -f $1`

# get versions
PMINK_VER_MNO=`cat $DIST/pmink/VERSION.mno|head -1|sed -e 's/mno-v//g'`
PMINK_VER_OPENLI=`cat $DIST/pmink/VERSION.openli|head -1|sed -e 's/openli-v//g'`
PMINK_VER_PMINK=`cat $DIST/pmink/VERSION.pmink|head -1|sed -e 's/pmink-v//g'`

# create dirs
mkdir -p $DIST/debian
mkdir -p $DIST/debian/pmink-mno_$PMINK_VER_MNO
mkdir -p $DIST/debian/pmink-mno_$PMINK_VER_MNO/DEBIAN
mkdir -p $DIST/debian/pmink-mno_$PMINK_VER_MNO/usr
mkdir -p $DIST/debian/pmink-mno_$PMINK_VER_MNO/usr/local
mkdir -p $DIST/debian/pmink-mno_$PMINK_VER_MNO/usr/local/pmink
mkdir -p $DIST/debian/pmink-mno_$PMINK_VER_MNO/etc
mkdir -p $DIST/debian/pmink-mno_$PMINK_VER_MNO/etc/init.d
mkdir -p $DIST/debian/pmink-mno_$PMINK_VER_MNO/etc/default
mkdir -p $DIST/debian/pmink-mno_$PMINK_VER_MNO/usr/share
mkdir -p $DIST/debian/pmink-mno_$PMINK_VER_MNO/usr/share/doc
mkdir -p $DIST/debian/pmink-mno_$PMINK_VER_MNO/usr/share/doc/pmink-mno
mkdir -p $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/DEBIAN
mkdir -p $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/usr
mkdir -p $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/usr/local
mkdir -p $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/usr/local/pmink
mkdir -p $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/etc
mkdir -p $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/etc/init.d
mkdir -p $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/etc/default
mkdir -p $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/usr/share
mkdir -p $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/usr/share/doc
mkdir -p $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/usr/share/doc/pmink-openli
mkdir -p $DIST/debian/pmink-pmink_$PMINK_VER_PMINK/DEBIAN
mkdir -p $DIST/debian/pmink-pmink_$PMINK_VER_PMINK/usr
mkdir -p $DIST/debian/pmink-pmink_$PMINK_VER_PMINK/usr/local
mkdir -p $DIST/debian/pmink-pmink_$PMINK_VER_PMINK/usr/local/pmink
mkdir -p $DIST/debian/pmink-pmink_$PMINK_VER_PMINK/etc
mkdir -p $DIST/debian/pmink-pmink_$PMINK_VER_PMINK/etc/init.d
mkdir -p $DIST/debian/pmink-pmink_$PMINK_VER_PMINK/etc/default
mkdir -p $DIST/debian/pmink-pmink_$PMINK_VER_PMINK/usr/share
mkdir -p $DIST/debian/pmink-pmink_$PMINK_VER_PMINK/usr/share/doc
mkdir -p $DIST/debian/pmink-pmink_$PMINK_VER_PMINK/usr/share/doc/pmink

# remove sticky bit if set on DEBIAN directory
chmod -s $DIST/debian/pmink-mno_$PMINK_VER_MNO/DEBIAN
chmod -s $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/DEBIAN
chmod -s $DIST/debian/pmink-pmink_$PMINK_VER_PMINK/DEBIAN

# create .deb control files 
sed "s/Version:/Version: $PMINK_VER_MNO/g" control-mno.tmpl > $DIST/debian/pmink-mno_$PMINK_VER_MNO/DEBIAN/control
sed "s/Version:/Version: $PMINK_VER_OPENLI/g" control-openli.tmpl > $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/DEBIAN/control
sed "s/Version:/Version: $PMINK_VER_PMINK/g" control-pmink.tmpl > $DIST/debian/pmink-pmink_$PMINK_VER_PMINK/DEBIAN/control

# copy files
cp -a $DIST/pmink.mno/* $DIST/debian/pmink-mno_$PMINK_VER_MNO/usr/local/pmink
cp -a $DIST/pmink.openli/* $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/usr/local/pmink
cp -a $DIST/pmink/* $DIST/debian/pmink-pmink_$PMINK_VER_PMINK/usr/local/pmink

# copy init scripts and config files
cp ../init/debian/* $DIST/debian/pmink-pmink_$PMINK_VER_PMINK/etc/init.d
cp ../init/debian/*.cfg $DIST/debian/pmink-pmink_$PMINK_VER_PMINK/etc/default
rm $DIST/debian/pmink-pmink_$PMINK_VER_PMINK/etc/init.d/*.cfg
find $DIST/debian/pmink-pmink_$PMINK_VER_PMINK/etc/default -type f -name '*.cfg'| while read f; do mv "$f" "${f%.cfg}"; done
cp $DIST/debian/pmink-pmink_$PMINK_VER_PMINK/etc/init.d/* $DIST/debian/pmink-mno_$PMINK_VER_MNO/etc/init.d
cp $DIST/debian/pmink-pmink_$PMINK_VER_PMINK/etc/init.d/* $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/etc/init.d
cp $DIST/debian/pmink-pmink_$PMINK_VER_PMINK/etc/default/* $DIST/debian/pmink-mno_$PMINK_VER_MNO/etc/default
cp $DIST/debian/pmink-pmink_$PMINK_VER_PMINK/etc/default/* $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/etc/default
rm $DIST/debian/pmink-mno_$PMINK_VER_MNO/etc/init.d/pmink_openli*
rm $DIST/debian/pmink-mno_$PMINK_VER_MNO/etc/default/pmink_openli*
rm  $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/etc/init.d/pmink_drd
rm  $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/etc/init.d/pmink_fgnd
rm  $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/etc/init.d/pmink_pdnd
rm  $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/etc/init.d/pmink_psgnd
rm  $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/etc/init.d/pmink_sgnd
rm  $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/etc/init.d/pmink_smshubd
rm  $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/etc/init.d/pmink_stpd
rm  $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/etc/default/pmink_drd
rm  $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/etc/default/pmink_fgnd
rm  $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/etc/default/pmink_pdnd
rm  $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/etc/default/pmink_psgnd
rm  $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/etc/default/pmink_sgnd
rm  $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/etc/default/pmink_smshubd
rm  $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/etc/default/pmink_stpd

# create .deb conffiles files
ls $DIST/debian/pmink-pmink_$PMINK_VER_PMINK/etc/init.d|sed 's/^/\/etc\/init.d\//' > $DIST/debian/pmink-pmink_$PMINK_VER_PMINK/DEBIAN/conffiles
ls $DIST/debian/pmink-pmink_$PMINK_VER_PMINK/etc/default|sed 's/^/\/etc\/default\//' >> $DIST/debian/pmink-pmink_$PMINK_VER_PMINK/DEBIAN/conffiles
ls $DIST/debian/pmink-mno_$PMINK_VER_MNO/etc/init.d|sed 's/^/\/etc\/init.d\//' > $DIST/debian/pmink-mno_$PMINK_VER_MNO/DEBIAN/conffiles
ls $DIST/debian/pmink-mno_$PMINK_VER_MNO/etc/default|sed 's/^/\/etc\/default\//' >> $DIST/debian/pmink-mno_$PMINK_VER_MNO/DEBIAN/conffiles
ls $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/etc/init.d|sed 's/^/\/etc\/init.d\//' > $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/DEBIAN/conffiles
ls $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/etc/default|sed 's/^/\/etc\/default\//' >> $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/DEBIAN/conffiles

# create changelog
echo -e "pmink ($PMINK_VER_PMINK) unstable; urgency=low\n" > $DIST/debian/pmink-pmink_$PMINK_VER_PMINK/DEBIAN/changelog.Debian
echo -e "pmink-mno ($PMINK_VER_MNO) unstable; urgency=low\n" > $DIST/debian/pmink-mno_$PMINK_VER_MNO/DEBIAN/changelog.Debian
echo -e "pmink-openli ($PMINK_VER_OPENLI) unstable; urgency=low\n" > $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/DEBIAN/changelog.Debian
git log -10 --pretty --numstat --summary --no-merges | git2cl >> $DIST/debian/pmink-pmink_$PMINK_VER_PMINK/DEBIAN/changelog.Debian
git log -10 --pretty --numstat --summary --no-merges | git2cl >> $DIST/debian/pmink-mno_$PMINK_VER_MNO/DEBIAN/changelog.Debian
git log -10 --pretty --numstat --summary --no-merges | git2cl >> $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/DEBIAN/changelog.Debian
gzip --best $DIST/debian/pmink-pmink_$PMINK_VER_PMINK/DEBIAN/changelog.Debian
gzip --best $DIST/debian/pmink-mno_$PMINK_VER_MNO/DEBIAN/changelog.Debian
gzip --best $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/DEBIAN/changelog.Debian
mv $DIST/debian/pmink-pmink_$PMINK_VER_PMINK/DEBIAN/changelog.Debian.gz $DIST/debian/pmink-pmink_$PMINK_VER_PMINK/usr/share/doc/pmink
mv $DIST/debian/pmink-mno_$PMINK_VER_MNO/DEBIAN/changelog.Debian.gz $DIST/debian/pmink-mno_$PMINK_VER_MNO/usr/share/doc/pmink-mno
mv $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/DEBIAN/changelog.Debian.gz $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/usr/share/doc/pmink-openli

# create ld.so.conf.d config file
cp postinst $DIST/debian/pmink-pmink_$PMINK_VER_PMINK/DEBIAN
cp postrm $DIST/debian/pmink-pmink_$PMINK_VER_PMINK/DEBIAN
cp postinst $DIST/debian/pmink-mno_$PMINK_VER_MNO/DEBIAN
cp postrm $DIST/debian/pmink-mno_$PMINK_VER_MNO/DEBIAN
cp postinst $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/DEBIAN
cp postrm $DIST/debian/pmink-openli_$PMINK_VER_OPENLI/DEBIAN

# crete .deb packages
dpkg --build $DIST/debian/pmink-mno_$PMINK_VER_MNO $DIST/debian
dpkg --build $DIST/debian/pmink-openli_$PMINK_VER_OPENLI $DIST/debian
dpkg --build $DIST/debian/pmink-pmink_$PMINK_VER_PMINK $DIST/debian
