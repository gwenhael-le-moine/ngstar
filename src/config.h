/*
 *  config.h : Global wide configuration
 *
 *  Copyngstar_right (C) 2003 Gwenhael LE MOINE
 *
 *   This file is part of NGStar
 *
 *   NGStar is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   NGStar is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with NGStar; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __NGSTAR_CONFIG__
#  define __NGSTAR_CONFIG__

#ifndef APPLICATION_VERSION
#  define APPLICATION_VERSION "FLTK's NGStar X.x.x"
#endif /* APPLICATION_VERSION */

#ifndef THEMES_PATH
#  define THEMES_PATH "./themes"
#endif /* THEMES_PATH */

#define WIDGETS_HEIGHT 15
#define NGSTAR_BOARD_HEIGHT 180

#define W_MAIN_WIDTH 320
#define W_MAIN_HEIGHT ( ( WIDGETS_HEIGHT * 2 ) + NGSTAR_BOARD_HEIGHT )

#define ATTACH_CALLBACK( widget, method ) \
   widget->callback( reinterpret_cast< void(*)(Widget*,void*) >( method ), this )

#endif /*__NGSTAR_CONFIG__*/
