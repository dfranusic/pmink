#!/bin/bash
# pmink
echo "Running autoreconf on pmink..."
autoreconf --install --force -I m4 > /dev/null 2>&1 || { echo "pmink autoreconf error!"; exit 1; }
# antlr3c
cd lib/libantlr3c-3.4/
echo "Running autoreconf on antlr3c..."
autoreconf --install --force > /dev/null 2>&1 || { echo "antlr3c autoreconf error!"; exit 1; }
# libiconv
cd ../libiconv-1.14/
echo "Running autoreconf on libiconv..."
autoreconf --install --force -I m4 > /dev/null 2>&1 || { echo "libiconv autoreconf error!"; exit 1; }
cd libcharset
echo "Running autoreconf on libiconv libcharset..."
autoreconf --install --force> /dev/null 2>&1 || { echo "libiconv libcharset autoreconf error!"; exit 1; }
echo "Running libiconv libcharset autogen..."
./autogen.sh > /dev/null 2>&1 || { echo "libiconv libcharset autogen error!"; exit 1; }
cd ..
echo "Running libiconv autogen..."
./autogen.sh --skip-gnulib > /dev/null 2>&1 || { echo "libiconv autogen error!"; exit 1; }
