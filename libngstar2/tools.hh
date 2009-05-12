/*
 *  tools.hh
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

#ifndef __TOOLS_H__
#  define __TOOLS_H__

#include <sstream>
#include <string>

#include <cerrno>
#include <cstdio>
#include <cstring>

#include <dirent.h>

using namespace std;

namespace ngstar2 {

  // defines
  // level dimensions
  #define NGSTAR_LEVEL_HEIGHT 9
  #define NGSTAR_LEVEL_WIDTH  16

  // char values of cells
  #define BALL 'B'
  #define CUBE 'C'
  #define EMPTY ' '
  #define GIFT 'G'
  #define WALL 'W'

  // enums
  typedef enum {empty, wall, gift, ball, cube} cell;
  typedef enum {go_up, go_down, go_left, go_right} direction;

  // utilities functions
  int  count_files_in_dir ( const char* );
  bool check_dir_existence( const char* );
  // next function thanks to C & C++ Comeau FAQ
  string itos( int arg );

} // end namespace ngstar2

#endif /* __TOOLS_H__ */
