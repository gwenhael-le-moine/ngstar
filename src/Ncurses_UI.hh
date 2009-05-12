/*
 *  Ncurses_UI.hh
 *
 *  Copyright (C) 2003, 2004, 2005, 2006 Gwenhael LE MOINE
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

#ifndef __NCURSESUSERINTERFACE_HH__
#  define __NCURSESUSERINTERFACE_HH__

/* includes */
#include <ncurses.h>

#include "../libngstar2/NGStar2.hh"
#include "../libngstar2/Configuration.hh"
#include "../libngstar2/Scores.hh"

/* defines */
#define SYMBOL_BALL  "()"
#define SYMBOL_CUBE  "[]"
#define SYMBOL_EMPTY "  "
#define SYMBOL_GIFT  "<>"
#define SYMBOL_WALL  "##"

using namespace ngstar2;

class Ncurses_UI {
 private :
  Configuration *config;
  NGStar2 *engine;
  Scores *scores;

  WINDOW *w_main;

  enum { color_BALL = 1,
	 color_CUBE,
	 color_EMPTY,
	 color_GIFT,
	 color_WALL,
	 color_TEXT,
	 color_BALL_SELECTED,
	 color_CUBE_SELECTED,
	 color_BORDER };

  // Methods
  void display_board( void );
  void display_info_panel( void );
  void display_score( void );
  void display_finished_levelset( void );
  void clear_board( void );
  void display( void );
  void keys_handling( int key, bool allow_moving );

 public :
  // Constructor
  Ncurses_UI( NGStar2 *, Configuration *, Scores * );
  // Destructor
  ~Ncurses_UI( void );

  // Methods
  void run( void );
};
#endif /* __NCURSESUSERINTERFACE_HH__ */
