/*
 *  Board.cc
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

#include "Board.hh"

// namespaces
using namespace fltk;
using namespace std;
using namespace ngstar2;

Board::Board(int x, int y, int w, int h, const char *label)
: fltk::Widget(x, y, w, h, label)
{
  // do initialization stuff...
}

void Board::init( NGStar2 *engine, Configuration *config, Scores *scores )
{
  this->config = config;
  this->engine = engine;
  this->scores = scores;

  this->load_textures(  );

  this->cell_w = this->w(  ) / this->engine->level_w;
  this->cell_h = this->h(  ) / this->engine->level_h;

  drawing_area = new Rectangle(  );
}

inline void Board::level_over(  )
{
  this->scores->set(  );
  
  if ( this->config->current_level == this->config->nb_levels ) {
    int i;
    bool tmp = true;
    string *message = new string( "This levelset ends here !" );

    for ( i=0 ; i<this->config->nb_levels ; ++i ) {
      if ( this->scores->get( i ) == -1 ) {
	if ( tmp == true )
	  message->append( "\n\n\tLevels not cleared :\n" );
	tmp = false;
	message->append( itos( i + 1 ) );
	message->append( " " );
      }
    }

    fltk::message( message->c_str(  ) );
    exit( 0 );
  }
  else {
    try {
      this->engine->next_level(  );
    }
    catch ( LoadingLevelException lle ) {
      cerr << lle << endl;
      exit( -1 );
    }
  }
}

inline void Board::draw_cell( cell type, int x, int y )
{
  SharedImage *texture;

  switch( type ) {
  case ball:
    texture = ( this->engine->moving == ball ) ? this->tex_ball_selected : this->tex_ball;
    break;
  case cube:
    texture = ( this->engine->moving == cube ) ? this->tex_cube_selected : this->tex_cube;
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
  }

  this->drawing_area->set( x, y, this->cell_w, this->cell_h );
  texture->draw( *( this->drawing_area ) );
}

void Board::draw(  )
{
  int i, j;
  unsigned int cell_x, cell_y;

  for ( i=this->engine->level_h ; i-- ; ) {
    for ( j=this->engine->level_w ; j-- ; ) {
      this->cell_w = this->w(  ) / this->engine->level_w;
      this->cell_h = this->h(  ) / this->engine->level_h;
      cell_y = ( this->engine->level_h - i - 1 ) * this->cell_h;
      cell_x = ( this->engine->level_w - j - 1) * this->cell_w;
      
      this->draw_cell( this->engine->board[i*this->engine->level_w+j], cell_x, cell_y );
    }
  }
}

SharedImage * Board::load_texture( string *path, string *name )
{
  return( pngImage::get( ( *path + *name ).c_str(  ) ) );
}

void Board::load_textures(  )
{
  string *themepath = new string( this->config->themes_path->c_str(  ) );

  themepath->append( "/" );
  themepath->append( this->config->theme->c_str(  ) );
  themepath->append( "/" );
  
  this->tex_ball = this->load_texture( themepath, new string( "tex_ball.png" ) );
  this->tex_ball_selected = this->load_texture( themepath, new string( "tex_ball_selected.png" ) );
  this->tex_cube = this->load_texture( themepath, new string( "tex_cube.png" ) );
  this->tex_cube_selected = this->load_texture( themepath, new string( "tex_cube_selected.png" ) );
  this->tex_empty = this->load_texture( themepath, new string( "tex_empty.png" ) );
  this->tex_gift = this->load_texture( themepath, new string( "tex_gift.png" ) );
  this->tex_wall = this->load_texture( themepath, new string( "tex_wall.png" ) );
}

int Board::handle( int event ) {
  switch(event) {
  case PUSH:
    unsigned int mouse_X;
    unsigned int mouse_Y;
  
    mouse_X = this->engine->level_w - ( ( event_x() / this->cell_w ) + 1 );
    mouse_Y = this->engine->level_h - ( ( event_y() / this->cell_h ) + 1 );
    if ( ( this->engine->ball_X == mouse_X ) && 
	 ( this->engine->ball_Y == mouse_Y ) )
      this->engine->moving = ball;
    else {
      if ( ( this->engine->cube_X == mouse_X ) && 
	   ( this->engine->cube_Y == mouse_Y ) )
	this->engine->moving = cube;
      else {
	unsigned int selected_X;
	unsigned int selected_Y;

	if ( this->engine->moving == ball ) {
	  selected_X = this->engine->ball_X;
	  selected_Y = this->engine->ball_Y;
	}
	else {
	  selected_X = this->engine->cube_X;
	  selected_Y = this->engine->cube_Y;
	}

	if ( mouse_X == selected_X ) {
	  if ( mouse_Y > selected_Y )
	    this->engine->move( go_up );
	  else
	    this->engine->move( go_down );
	}
	else {
	  if ( mouse_Y == selected_Y ) {
	    if ( mouse_X > selected_X )
	      this->engine->move( go_left );
	    else
	      this->engine->move( go_right );
	  }
	}
      }
      if ( this->engine->is_it_over(  ) == true ) {
	// the callback is used to update Fltk2_UI widgets that need it
	this->do_callback(  );
	
	this->level_over(  );
      }
    }
    // the callback is used to update Fltk2_UI widgets that need it
    this->do_callback(  );
    return 1;

  case RELEASE:
    return 1;
    
  case SHORTCUT:
    return 1;
  
  case FOCUS:
    focus( this );
    return 1;
  
  case UNFOCUS:
    return 1;
  
  case KEY:
    if ( this->engine->is_it_over(  ) == false )
      switch ( event_key() ) {
      case UpKey :
	this->engine->move( go_up );
	break;
      case DownKey :
	this->engine->move( go_down );
	break;
      case LeftKey :
	this->engine->move( go_left );
	break;
      case RightKey :
	this->engine->move( go_right );
	break;
      case ' ' :
	this->engine->switch_moving();
	break;
      case 'n' :
	try {
	  this->engine->next_level(  );
	}
	catch ( InvalidLevelFile ilf ) {
	  cerr << ilf << endl;
	  exit( -1 );
	}
	catch ( InvalidCellValue icv ) {
	  cerr << icv << endl;
	  exit( -1 );
	}
	catch ( InvalidCellCoordinates icc ) {
	  cerr << icc << endl;
	  exit( -1 );
	}
	break;
      case 'p' :
	try {
	  this->engine->previous_level(  );
	}
	catch ( InvalidLevelFile ilf ) {
	  cerr << ilf << endl;
	  exit( -1 );
	}
	catch ( InvalidCellValue icv ) {
	  cerr << icv << endl;
	  exit( -1 );
	}
	catch ( InvalidCellCoordinates icc ) {
	  cerr << icc << endl;
	  exit( -1 );
	}
	break;
      case 'q' :
	exit( 0 );
	break;
      case 'r' :
	try {
	  this->engine->load_level(  );
	}
	catch ( InvalidLevelFile ilf ) {
	  cerr << ilf << endl;
	  exit( -1 );
	}
	catch ( InvalidCellValue icv ) {
	  cerr << icv << endl;
	  exit( -1 );
	}
	catch ( InvalidCellCoordinates icc ) {
	  cerr << icc << endl;
	  exit( -1 );
	}
	break;
      default :
	break;
      } // keys sorting switch end
    // the callback is used to update Fltk2_UI widgets that need it
    this->do_callback(  );
    
    if ( this->engine->is_it_over(  ) == true )
      this->level_over(  );

    return 1;
  
  default:
    return 1;
  }
}
