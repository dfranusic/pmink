#!/bin/bash
cd graphics
latex 8_2_1.tex && dvips 8_2_1.dvi && ps2pdf 8_2_1.ps && pdfcrop 8_2_1.pdf 8_2_1.pdf
cd ..
pdflatex smsf
makeglossaries smsf
pdflatex smsf
pdflatex smsf
