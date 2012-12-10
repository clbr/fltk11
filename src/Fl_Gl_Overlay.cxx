//
// "$Id: Fl_Gl_Overlay.cxx 5350 2006-08-23 17:40:54Z mike $"
//
// OpenGL overlay code for the Fast Light Tool Kit (FLTK).
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

#include <config.h>
#if HAVE_GL

#include <FL/Fl.H>
#include <FL/x.H>
#include "Fl_Gl_Choice.H"
#include <FL/Fl_Gl_Window.H>
#include <stdlib.h>


int Fl_Gl_Window::can_do_overlay() {return 0;}

void Fl_Gl_Window::make_overlay() {overlay = this;}


void Fl_Gl_Window::redraw_overlay() {
  if (!shown()) return;
  make_overlay();
#ifdef __APPLE__
  redraw();
#else
#ifndef WIN32
  if (overlay != this)
    ((Fl_Gl_Window*)overlay)->redraw();
  else
#endif
    damage(FL_DAMAGE_OVERLAY);
#endif
}

void Fl_Gl_Window::make_overlay_current() {
  make_overlay();
#ifdef __APPLE__
  // this is not very useful, but unfortunatly, Apple decided
  // that front buffer drawing can no longer (OS X 10.4) be
  // supported on their platforms.
  make_current();
#else
    glDrawBuffer(GL_FRONT);
#endif
}

void Fl_Gl_Window::hide_overlay() {
}

#endif

//
// End of "$Id: Fl_Gl_Overlay.cxx 5350 2006-08-23 17:40:54Z mike $".
//
