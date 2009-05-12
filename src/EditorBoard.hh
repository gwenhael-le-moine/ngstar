/*
 *  EditorBoard.hh
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

#ifndef __EDITOR_BOARD__
#  define __EDITOR_BOARD__

// C++ includes
#include <iostream>
#include <string>

// FLTK2 includes
#include <fltk/Widget.h>
#include <fltk/SharedImage.h>
#include <fltk/Rectangle.h>
#include <fltk/events.h>
#include <fltk/ask.h>

// local includes
#include "../libngstar2/NGStar2.hh"
#include "../libngstar2/Configuration.hh"
#include "../libngstar2/Scores.hh"

#include "Board.hh"

// namespaces
using namespace fltk;
using namespace std;
using namespace ngstar2;

class EditorBoard : public Board {
public:

  cell cell_to_put;
  int nb_changes_since_save;

  EditorBoard( int x, int y, int w, int h, const char *label );
  void init( NGStar2 *engine, Configuration *config );
  void draw_cell( cell type, int x, int y );
  int handle( int event );
  void clear(  );

  // Overloaded oprators
  friend ostream& operator<<(ostream& os, EditorBoard *eb )
  {
    int i, j;
    char cell;
    
    for ( i=eb->engine->level_h ; i-- ; ) {
      for ( j=eb->engine->level_w ; j-- ; ) {
	switch( eb->engine->board[i*eb->engine->level_w+j] ) {
	case ball:
	  cell = BALL;
	  break;
	case cube:
	  cell = CUBE;
	  break;
	case ngstar2::empty:
	  cell = EMPTY;
	  break;
	case gift:
	  cell = GIFT;
	  break;
	case wall:
	  cell = WALL;
	  break;
	default:
	  cerr << "What was that ?!" << endl;
	  break;
	}

	os << cell;
      }
    }
    return( os );
  }
};

#endif /*__EDITOR_BOARD__*/
