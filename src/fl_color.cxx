//
// "$Id: fl_color.cxx 5835 2007-05-16 11:46:07Z matt $"
//
// Color functions for the Fast Light Tool Kit (FLTK).
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

// Implementation of fl_color(i), fl_color(r,g,b).

#ifdef WIN32
#  include "fl_color_win32.cxx"
#elif defined(__APPLE__)
#  include "fl_color_mac.cxx"
#else

// Also code to look at the X visual and figure out the best way to turn
// a color into a pixel value.

// SGI compiler seems to have problems with unsigned char arguments
// being used to index arrays.  So I always copy them to an integer
// before use.

#  include "Fl_XColor.H"
#  include <FL/Fl.H>
#  include <FL/x.H>
#  include <FL/fl_draw.H>

////////////////////////////////////////////////////////////////
// figure_out_visual() calculates masks & shifts for generating
// pixels in true-color visuals:

uchar fl_redmask, fl_greenmask, fl_bluemask;
int fl_redshift, fl_greenshift, fl_blueshift, fl_extrashift;
static uchar beenhere;

static void figure_out_visual() {
  beenhere = 1;
  if (!fl_visual->red_mask || !fl_visual->green_mask || !fl_visual->blue_mask){
    Fl::fatal("Requires true color visual");
  }

  // get the bit masks into a more useful form:
  int i,j,m;

  for (i = 0, m = 1; m; i++, m<<=1) if (fl_visual->red_mask & m) break;
  for (j = i; m; j++, m<<=1) if (!(fl_visual->red_mask & m)) break;
  fl_redshift = j-8;
  fl_redmask = (j-i >= 8) ? 0xFF : 0xFF-(255>>(j-i));

  for (i = 0, m = 1; m; i++, m<<=1) if (fl_visual->green_mask & m) break;
  for (j = i; m; j++, m<<=1) if (!(fl_visual->green_mask & m)) break;
  fl_greenshift = j-8;
  fl_greenmask = (j-i >= 8) ? 0xFF : 0xFF-(255>>(j-i));

  for (i = 0, m = 1; m; i++, m<<=1) if (fl_visual->blue_mask & m) break;
  for (j = i; m; j++, m<<=1) if (!(fl_visual->blue_mask & m)) break;
  fl_blueshift = j-8;
  fl_bluemask = (j-i >= 8) ? 0xFF : 0xFF-(255>>(j-i));

  i = fl_redshift;
  if (fl_greenshift < i) i = fl_greenshift;
  if (fl_blueshift < i) i = fl_blueshift;
  if (i < 0) {
    fl_extrashift = -i;
    fl_redshift -= i; fl_greenshift -= i; fl_blueshift -= i;
  } else
    fl_extrashift = 0;

}

static unsigned fl_cmap[256] = {
#include "fl_cmap.h" // this is a file produced by "cmap.cxx":
};

static Fl_XColor fl_xmap[256];

////////////////////////////////////////////////////////////////
// Get an rgb color.  This is easy for a truecolor visual.  For
// colormapped it picks the closest color out of the cube in the
// fltk colormap.  However if this color cube entry has been
// requested before, you will get the earlier requested color, and
// even this may be approximated if the X colormap was full.

ulong fl_xpixel(uchar r,uchar g,uchar b) {
  if (!beenhere) figure_out_visual();
  return
    (((r&fl_redmask) << fl_redshift)+
     ((g&fl_greenmask)<<fl_greenshift)+
     ((b&fl_bluemask)<< fl_blueshift)
     ) >> fl_extrashift;
}

void fl_color(uchar r,uchar g,uchar b) {
  fl_color_ = fl_rgb_color(r, g, b);
  XSetForeground(fl_display, fl_gc, fl_xpixel(r,g,b));
}

////////////////////////////////////////////////////////////////
// Get a color out of the the fltk colormap.  Again for truecolor
// visuals this is easy.  For colormap this actually tries to allocate
// an X color, and does a least-squares match to find the closest
// color if X cannot allocate that color.

// calculate what color is actually on the screen for a mask:
static inline uchar realcolor(uchar color, uchar mask) {
#  if 0
  // accurate version if the display has linear gamma, but fl_draw_image
  // works better with the simpler version on most screens...
  uchar m = mask;
  uchar result = color&m;
  for (;;) {
    while (m&mask) {m>>=1; color>>=1;}
    if (!m) break;
    mask = m;
    result |= color&m;
  }
  return result;
#  else
  return (color&mask) | (~mask)&(mask>>1);
#  endif
}

