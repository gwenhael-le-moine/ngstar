/*
 *  Fltk2_UI.hh
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

#ifndef __FLTK2_UI__
#  define __FLTK2_UI__

/* includes */
#include <fltk/run.h>
#include <fltk/damage.h>
#include <fltk/Style.h>
// Window
#include <fltk/Window.h>
// Widgets
#include <fltk/Widget.h>
#include <fltk/MenuBar.h>
#include <fltk/ValueInput.h>
#include <fltk/Output.h>
#include <fltk/Item.h>
#include <fltk/ItemGroup.h>

#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

#include "../libngstar2/NGStar2.hh"
#include "../libngstar2/Configuration.hh"
#include "../libngstar2/Scores.hh"

#include "config.h"
#include "Board.hh"

/* defines */
#define B_QUIT_WIDTH 30
#define B_RESTART_LEVEL_WIDTH 85
#define VI_CURRENT_LEVEL_WIDTH 30

/* namespaces */
using namespace fltk;
using namespace std;

class Fltk2_UI {
private:
  //variables
  // a pointer to Configuration class
  Configuration *config;
  NGStar2 *engine;
  Scores *scores;

  // Windows
  Window *w_main;

  // Widgets
  // first line of controls widgets
  MenuBar *menus;
  ItemGroup *sm_levelsets;
  ItemGroup *sm_themes;

  // second line line of controls widgets
  ValueInput *vi_current_level;
  Output *o_nb_gifts;
  Output *o_nb_moves;

  // and finally the board itself
  Board *board;

  void make_list( string *path, string *items, ItemGroup *menu, void (cb)(fltk::Item*, Fltk2_UI*) );

  // Callbacks
  static void cb_menu_quit( Widget *, void * );
  static void cb_menu_show_scores( Widget *, Fltk2_UI *gui );
  static void cb_menu_license ( Widget *, void * );
  static void cb_menu_about( Widget *, void * );
  static void cb_menu_restart_level( Widget *, Fltk2_UI *gui );

  static void cb_vi_current_level( ValueInput *vi_current_level, Fltk2_UI *gui );
  static void cb_menu_levelset( Item *m_levelset, Fltk2_UI *gui );
  static void cb_menu_theme( Item *m_theme, Fltk2_UI *gui );
  static void cb_o_nb_moves( Output *o_nb_moves, Fltk2_UI *gui );
  static void cb_o_nb_gifts( Output *o_nb_gifts, Fltk2_UI *gui );
  static void cb_board( Board *board, Fltk2_UI *gui );

public:
  Fltk2_UI( NGStar2 *engine, Configuration *config, Scores *scores_holder );
  void show(  );
};

#endif /*__FLTK2_UI__*/
