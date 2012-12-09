#!/bin/sh

export LDFLAGS="-Wl,-O1 -Wl,-gc-sections -Wl,-as-needed"
export CFLAGS="-ffunction-sections -fdata-sections -Os -pipe -fomit-frame-pointer"
export CXXFLAGS="$CFLAGS -fno-rtti -fno-exceptions"

./configure --disable-gl --enable-shared
