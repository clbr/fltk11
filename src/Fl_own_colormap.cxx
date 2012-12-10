//
// "$Id: Fl_own_colormap.cxx 5190 2006-06-09 16:16:34Z mike $"
//
// Private colormap support for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2005 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

// Using the default system colormap can be a bad idea on PseudoColor
// visuals, since typically every application uses the default colormap and
// you can run out of colormap entries easily.
//
// The solution is to always create a new colormap on PseudoColor displays
// and copy the first 16 colors from the default colormap so that we won't
// get huge color changes when switching windows.

#include <config.h>
#include <FL/Fl.H>
#include <FL/x.H>

#ifdef WIN32
// There is probably something relevant to do on MSWindows 8-bit displays
// but I don't know what it is

void Fl::own_colormap() {}

#elif defined(__APPLE__)
// MacOS X always provides a TrueColor interface...

void Fl::own_colormap() {}
#else
// X version

void Fl::own_colormap() {
  fl_open_display();
}

#endif

//
// End of "$Id: Fl_own_colormap.cxx 5190 2006-06-09 16:16:34Z mike $".
//
