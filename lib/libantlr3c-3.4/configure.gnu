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

./configure --prefix=$PREFIX --enable-64bit
