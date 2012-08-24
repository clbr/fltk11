#!/bin/sh

export LDFLAGS="-Wl,-O1 -Wl,-gc-sections"
export CFLAGS="-ffunction-sections -fdata-sections -Os -s -pipe -fomit-frame-pointer"
export CXXFLAGS="$CFLAGS -fno-rtti -fno-exceptions"

./configure --disable-gl
