/*
 *  main.cc
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

#include <string>

#if defined (_USE_FLTK2_) || defined (_BUILD_EDITOR_)
#  include <fltk/run.h>
#endif

#if defined (_USE_FLTK2_)
#  include "Fltk2_UI.hh"
#endif

#if defined (_BUILD_EDITOR_)
#  include "LevelMaker_UI.hh"
#endif

#if defined (_USE_NCURSES_)
#  include "Ncurses_UI.hh"
#endif

#include "../libngstar2/NGStar2.hh"
#include "../libngstar2/Configuration.hh"
#include "../libngstar2/Scores.hh"
#include "../libngstar2/Exceptions.hh"

using namespace std;
using namespace ngstar2;

int main( int argc, char* argv[] )
{
  string *bin_name;

  Configuration *config;
  NGStar2 *engine;
  Scores *scores;

  /****************************************/
  /*         Initialize NGStar            */
  /****************************************/
  // Reading $HOME/.ngstar/config & commandline parameters
  try {
    config = new Configuration( argc, argv );
    cerr << "Config loaded" << endl;
  }
  catch( CannotFindLevelsets cfl) {
    cerr << cfl << endl;
    exit( -1 );
  }

  // initializing NGStar2 engine
  try {
    engine = new NGStar2( config );
    cerr << "NGStar2 engine loaded" << endl;
  }
  catch( InvalidLevelFile ilf ) {
    cerr << ilf << endl;
    exit( -1 );
  }
  catch( InvalidCellValue icv ) {
    cerr << icv << endl;
    exit( -1 );
  }
  catch( InvalidCellCoordinates icc ) {
    cerr << icc << endl;
    exit( -1 );
  }

  scores = new Scores( engine );

  enum { ncurses, fltk2, editor };

  int what_to_run;

  bin_name = new string( basename( argv[0] ) );

#if defined (_USE_FLTK2_)
#  if defined (_USE_NCURSES_)
  if ( ( ( bin_name->find( "fltk" ) != string::npos ) ||
       ( bin_name->find( "x" ) != string::npos ) ||
       ( bin_name->find( "X" ) != string::npos ) )
#    if defined (_BUILD_EDITOR_)
       && ( bin_name->find( "editor" ) == string::npos )
#    endif
       )
#  endif
    what_to_run = fltk2;
#  if defined (_USE_NCURSES_)
  else
#  endif
#endif
#if defined (_BUILD_EDITOR_)
#  if defined (_USE_NCURSES_)
  if ( ( bin_name->find( "editor" ) != string::npos ) )
#  endif
    what_to_run = editor;
#  if defined (_USE_NCURSES_)
  else
#  endif
#endif
#if defined (_USE_NCURSES_)
    what_to_run = ncurses;
#endif

  switch ( what_to_run ){
  case fltk2 :
#if defined (_USE_FLTK2_)
    /****************************************/
    /*          fltk2  interface            */
    /****************************************/
    Fltk2_UI *fltk2_ui;

    fltk2_ui = new Fltk2_UI( engine, config, scores );
    cerr << "Fltk2_UI loaded" << endl;
    /*
     * Main loop
     */
    fltk2_ui->show(  );

    return( run(  ) );
#endif

#if defined (_USE_NCURSES_)
  case ncurses :
    /****************************************/
    /*         ncurses interface            */
    /****************************************/
    Ncurses_UI *ncurses_ui;

    ncurses_ui = new Ncurses_UI( engine, config, scores );
    cerr << "Ncurses_UI loaded" << endl;
    /*
     * Main loop
     */
    ncurses_ui->run(  );

    return( 0 );
#endif

#if defined (_BUILD_EDITOR_)
  case editor :
    /****************************************/
    /*          Editor interface            */
    /****************************************/
    LevelMaker_UI *levelmaker_ui;

    levelmaker_ui = new LevelMaker_UI( engine, config );
    cerr << "LevelMaker_UI loaded" << endl;
    /*
     * Main loop
     */
    levelmaker_ui->show(  );

    return( fltk::run(  ) );
#endif
  default :
    cerr << "Well... Err... You must at least compile one interface... !" << endl;
  }

  return( 0 );
}
