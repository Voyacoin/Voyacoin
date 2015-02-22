#!/bin/bash
# create multiresolution windows icon
ICON_SRC=../../src/qt/res/icons/voyacoin.png
ICON_DST=../../src/qt/res/icons/voyacoin.ico
convert ${ICON_SRC} -resize 16x16 voyacoin-16.png
convert ${ICON_SRC} -resize 32x32 voyacoin-32.png
convert ${ICON_SRC} -resize 48x48 voyacoin-48.png
convert voyacoin-16.png voyacoin-32.png voyacoin-48.png ${ICON_DST}

