/*
 *  NGStar2.cc
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

/* includes */
#include "NGStar2.hh"
#include "tools.hh"

using namespace std;

namespace ngstar2 {

  // NGSTAR2 class implementation
  // in order of declaration
  // private:

  //
  // Set the cell(x, y) with the given value (if valid)
  //
  void NGStar2::set_cell( int x, int y , char value )
  {
    if ( ( x <= NGSTAR_LEVEL_WIDTH ) && ( y <= NGSTAR_LEVEL_HEIGHT ) ) {
      switch ( value ) {
      case WALL : // char if for ngs levels
      case 3 :   // int is for gsl levels
                this->board[y*NGSTAR_LEVEL_WIDTH+x] = wall;
                break;
      case EMPTY :
      case 2 :
                this->board[y*NGSTAR_LEVEL_WIDTH+x] = empty;
                break;
      case GIFT :
      case 4 :
                this->board[y*NGSTAR_LEVEL_WIDTH+x] = gift;
                this->nb_gifts++;
                break;
      case BALL :
      case 0 :
                this->board[y*NGSTAR_LEVEL_WIDTH+x] = ball;
                this->ball_X = x;
                this->ball_Y = y;
                break;
      case CUBE :
      case 1 :
                this->board[y*NGSTAR_LEVEL_WIDTH+x] = cube;
                this->cube_X = x;
                this->cube_Y = y;
                break;
      case '\n' :
                break;
      default :
                throw InvalidCellValue( value );
                break;
      }
    } else {
      throw InvalidCellCoordinates( x, y );
    }
  }


  //
  // Load a level from file filename in NGStar format
  //
  inline void NGStar2::read_ngs_level( const string *filename )
  {
    int i;
    int j;
    char current_cell;
    bool ball_set=false;
    bool cube_set=false;

    ifstream file;

    file.open( filename->c_str(  ), ios::in );
    if ( file == NULL ) {
      throw InvalidLevelFile( filename );
    }

    this->level_h = NGSTAR_LEVEL_HEIGHT;
    this->level_w = NGSTAR_LEVEL_WIDTH;
    this->board = new cell[ this->level_w * this->level_h ];

    for ( i=NGSTAR_LEVEL_HEIGHT ; i-- ; ) {
      for ( j=NGSTAR_LEVEL_WIDTH ; j-- ; ) {
                file.read( &current_cell, 1 );
                if ( file.eof(  ) )
                    throw InvalidLevelFile( filename );
                this->set_cell( j, i, current_cell );
                if ( current_cell == BALL )
                    ball_set = true;
                else
                    if ( current_cell == CUBE )
                        cube_set = true;
      }
    }

    file.close(  );

    if ( ball_set == false || cube_set == false ) {
      throw InvalidLevelFile( filename );
    }
  }

  //
  // Load a level from file filename in G-star format
  // (from g-star source, translated into C++ and adapted
  // but basically the structure is the same)
  //
  inline void NGStar2::read_gsl_level( const string *filename )
  {
    int i;
    int j;
    char current_cell;
    char width;
    char height;
    bool ball_set=false;
    bool cube_set=false;

    ifstream file;

    file.open( filename->c_str(  ), ios::binary );
    if ( file == NULL ) {
      throw InvalidLevelFile( filename );
    }

    /* skeeping header "gsl1" */
    file.seekg( 4 );

    file.read( &width, 1 );
    file.read( &height, 1 );

    this->level_h = (int)height; //NGSTAR_LEVEL_HEIGHT;
    this->level_w = (int)width; //NGSTAR_LEVEL_WIDTH;
    this->board = new cell[ this->level_w * this->level_h ];

    if (( width == NGSTAR_LEVEL_WIDTH ) && ( height == NGSTAR_LEVEL_HEIGHT )) {
      for ( i=NGSTAR_LEVEL_HEIGHT ; i-- ; ) {
                for ( j=NGSTAR_LEVEL_WIDTH ; j-- ; ) {
                    file.read( &current_cell, 1 );
                    if ( file.eof(  ) )
                        throw InvalidLevelFile( filename );
                    this->set_cell( j, i, current_cell );
                    if ( current_cell == 0 )
                        ball_set = true;
                    else
                        if ( current_cell == 1 )
                            cube_set = true;
                }
      }
    }

    file.close(  );

    if ( ball_set == false || cube_set == false ) {
      throw InvalidLevelFile( filename );
    }
  }

  //
  // Determine the format of level in filename and call
  // the corresponding method
  //
  void NGStar2::read_level( const string *filename )
  {
    ifstream file;
    char *foo = new char[ 5 ];

    this->nb_gifts = 0;

    file.open( filename->c_str(  ), ios::binary );
    if ( file == NULL ) {
      throw InvalidLevelFile( filename );
    }

    /* verifying file type (gsl or ngs) */
    file.read( foo, 4 );
    foo[4] = '\0';
    if ( (new string( foo ))->compare( "gsl1" ) == 0 ) {
      file.close(  );
      read_gsl_level( filename );
    }
    else {
      file.close(  );
      read_ngs_level( filename );
    }

    this->nb_moves = 0;
    this->moving = ball;
  }

