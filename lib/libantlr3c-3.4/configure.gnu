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

./configure --prefix=$PREFIX --bindir=$PREFIX/bin/pmink --libdir=$PREFIX/lib/pmink/bundled/lib --includedir=$PREFIX/include/pmink/bundled --datarootdir=$PREFIX/share/pmink/bundled --enable-64bit
