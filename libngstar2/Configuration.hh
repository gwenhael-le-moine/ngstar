/*
 *  Configuration.hh
 *
 *  Copyright (C) 2004, 2005, 2006 Gwenhael LE MOINE
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

#ifndef __CONFIGURATION_HH__
#  define __CONFIGURATION_HH__

#include <fstream>
#include <iostream>
#include <string>

#include <cstdlib>

#include <getopt.h>
#include <sys/stat.h>

#include "Exceptions.hh"
#include "tools.hh"

using namespace std;

namespace ngstar2 {

  class Configuration {
  private:
    string* fileName;

  public:
    // Levels
    int     current_level;
    int     nb_levels;
    // Levelsets
    string* levelsets_path;
    string* levelset;
#if defined (_USE_FLTK2_) || defined (_BUILD_EDITOR_)
    // Themes
    string* themes_path;
    string* theme;
#endif
#ifdef _USE_NCURSES_
    int     black_and_white;
#endif

    Configuration( int argc, char* argv[] );

    void read(  );
    void write(  );
    void create_default(  );
    int  parse_args( int argc, char* argv[] );

    void set_levelset( const string * );
    void set_level( int );
#if defined (_USE_FLTK2_) || defined (_BUILD_EDITOR_)
    void set_theme( const string * );
#endif

    friend ostream& operator<<(ostream& os, Configuration* config )
    {
      os << "levelsets_path = " << config->levelsets_path->c_str(  ) << endl;
      os << "levelset       = " << config->levelset->c_str(  )       << endl;
#if defined (_USE_FLTK2_) || defined (_BUILD_EDITOR_)
      os << "themes_path    = " << config->themes_path->c_str(  )    << endl;
      os << "theme          = " << config->theme->c_str(  )          << endl;
#endif
      os << "current_level  = " << config->current_level             << endl;
#ifdef _USE_NCURSES_
      os << "black_and_white  = " << config->black_and_white         << endl;
#endif

      return( os );
    }
  };

} // end namespace ngstar2

#endif /* __CONFIGURATION_HH__*/