  //
  // Move the ball in given direction
  // stopped by walls and the cube
  //
  void NGStar2::move_ball( direction direction )
  {
    int dh=0;
    int dv=0;
    unsigned int tmp_ball_X = this->ball_X;
    unsigned int tmp_ball_Y = this->ball_Y;

    /* determining the direction it will go */
    switch ( direction ) {
    case go_up    : dv=1; break;
    case go_down  : dv=-1; break;
    case go_left  : dh=1; break;
    case go_right : dh=-1; break;
    default : break;
    }

    /* determining the cell it will go */
    while (
                     /* checking board limits */
                     (( tmp_ball_Y + dv >= 0 ) &&
                        ( tmp_ball_Y + dv < NGSTAR_LEVEL_HEIGHT )) &&
                     (( tmp_ball_X + dh >= 0 ) &&
                        ( tmp_ball_X + dh < NGSTAR_LEVEL_WIDTH )) &&
                     /* is next cell a wall ? */
                     (this->board[(tmp_ball_Y + dv)*NGSTAR_LEVEL_WIDTH+tmp_ball_X + dh] != wall) &&
                     (this->board[(tmp_ball_Y + dv)*NGSTAR_LEVEL_WIDTH+tmp_ball_X + dh] != cube)
                     )
      {
                tmp_ball_Y += dv;
                tmp_ball_X += dh;

                /* did it collect a gift ? */
                if ( this->board[tmp_ball_Y*NGSTAR_LEVEL_WIDTH+tmp_ball_X] == gift )
                    {
                        this->board[tmp_ball_Y*NGSTAR_LEVEL_WIDTH+tmp_ball_X] = empty;
                        this->nb_gifts--;
                    }
      }

    if ( ( tmp_ball_X != this->ball_X ) || ( tmp_ball_Y != this->ball_Y ) ) {
      // if it's the first move then set t1 as the start of level
      if ( this->nb_moves == 0 )
                (void) time( &( this->t1 ) );
      /* empty previous cell */
      this->board[this->ball_Y*NGSTAR_LEVEL_WIDTH+this->ball_X] = empty;

      /* changing coordinates */
      this->ball_Y = tmp_ball_Y;
      this->ball_X = tmp_ball_X;

      /* set destination cell */
      this->board[tmp_ball_Y*NGSTAR_LEVEL_WIDTH+tmp_ball_X] = ball;

      ++(this->nb_moves);
    }
  }

  //
  // Move the cube in given direction
  // stopped by walls, gifts and the ball
  // slightly different from move_ball
  //
  void NGStar2::move_cube( direction direction )
  {
    int dh=0;
    int dv=0;
    unsigned int tmp_cube_X = this->cube_X;
    unsigned int tmp_cube_Y = this->cube_Y;

    /* determining the direction it will go */
    switch ( direction ) {
    case go_up    : dv=1; break;
    case go_down  : dv=-1; break;
    case go_left  : dh=1; break;
    case go_right : dh=-1; break;
    default : break;
    }

    /* determining the cell it will go */
    while (
                     /* checking board limits */
                     (( tmp_cube_Y + dv >= 0 ) &&
                        ( tmp_cube_Y + dv < NGSTAR_LEVEL_HEIGHT )) &&
                     (( tmp_cube_X + dh >= 0 ) &&
                        ( tmp_cube_X + dh < NGSTAR_LEVEL_WIDTH )) &&
                     /* is next cell a wall ? */
                     (this->board[(tmp_cube_Y + dv)*NGSTAR_LEVEL_WIDTH+tmp_cube_X + dh] == empty)
                     )
      {
                tmp_cube_Y += dv;
                tmp_cube_X += dh;
      }

    if ( ( tmp_cube_X != this->cube_X ) || ( tmp_cube_Y != this->cube_Y ) ) {
      // if it's the first move then set t1 as the start of level
      if ( this->nb_moves == 0 )
                (void) time( &( this->t1 ) );
      /* empty previous cell */
      this->board[this->cube_Y*NGSTAR_LEVEL_WIDTH+this->cube_X] = empty;

      /* changing coordinates */
      this->cube_Y = tmp_cube_Y;
      this->cube_X = tmp_cube_X;

      /* set destination cell */
      this->board[tmp_cube_Y*NGSTAR_LEVEL_WIDTH+tmp_cube_X] = cube;

      ++(this->nb_moves);
    }
  }

  // public:

  //
  // Constructor, initialize things
  //
  NGStar2::NGStar2( Configuration *config )
  {
    this->config = config;
    this->load_level(  );
  }

  //
  // Load level nb_level from current levelset if level exists
  //
  void NGStar2::load_level(  )
  {
    string *level_filename;

    if ( ( this->config->levelset != NULL ) &&
                 ( this->config->current_level <= this->config->nb_levels ) ) {
      level_filename = new string( this->config->levelsets_path->c_str(  ) );
      level_filename->append( "/" );
      level_filename->append( this->config->levelset->c_str(  ) );
      level_filename->append( "/" );
      level_filename->append( itos( this->config->current_level ) );

      read_level( level_filename );
    }
  }

  //
  // Load level nb_level from current levelset if level exists
  //
  void NGStar2::load_level( const char *filename )
  {
    read_level( new string( filename ) );
  }

  void NGStar2::next_level    (  )
  {
    if( ( this->config->current_level + 1 ) <= this->config->nb_levels )
      ++(this->config->current_level);

    this->load_level(  );
  }

  void NGStar2::previous_level(  )
  {
    if( this->config->current_level > 1 )
      --(this->config->current_level);

    this->load_level(  );
  }

  void NGStar2::switch_moving(  )
  {
    ++( this->nb_moves );
    this->moving = ( this->moving == ball ) ? cube : ball;
  }

  void NGStar2::move( direction where )
  {
    if ( this->moving == ball )
      this->move_ball( where );
    else
      this->move_cube( where );
  }

  //
  // Check is there's gifts left and return the result
  //
  bool NGStar2::is_it_over( void )
  {
    return( ( this->nb_gifts > 0 ) ? false : true );
  }

  int NGStar2::get_duration(  )
  {
    time_t t2;
    (void) time( &( t2 ) );
    return( (int)t2 - ( this->t1 ) );
  }

} // end namespace ngstar2
