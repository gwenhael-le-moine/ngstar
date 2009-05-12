/*
 *  Board.hh
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

#ifndef __NGSTAR_BOARD__
#  define __NGSTAR_BOARD__

// C++ includes
#include <string>

// C includes
#include <unistd.h>

// FLTK2 includes
#include <fltk/Widget.h>
#include <fltk/SharedImage.h>
#include <fltk/Rectangle.h>
#include <fltk/events.h>
#include <fltk/ask.h>
#include <fltk/draw.h>
#include <fltk/damage.h>

// local includes
#include "../libngstar2/NGStar2.hh"
#include "../libngstar2/Configuration.hh"
#include "../libngstar2/Scores.hh"

// namespaces
using namespace fltk;
using namespace std;
using namespace ngstar2;

class Board : public Widget {
private:
  SharedImage * load_texture( string *path, string *name );

protected:
  //variables
  Configuration *config;
  NGStar2       *engine;
  Scores        *scores;

  unsigned int cell_w;
  unsigned int cell_h;
  Rectangle   *drawing_area;

  // methods
  inline void draw_cell( cell type, int x, int y );
  inline void level_over(  );

public:
  // variables
  // textures
  SharedImage *tex_ball;
  SharedImage *tex_ball_selected;
  SharedImage *tex_cube;
  SharedImage *tex_cube_selected;
  SharedImage *tex_empty;
  SharedImage *tex_gift;
  SharedImage *tex_wall;

  // Constructor
  Board( int x, int y, int w, int h, const char *label = 0 );

  // Methods
  void init( NGStar2 *engine, Configuration *config, Scores *scores );
  void load_textures(  );
  void draw(  );

  // Method overloaded
  int  handle( int event );
};

#endif /*__NGSTAR_BOARD__*/
