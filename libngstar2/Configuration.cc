/*
 *  Configuration.cc
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

#include "Configuration.hh"

namespace ngstar2 {

  Configuration::Configuration( int argc, char* argv[] )
  {
    ifstream file;

    this->levelsets_path = new string( LEVELSETS_PATH );
    this->levelset = new string( "g-star" );
#if defined (_USE_FLTK2_) || defined (_BUILD_EDITOR_)
    this->themes_path = new string( THEMES_PATH );
    this->theme = new string( "cyco002" );
#endif
    this->fileName = new string( getenv( "HOME" ) );
    this->fileName->append( "/.ngstar/config" );
    this->set_level( 1 );
#ifdef _USE_NCURSES_
    this->black_and_white = 0;
#endif

    cerr << "Default values setted" << endl;

    // test if ~/.ngstar/config exists
    file.open( this->fileName->c_str(), ios::in );
    if ( file != NULL ) { // if yes, read it
      file.close(  );
      file.clear(  );
      this->read(  );
      cout << "configuration file LOADED" << endl;
    }

    this->parse_args( argc, argv );

    this->set_levelset( this->levelset );
#if defined (_USE_FLTK2_) || defined (_BUILD_EDITOR_)
    this->set_theme( this->theme );
#endif
  }

  void Configuration::read()
  {
    ifstream file;
    char *a, *egal, *b;

    file.open( this->fileName->c_str(  ), ios::in );

    do {
      a = (char*)malloc( strlen( "LevelSetsPath" ) + 1 );
      egal = (char*)malloc( 2 );
      b = (char*)malloc( 1024 );

      file >> a >> egal >> b;

      if ( ( new string( "LevelSetsPath" ) )->compare( a ) == 0 ) {
	this->levelsets_path = new string( b );
      } else
	
	if ( ( new string( "LevelSet" ) )->compare( a ) == 0 ) {
	  this->levelset = new string( b );
	} else

	  if ( ( new string( "CurrentLevel" ) )->compare( a ) == 0 ) {
	    this->set_level( atoi( b ) );
	  }

#if defined (_USE_FLTK2_) || defined (_BUILD_EDITOR_)
	  else
	    if ( ( new string( "ThemesPath" ) )->compare( a ) == 0 ) {
	      this->themes_path = new string( b );
	    } else
	      
	      if ( ( new string( "Theme" ) )->compare( a ) == 0 ) {
		this->theme = new string( b );
	      }
#endif
#ifdef _USE_NCURSES_
	      else
		
		if ( ( new string( "black_and_white" ) )->compare( a ) == 0 ) {
		  this->black_and_white = atoi( b );
		}
#endif
      free( a );
      free( egal );
      free( b );
    } while( !file.eof(  ) );

    file.close(  );
    file.clear(  );
  }

  void Configuration::write()
  {
    ofstream file;
    file.open( this->fileName->c_str(  ), ios::out );

    file << this << endl;

    file.close(  );
    file.clear(  );
  }

  void Configuration::create_default()
  {
    string *strtmp = new string( getenv( "HOME" ) );
    strtmp->append( "/.ngstar" );

    // test if directory ~/.ngstar exists  
    if ( check_dir_existence( strtmp->c_str(  ) ) == false ) {
      string* strtmp2;
      mkdir( strtmp->c_str(  ), S_IRWXO | S_IRWXG | S_IRWXU );

#if defined (_USE_FLTK2_) || defined (_BUILD_EDITOR_)
      strtmp2 = new string( strtmp->c_str(  ) );
      strtmp2->append( "/themes" );
      mkdir( strtmp2->c_str(  ), S_IRWXO | S_IRWXG | S_IRWXU );
#endif
      strtmp2 = new string( strtmp->c_str(  ) );
      strtmp2->append( "/levelsets" );
      mkdir( strtmp2->c_str(  ), S_IRWXO | S_IRWXG | S_IRWXU );
    }

    this->write(  );
  }

  int Configuration::parse_args( int argc, char* argv[] )
  {
    int option_index;
    char c = '?';
  
    char* optstring = "l:t:w:bchv";
    static struct option long_options[] = {
      {"levelset",      required_argument,      NULL, 'l'},
#if defined (_USE_FLTK2_) || defined (_BUILD_EDITOR_)
      {"theme",         required_argument,      NULL, 't'},
#endif
#ifdef _USE_NCURSES_
      {"black-and-white", no_argument,      NULL, 'b'},
#endif
      {"wrap",          required_argument,      NULL, 'w'},
      {"create-default-config", no_argument,    NULL, 'c'},
      {"help",          no_argument,            NULL, 'h'},
      {"version",       no_argument,            NULL, 'v'},
      {0, 0, 0, 0}
    };
  
    char* help_text = "ngstar [options]\n"
      "\t-v --version :\n\t\t show version\n"
      "\t-h --help :\n\t\t what you are reading\n"
      "\t-c --create-default-config :\n\t\t create $HOME/.ngstar/* with default values\n"
      "\t-l<levelset> --levelset=<levelset> :\n\t\t use a different levelset (default: \"default\")\n"
#if defined (_USE_FLTK2_) || defined (_BUILD_EDITOR_)
      "\t-t<theme> --theme=<theme> :\n\t\t use a different theme (default: \"default\")\n"
#endif
#ifdef _USE_NCURSES_
      "\t-b --black-and-white :\n\t\t don't use colors (ncurses)\n"
#endif
      "\t-w<n> --wrap=<n> :\n\t\t wrap to level n\n";

    while(c != EOF) {
      c = getopt_long(argc, argv, optstring, long_options, &option_index);
    
      switch(c) {
      case 'c' :
	this->create_default(  );
	cout << "Default $HOME/.ngstar created" << endl;
	exit( 0 );
	break;
      case 'v' :
	cout << APPLICATION_VERSION << endl;
	exit(0);
	break;
      case 'h' :
	cout << help_text << endl;
	exit(0);
	break;
      case 'l' :
	this->levelset = new string( optarg );
	break;
#if defined (_USE_FLTK2_) || defined (_BUILD_EDITOR_)
      case 't' :
	this->theme = new string( optarg );
	break;
#endif
#ifdef _USE_NCURSES_
      case 'b' :
	this->black_and_white = 1;
	break;
#endif
      case 'w' :
	this->set_level( atoi( optarg ) );
	break;
      case '?' :
      case ':' :
	exit(0);
	break;
      default : break;
      }
    }

    if (optind < argc) {
      cout << "Invalid arguments : " << endl;
      while (optind < argc)
	cout << argv[optind++] << endl;
      cout << endl;
    }
  
    return(optind);
  }

  //
  // If given levelset_name exist sets it as current levelset
  //
  void Configuration::set_levelset( const string *levelset_name )
  {
    string *strtmp;

    this->levelset = new string( levelset_name->c_str(  ) );

    cerr << "levelset setted in config" << endl;
    strtmp = new string( this->levelsets_path->c_str(  ) );
    strtmp->append( "/" );
    strtmp->append( this->levelset->c_str(  ) );
    if ( check_dir_existence( strtmp->c_str(  ) ) == false ) {
      strtmp = new string( "./datas/levelsets/" );
      strtmp->append( this->levelset->c_str(  ) );
      if ( check_dir_existence( strtmp->c_str(  ) ) == false ) {
	strtmp =  new string( getenv( "HOME" ) );
	strtmp->append( "/.ngstar/levelsets/" );
	strtmp->append( this->levelset->c_str(  ) );
	if ( check_dir_existence( strtmp->c_str(  ) ) == false ) {
	  throw CannotFindLevelsets( this->levelset );
	}
	else {
	  this->levelsets_path = new string( getenv( "HOME" ) );
	  this->levelsets_path->append( "/.ngstar/levelsets/" );
	}
      }
      else {
	this->levelsets_path = new string( "./datas/levelsets/" );
      }
    }
    
    this->nb_levels = count_files_in_dir( strtmp->c_str(  ) );
    if ( this->current_level > this->nb_levels )
      this->current_level = 1;
    
    cerr << "Levelset " << strtmp->c_str(  ) << " LOADED : " << this->nb_levels << " levels" << endl;
  }
  
  void Configuration::set_level( int lvl )
  {
    this->current_level = lvl;
  }

#if defined (_USE_FLTK2_) || defined (_BUILD_EDITOR_)
  void Configuration::set_theme( const string *theme_name )
  {
    string *strtmp;
    
    this->theme = new string( theme_name->c_str(  ) );

    cerr << "theme setted in config" << endl;
    strtmp = new string( this->themes_path->c_str(  ) );
    strtmp->append( "/" );
    strtmp->append( this->theme->c_str(  ) );
    if ( check_dir_existence( strtmp->c_str(  ) ) == false ) {
      strtmp = new string( "./datas/themes/" );
      strtmp->append( this->theme->c_str(  ) );
      if ( check_dir_existence( strtmp->c_str(  ) ) == false ) {
	strtmp =  new string( getenv( "HOME" ) );
	strtmp->append( "/.ngstar/themes/" );
	strtmp->append( this->theme->c_str(  ) );
	if ( check_dir_existence( strtmp->c_str(  ) ) == false ) {
	  throw CannotFindThemes( this->theme );
	}
      }
      else {
	this->themes_path = new string( "./datas/themes/" );
      }

    }
  }
#endif

} // end namespace ngstar2
