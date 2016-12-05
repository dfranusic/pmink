#!/bin/bash

ARGS=`getopt -q -o' ' -l prefix: -- "$@"`

eval set -- "$ARGS";


while true; do
  case "$1" in
    --prefix ) 
        shift;
        PREFIX=$1
        ;;
    -- ) shift; break ;;
    * ) break ;;
  esac
done

if [ -z "$PREFIX" ];
then
  exit 1
fi

sed -e "s#/usr/local#${PREFIX}#" Makefile.orig > Makefile
echo -e "maintainer-clean:\n" >> Makefile
echo -e "check:\n" >> Makefile
sed -e "s#/usr/local#${PREFIX}#" src/luaconf.h.orig > src/luaconf.h
