/*
 *  Exceptions.hh
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

#ifndef __EXCEPTIONS_HH__
#  define __EXCEPTIONS_HH__

#include <iostream>
#include <string>

using namespace std;

namespace ngstar2 {
  class CannotFindLevelsets {
  private:
    string *levelset;
    
  public:
    CannotFindLevelsets( string *levelset )
    {
      this->levelset = new string( levelset->c_str(  ) );
    }

    friend ostream& operator<<(ostream& os, CannotFindLevelsets cfl )
    {
      os << "ERROR : Cannot find where levelset " << (cfl.levelset)->c_str(  ) << " !";
      return( os );
    }
  };

#if defined (_USE_FLTK2_) || defined (_BUILD_EDITOR_)
  class CannotFindThemes {
  private:
    string *theme;

  public:
    CannotFindThemes( string *theme )
    {
      this->theme = new string( theme->c_str(  ) );
    }

    friend ostream& operator<<(ostream& os, CannotFindThemes cft )
    {
      os << "ERROR : Cannot find where theme" << cft.theme->c_str(  ) << " !";
      return( os );
    }
  };
#endif

  class LoadingLevelException {
    friend ostream& operator<<(ostream& os, LoadingLevelException lle )
    {
      os << "Error Loading level";
      return( os );
    }
  };
  
  class InvalidLevelFile : public LoadingLevelException {
  private:
    string *levelfile;

  public:
    // InvalidLevelFile(  )
    // {
    // this->levelfile = new string( "Unspecified level" );
    // }

    InvalidLevelFile( const string *levelfile )
    {
      this->levelfile = new string( levelfile->c_str(  ) );
    }

    friend ostream& operator<<(ostream& os, InvalidLevelFile ilf )
    {
      os << "ERROR : Invalid level file " << ilf.levelfile->c_str(  ) << " !";
      return( os );
    }
  };

  class InvalidCellValue : public LoadingLevelException {
  private:
    char cell;

  public:
    InvalidCellValue( char cell )
    {
      this->cell = cell;
    }

    friend ostream& operator<<(ostream& os, InvalidCellValue icv )
    {
      os << "ERROR : Invalid cell " << icv.cell << " in level file !";
      return( os );
    }
  };

  class InvalidCellCoordinates : public LoadingLevelException {
  private:
    int x;
    int y;

  public:
    InvalidCellCoordinates( int x, int y )
    {
      this->x = x;
      this->y = y;
    }

    friend ostream& operator<<(ostream& os, InvalidCellCoordinates icc )
    {
      os << "ERROR : Trying to set a cell outside of board !" << endl;
      os << "Invalid coordinates are : ( " << icc.x << " ; " << icc.y << " )";
      return( os );
    }
  };
}

#endif /* __EXCEPTIONS_HH__ */
