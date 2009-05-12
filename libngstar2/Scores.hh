/*
 *  Scores.hh
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

#ifndef __SCORES_H__
#define __SCORES_H__

#include <iostream>
#include <string>

#include <cstdlib>

#include "NGStar2.hh"

using namespace std;

namespace ngstar2 {
  
  class Scores {
  private :
    NGStar2 *engine;

    int *scores;

    string *scorefilename;
    
  public:
    // Constructor(s)
    Scores( NGStar2 * );
    
    // Methods
    void init(  );
    void read(  );
    void write(  );
    int get(  );
    int get( int );
    void set(  );
    void set( int, int, int );

    friend ostream& operator<<(ostream& os, Scores* scrs )
    {
      int i;

      for ( i = 0 ; i < scrs->engine->config->nb_levels ; ++i )
	os << (scrs->scores)[ i ] << endl;

      return( os );
    }
  };
  
} // end namespace ngstar2

#endif /* __SCORES_H__ */
