/*
 *  LevelMaker_UI.hh
 *
 *  Copyright (C) 2005, 2006 Gwenhael LE MOINE
 *
 *   This file is part of NGStar2
 *
 *   NGStar2 is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   NGStar2 is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with NGStar2; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#ifndef __LEVELMAKER_UI__
#  define __LEVELMAKER_UI__

/* includes */
#include <stdexcept>

#include <fltk/run.h>
#include <fltk/damage.h>
#include <fltk/Style.h>
// Window
#include <fltk/Window.h>
// Groups
#include <fltk/Group.h>
// Widgets
#include <fltk/Widget.h>
#include <fltk/Button.h>
#include <fltk/MenuBar.h>
#include <fltk/Output.h>
#include <fltk/ComboBox.h>
#include <fltk/Group.h>
#include <fltk/RadioLightButton.h>
#include <fltk/file_chooser.h>

#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

#include "../libngstar2/NGStar2.hh"
#include "../libngstar2/Configuration.hh"
#include "../src/config.h"

#include "EditorBoard.hh"

/* defines */
#define B_QUIT_WIDTH 30
#define B_RESTART_LEVEL_WIDTH 85
#define VI_CURRENT_LEVEL_WIDTH 30

/* namespaces */
using namespace fltk;
using namespace std;

class LevelMaker_UI {
private:
  //variables
  // a pointer to Configuration class
  Configuration *config;
  NGStar2 *engine;

  // filename to write the new made/modified levelto
  string *filename;

  // Windows
  Window *w_main;

  // Groups
  Group *g_controls;

  // Widgets
  // first line of controls widgets
  MenuBar *menus;

  // second line line of controls widgets
  Button *b_clear_board;
  RadioLightButton *rlb_ball;
  RadioLightButton *rlb_cube;
  RadioLightButton *rlb_empty;
  RadioLightButton *rlb_gift;
  RadioLightButton *rlb_wall;

  // Callbacks
  // Menu callbacks
  static void cb_menu_quit( Widget *, LevelMaker_UI * );
  static void cb_menu_new( Widget *, LevelMaker_UI * );
  static void cb_menu_new_levelset( Widget *, LevelMaker_UI * );
  static void cb_menu_open( Widget *, LevelMaker_UI * );
  static void cb_menu_save( Widget *, LevelMaker_UI * );
  static void cb_menu_saveas( Widget *, LevelMaker_UI * );

  static void cb_menu_license ( Widget *, void * );
  static void cb_menu_about( Widget *, void * );

  static void cb_b_clear_board( Button *b_clear_board, LevelMaker_UI *gui );
  static void cb_board( EditorBoard *board, LevelMaker_UI *gui );

  static void cb_rlb( RadioLightButton *rlb, LevelMaker_UI *gui );

  // Methods
  void save_to_file(  );

public:
  // and finally the board itself
  EditorBoard *board;
  
  LevelMaker_UI( NGStar2 *engine, Configuration *config );
  void show(  );
};

#endif /*__LEVELMAKER_UI__*/