ulong fl_xpixel(Fl_Color i) {
  if (i & 0xffffff00) {
    return fl_xpixel((i >> 24) & 255, (i >> 16) & 255, (i >> 8) & 255);
  }

  Fl_XColor &xmap = fl_xmap[i];
  if (xmap.mapped) return xmap.pixel;

  if (!beenhere) figure_out_visual();

  uchar r,g,b;
  {unsigned c = fl_cmap[i]; r=uchar(c>>24); g=uchar(c>>16); b=uchar(c>>8);}

    // return color for a truecolor visual:
    xmap.mapped = 2; // 2 prevents XFreeColor from being called
    xmap.r = realcolor(r, fl_redmask);
    xmap.g = realcolor(g, fl_greenmask);
    xmap.b = realcolor(b, fl_bluemask);
    return xmap.pixel =
      (((r&fl_redmask) << fl_redshift)+
       ((g&fl_greenmask)<<fl_greenshift)+
       ((b&fl_bluemask)<< fl_blueshift)
       ) >> fl_extrashift;
}

Fl_Color fl_color_;

void fl_color(Fl_Color i) {
  if (i & 0xffffff00) {
    unsigned rgb = (unsigned)i;
    fl_color((uchar)(rgb >> 24), (uchar)(rgb >> 16), (uchar)(rgb >> 8));
  } else {
    fl_color_ = i;
    XSetForeground(fl_display, fl_gc, fl_xpixel(i));
  }
}

void Fl::free_color(Fl_Color i, int overlay) {
  if (overlay) return;
  if (fl_xmap[i].mapped) {
    fl_xmap[i].mapped = 0;
  }
}

void Fl::set_color(Fl_Color i, unsigned c) {
  if (fl_cmap[i] != c) {
    free_color(i,0);
    fl_cmap[i] = c;
  }
}

#endif // end of X-specific code

unsigned Fl::get_color(Fl_Color i) {
  if (i & 0xffffff00) return (i);
  else return fl_cmap[i];
}

void Fl::set_color(Fl_Color i, uchar red, uchar green, uchar blue) {
  Fl::set_color((Fl_Color)(i & 255),
	((unsigned)red<<24)+((unsigned)green<<16)+((unsigned)blue<<8));
}

void Fl::get_color(Fl_Color i, uchar &red, uchar &green, uchar &blue) {
  unsigned c;

  if (i & 0xffffff00) c = (unsigned)i;
  else c = fl_cmap[i];

  red   = uchar(c>>24);
  green = uchar(c>>16);
  blue  = uchar(c>>8);
}

Fl_Color fl_color_average(Fl_Color color1, Fl_Color color2, float weight) {
  unsigned rgb1;
  unsigned rgb2;
  uchar r, g, b;

  if (color1 & 0xffffff00) rgb1 = color1;
  else rgb1 = fl_cmap[color1 & 255];

  if (color2 & 0xffffff00) rgb2 = color2;
  else rgb2 = fl_cmap[color2 & 255];

  r = (uchar)(((uchar)(rgb1>>24))*weight + ((uchar)(rgb2>>24))*(1-weight));
  g = (uchar)(((uchar)(rgb1>>16))*weight + ((uchar)(rgb2>>16))*(1-weight));
  b = (uchar)(((uchar)(rgb1>>8))*weight + ((uchar)(rgb2>>8))*(1-weight));

  return fl_rgb_color(r, g, b);
}

Fl_Color fl_inactive(Fl_Color c) {
  return fl_color_average(c, FL_GRAY, .33f);
}

Fl_Color fl_contrast(Fl_Color fg, Fl_Color bg) {
  unsigned c1, c2;	// RGB colors
  int l1, l2;		// Luminosities


  // Get the RGB values for each color...
  if (fg & 0xffffff00) c1 = (unsigned)fg;
  else c1 = fl_cmap[fg];

  if (bg & 0xffffff00) c2 = (unsigned)bg;
  else c2 = fl_cmap[bg];

  // Compute the luminosity...
  l1 = ((c1 >> 24) * 30 + ((c1 >> 16) & 255) * 59 + ((c1 >> 8) & 255) * 11) / 100;
  l2 = ((c2 >> 24) * 30 + ((c2 >> 16) & 255) * 59 + ((c2 >> 8) & 255) * 11) / 100;

  // Compare and return the contrasting color...
  if ((l1 - l2) > 99) return fg;
  else if ((l2 - l1) > 99) return fg;
  else if (l2 > 127) return FL_BLACK;
  else return FL_WHITE;
}

//
// End of "$Id: fl_color.cxx 5835 2007-05-16 11:46:07Z matt $".
//
