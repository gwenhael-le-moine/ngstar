/*
 *  Scores.cc
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

#include "Scores.hh"

namespace ngstar2{

  // Constructor(s)
  Scores::Scores( NGStar2 *engine )
  {
    this->engine = engine;
    
    this->init(  );
  }
  
  // Methods
  void Scores::init(  )
  {
    int i;

    this->scores = new int[ this->engine->config->nb_levels ];
    
    for ( i = 0 ; i < this->engine->config->nb_levels ; ++i )
      (this->scores)[ i ] = -1;

    // building this->scorefilename
    string *DOTngstar_path;
    
    DOTngstar_path = new string( getenv( "HOME" ) );
    DOTngstar_path->append( "/.ngstar/" );
    if ( check_dir_existence( DOTngstar_path->c_str(  ) ) == false )
      DOTngstar_path = new string( "/tmp/" );
    DOTngstar_path->append( this->engine->config->levelset->c_str(  ) );
    DOTngstar_path->append( ".scores" );

    this->scorefilename = new string( DOTngstar_path->c_str(  ) );

    this->read(  );
  }

  void Scores::read(  )
  {
    int i;
    ifstream file;
    
    file.open( this->scorefilename->c_str(  ), ios::in );
    
    if ( file != NULL )
      for ( i=0 ; i < this->engine->config->nb_levels ; ++i )
	file >> this->scores[i];

    file.close(  );
    file.clear(  );
  }

  void Scores::write(  )
  {
    ofstream file;
    
    file.open( this->scorefilename->c_str(  ), ios::out );
    
    file << this;
    
    file.close(  );
    file.clear(  );
  }
  
  int Scores::get(  )
  {
    return( this->get( this->engine->config->current_level - 1 ) );
  }
  
  int Scores::get( int level )
  {
    return( (this->scores)[ level ] );
  }
  
  void Scores::set(  )
  {
    this->set( this->engine->config->current_level,
	       this->engine->nb_moves,
	       this->engine->get_duration(  ) );
  }
  
  void Scores::set( int level, int nbMoves, int nbSeconds )
  {
    //    int score = abs( nbMoves - nbSeconds );
    int score = nbMoves * nbSeconds;
    if ( ( score <= (this->scores)[ level - 1 ] ) ||
				 ( (this->scores)[ level - 1 ] == -1) ) {
      (this->scores)[ level - 1 ] = (int) score;
      this->write(  );
    }
  }
  
} // end namespace ngstar2
