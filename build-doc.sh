#!/bin/sh

doxygen
pandoc -s -S --mathjax -t revealjs --slide-level=2 Slides.md -o doc/slides.html
#pandoc -s -S --mathjax -t revealjs -f markdown_mmd --slide-level=2 --standalone -V revealjs-url=http://lab.hakim.se/reveal-js --css=http://lab.hakim.se/reveal-js/css/theme/solarized.css --css=http://lab.hakim.se/reveal-js/css/print/paper.css  Slides.md -o doc/slides.html

