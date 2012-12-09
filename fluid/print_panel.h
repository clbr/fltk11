//
// "$Id$"
//
// FLUID print panel for the Fast Light Tool Kit (FLTK).
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

// generated by Fast Light User Interface Designer (fluid) version 1.0110

#ifndef print_panel_h
#define print_panel_h
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
extern Fl_Double_Window *print_panel;
#include <FL/Fl_Group.H>
extern Fl_Group *print_panel_controls;
#include <FL/Fl_Choice.H>
extern Fl_Choice *print_choice;
#include <FL/Fl_Button.H>
extern Fl_Button *print_properties;
#include <FL/Fl_Box.H>
extern Fl_Box *print_status;
#include <FL/Fl_Round_Button.H>
extern Fl_Round_Button *print_all;
extern Fl_Round_Button *print_pages;
extern Fl_Round_Button *print_selection;
#include <FL/Fl_Input.H>
extern Fl_Input *print_from;
extern Fl_Input *print_to;
#include <FL/Fl_Spinner.H>
extern Fl_Spinner *print_copies;
#include <FL/Fl_Check_Button.H>
extern Fl_Check_Button *print_collate_button;
extern Fl_Group *print_collate_group[2];
#include <FL/Fl_Return_Button.H>
extern void print_cb(Fl_Return_Button*, void*);
#include <FL/Fl_Progress.H>
extern Fl_Progress *print_progress;
extern Fl_Double_Window *print_properties_panel;
extern Fl_Choice *print_page_size;
extern Fl_Button *print_output_mode[4];
Fl_Double_Window* make_print_panel();
extern Fl_Menu_Item menu_print_page_size[];
extern void print_cb(Fl_Return_Button *, void *);
void print_load();
void print_update_status();
#endif

//
// End of "$Id$".
//
