/*
 *  EditorBoard.cc
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

#include "EditorBoard.hh"

// namespaces
using namespace fltk;
using namespace std;
using namespace ngstar2;

EditorBoard::EditorBoard( int x, int y, int w, int h, const char *label )
  : Board( x, y, w, h, label )
{

}

void EditorBoard::init( NGStar2 *engine, Configuration *config )
{
  this->config = config;
  this->engine = engine;
  
  this->load_textures(  );
  
  this->cell_w = this->w(  ) / this->engine->level_w;
  this->cell_h = this->h(  ) / this->engine->level_h;
  this->drawing_area = new Rectangle(  );
  
  this->cell_to_put = wall;
  this->nb_changes_since_save = 0;
}

void EditorBoard::draw_cell( cell type, int x, int y )
{
  SharedImage *texture;

  switch( type ) {
  case ball:
    texture = this->tex_ball;
    break;
  case cube:
    texture = this->tex_cube;
    break;
  case ngstar2::empty:
    texture = this->tex_empty;
    break;
  case gift:
    texture = this->tex_gift;
    break;
  case wall:
    texture = this->tex_wall;
    break;
  default:
    cerr << "What was that ?!" << endl;
    break;
  }

  this->drawing_area->set( x, y, this->cell_w, this->cell_h );
  texture->draw( *( this->drawing_area ) );
}

int EditorBoard::handle( int event ) {
  switch(event) {
  case PUSH:
    unsigned int mouse_X;
    unsigned int mouse_Y;
    char cell_pushed;
  
    mouse_X = this->engine->level_w - ( ( event_x() / this->cell_w ) + 1 );
    mouse_Y = this->engine->level_h - ( ( event_y() / this->cell_h ) + 1 );

    switch( this->cell_to_put ) {
    case ball:
      cell_pushed = BALL;
      break;
    case cube:
      cell_pushed = CUBE;
      break;
    case ngstar2::empty:
      cell_pushed = EMPTY;
      break;
    case gift:
      cell_pushed = GIFT;
      break;
    case wall:
      cell_pushed = WALL;
      break;
    default:
      cerr << "What was that ?!" << endl;
      break;
    }
    
    this->engine->set_cell( mouse_X, mouse_Y, cell_pushed );
    this->nb_changes_since_save++;
    
    // the callback is used to update LevelMaker_UI widgets that need it
    this->do_callback(  );
    this->redraw(  );
    return 1;
    
  case RELEASE:
    return 1;
    
  case SHORTCUT:
    return 1;
  
  case FOCUS:
    return 1;
  
  case UNFOCUS:
    return 1;
  
  case KEY:
    switch ( event_key() ) {
    case 'b' :
      this->cell_to_put = ball;
      break;
    case 'c' :
      this->cell_to_put = cube;
      break;
    case 'e' :
      this->cell_to_put = ngstar2::empty;
      break;
    case 'g' :
      this->cell_to_put = gift;
      break;
    case 'w' :
      this->cell_to_put = wall;
      break;
    default :
      break;
    }
    // the callback is used to update LevelMaker_UI widgets that need it
    this->do_callback(  );
    this->redraw(  );
    return 1;
  
  default:
    return 1;
  }
}

void EditorBoard::clear(  )
{
  int x, y;

  for ( x=NGSTAR_LEVEL_WIDTH ; x-- ; )
    for ( y=NGSTAR_LEVEL_HEIGHT ; y-- ; )
      this->engine->set_cell( x, y, EMPTY );
}
