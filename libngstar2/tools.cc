/*
 *  tools.cc
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

#include "tools.hh"

using namespace std;

namespace ngstar2 {

  // Utilities functions implementation
  int count_files_in_dir( const char* dirname )
  {
    int counter = 0;
    DIR* dir;
    struct dirent* dir_content;

    /* opening dirname */
    dir = opendir( dirname );
    if ( !dir ) {
      perror("opendir");
      return( -1 );
    }
    else {
      /* reading dirname */
      while ( (dir_content = readdir( dir )) != NULL )
	if ( ( ( dir_content->d_name )[ 0 ] != '.' ) &&
	     ( ( dir_content->d_name )[ strlen( dir_content->d_name ) - 1 ] != '~' ) )
	  ++counter;


      /* closing dirname */
      if ( closedir( dir ) == -1 ) {
	perror( "closedir" );
	return -1;
      }
    
      return counter;
    }
  }

  bool check_dir_existence( const char* path )
  {
    bool result;
    DIR* dirtmp = opendir( path );

    result = ( dirtmp != NULL ) ? true : false;

    closedir( dirtmp );

    return( result );
  }
  
  string itos( int arg )
  {
    ostringstream buffer;
    buffer << arg; // send the int to the ostringstream
    return buffer.str(); // capture the string
  }
  
} // end namespace ngstar2
