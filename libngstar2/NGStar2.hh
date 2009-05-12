/*
 *  NGStar2.hh
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

#ifndef __NGSTAR_H__
#define __NGSTAR_H__

// includes
#include <iostream>
#include <fstream>
#include <string>

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include "Exceptions.hh"
#include "Configuration.hh"
#include "tools.hh"

using namespace std;

namespace ngstar2 {

  // classe(s)
  class NGStar2 {
  private:
    time_t t1;

    inline void read_ngs_level( const string * );
    inline void read_gsl_level( const string * );
    void        read_level    ( const string * );

    void move_ball     ( direction );
    void move_cube     ( direction );

  public:
    // variables
    Configuration *config;

    cell* board;
    cell  moving;

    int   nb_gifts;
    int   nb_moves;

    unsigned int level_h;
    unsigned int level_w;

    unsigned int ball_X;
    unsigned int ball_Y;

    unsigned int cube_X;
    unsigned int cube_Y;

    // Constructor
    NGStar2( Configuration * );

    // Methods
    void set_cell      ( int, int, char );
    void load_level    ( void );
    void load_level    ( const char * );
    void next_level    ( void );
    void previous_level( void );
    void switch_moving ( void );
    void move          ( direction );
    bool is_it_over    ( void );
    int  get_duration  ( void );

    friend ostream& operator<<(ostream& os, NGStar2 *ngstar2 )
    {
      int i;
      int j;
      cell current_cell;

      for ( i=NGSTAR_LEVEL_HEIGHT ; i-- ; ) {
                for ( j=NGSTAR_LEVEL_WIDTH ; j-- ; ) {
                    current_cell = ngstar2->board[i*NGSTAR_LEVEL_WIDTH+j];

                    switch ( current_cell ) {
                    case wall :
                        os << "W";
                        break;
                    case empty :
                        os << " ";
                        break;
                    case gift :
                        os << "G";
                        break;
                    case ball :
                        os << "B";
                        break;
                    case cube :
                        os << "C";
                        break;
                    default :
                        break;
                    }
                }
                os << endl;
      }
      os << ngstar2->nb_gifts << " gifts ; ";
      os << "ball(" << ngstar2->ball_X << "," <<  ngstar2->ball_Y << ") ; ";
      os << "cube(" << ngstar2->cube_X << "," << ngstar2->cube_Y << ")" << endl ;
      if ( ngstar2->is_it_over() == true )
                os << "it's over" << endl;
      else
                os << "it ain't over" << endl;

      return( os );
    }

  };

} // end namespace ngstar2

#endif /* __NGSTAR_H__ */
