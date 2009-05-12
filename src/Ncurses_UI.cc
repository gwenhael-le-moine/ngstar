/*
 *  Ncurses_UI.cc
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

/* includes */
#include "Ncurses_UI.hh"

using namespace ngstar2;

Ncurses_UI::Ncurses_UI( NGStar2 *engine, Configuration* config, Scores *scores )
{
  /* global variables initialization */
  this->config = config;
  this->engine = engine;
  this->scores = scores;

  /* ncurses initialization */
  this->w_main = initscr(  );
  noecho(  );
  cbreak(  );
  nonl(  );
  intrflush( this->w_main, FALSE );
  keypad( this->w_main, TRUE );

  if ( ( has_colors(  ) == TRUE ) && ( this->config->black_and_white == 0 ) ) {
    start_color(  );
    init_pair( color_CUBE,          COLOR_RED,    COLOR_BLACK  );
    init_pair( color_BALL,          COLOR_BLUE,   COLOR_BLACK  );
    init_pair( color_GIFT,          COLOR_YELLOW, COLOR_BLACK  );
    init_pair( color_WALL,          COLOR_WHITE,  COLOR_WHITE  );
    init_pair( color_EMPTY,         COLOR_BLACK,  COLOR_BLACK  );
    init_pair( color_TEXT,          COLOR_YELLOW, COLOR_BLACK  );
    init_pair( color_CUBE_SELECTED, COLOR_RED,    COLOR_YELLOW );
    init_pair( color_BALL_SELECTED, COLOR_BLUE,   COLOR_YELLOW );
    init_pair( color_BORDER,        COLOR_WHITE,  COLOR_BLACK  );
  }

  try {
    this->engine->load_level(  );
  }
  catch ( InvalidLevelFile ilf ) {
    cerr << ilf << endl;
    this->~Ncurses_UI(  );
    exit( -1 );
  }
  catch ( InvalidCellValue icv ) {
    cerr << icv << endl;
    this->~Ncurses_UI(  );
    exit( -1 );
  }
  catch ( InvalidCellCoordinates icc ) {
    cerr << icc << endl;
    this->~Ncurses_UI(  );
    exit( -1 );
  }
}

Ncurses_UI::~Ncurses_UI(  )
{
  echo(  );
  nocbreak(  );
  endwin(  );
}

void Ncurses_UI::display_board( void )
{
  int i, j, x, y;
  cell current_cell;

  /* border */
  attron( COLOR_PAIR( color_BORDER ));
  attron( A_BOLD );
  mvhline( 0, 2, 0, 32 );
  mvhline( 10, 2, 0, 32 );
  mvvline( 1, 1, 0, 9 );
  mvvline( 1, 34, 0, 9 );
  mvprintw( 0, 1, "%c", '+' );//ACS_ULCORNER );
  mvprintw( 0, 34, "%c", '+' );//ACS_URCORNER );
  mvprintw( 10, 1, "%c", '+' );//ACS_LLCORNER );
  mvprintw( 10, 34, "%c", '+' );//ACS_LRCORNER );
  move( 0, 0 );
  attroff( A_BOLD );
  attroff( COLOR_PAIR( color_BORDER ));
  
  /* board */
  for ( i=NGSTAR_LEVEL_HEIGHT ; i-- ; ) {
    for ( j=NGSTAR_LEVEL_WIDTH ; j-- ; ) {
      current_cell = this->engine->board[i*NGSTAR_LEVEL_WIDTH+j];
      x = 2+(NGSTAR_LEVEL_WIDTH-j-1)*2;
      y = NGSTAR_LEVEL_HEIGHT-i;

      switch ( current_cell ) {
	case wall :
	  attron( COLOR_PAIR( color_WALL ));
	  mvwprintw( w_main, y, x, SYMBOL_WALL );
	  attroff( COLOR_PAIR( color_WALL ));
	  break;
        case empty :
	  attron( COLOR_PAIR( color_EMPTY ));
	  mvwprintw( w_main, y, x, SYMBOL_EMPTY );
	  attroff( COLOR_PAIR( color_EMPTY ));
	  break;
	case gift :
	  attron( COLOR_PAIR( color_GIFT ));
	  attron( A_BOLD );
	  mvwprintw( w_main, y, x, SYMBOL_GIFT );
	  attroff( A_BOLD );
	  attroff( COLOR_PAIR( color_GIFT ));
	  break;
	case ball :
	  if ( this->engine->moving == ball ) {
	    attron( COLOR_PAIR( color_BALL_SELECTED ));
	    attron( A_BOLD );
	  }
	  else
	    attron( COLOR_PAIR( color_BALL ));
	  mvwprintw( w_main, y, x, SYMBOL_BALL );
	  if ( this->engine->moving == ball ) {
	    attroff( A_BOLD );
	    attroff( COLOR_PAIR( color_BALL_SELECTED ));
	  }
	  else
	    attroff( COLOR_PAIR( color_BALL ));
	  break;
	case cube :
	  if ( this->engine->moving == cube ) {
	    attron( COLOR_PAIR( color_CUBE_SELECTED ));
	    attron( A_BOLD );
	  }
	  else
	    attron( COLOR_PAIR( color_CUBE ));
	  mvwprintw( w_main, y, x, SYMBOL_CUBE );
	  if ( this->engine->moving == cube ) {
	    attroff( A_BOLD );
	    attroff( COLOR_PAIR( color_CUBE_SELECTED ));
	  }
	  else
	    attroff( COLOR_PAIR( color_CUBE ));
	  break;
        default :
	  break;
      }
    }
  }

//  touchwin( this->w_board );
  wrefresh( this->w_main );
}

void Ncurses_UI::display_info_panel( void )
{
  attron( COLOR_PAIR( color_TEXT ));
  attron( A_BOLD );
  mvwprintw( this->w_main,  0, 40, APPLICATION_VERSION );
  attroff( A_BOLD );
  mvwprintw( this->w_main,  2, 40, "level %i / %i", this->config->current_level, this->config->nb_levels );
  mvwprintw( this->w_main,  3, 40, "levelset : %s ", this->config->levelset->c_str(  ) );
  mvwprintw( this->w_main,  4, 40, "%i gifts ; %i moves ", this->engine->nb_gifts, this->engine->nb_moves );
  mvwprintw( this->w_main,  5, 40, "ARROWS to move" );
  mvwprintw( this->w_main,  6, 40, "SPACE to switch ball/cube" );
  mvwprintw( this->w_main,  7, 40, "N next level, P previous level" );
  mvwprintw( this->w_main,  8, 40, "R to restart the current level" );
  mvwprintw( this->w_main,  9, 40, "and Q to quit" );
  mvwprintw( this->w_main, 10, 40, "" );
  attroff( COLOR_PAIR( color_TEXT ));
}

void Ncurses_UI::clear_board( void )
{
  int i, j;
  int x, y;

  /* clear board */
  for ( i=NGSTAR_LEVEL_HEIGHT ; i-- ; ) {
    for ( j=NGSTAR_LEVEL_WIDTH ; j-- ; ) {
      x = 2+(NGSTAR_LEVEL_WIDTH-j-1)*2;
      y = NGSTAR_LEVEL_HEIGHT-i;
      
      attron( COLOR_PAIR( color_EMPTY ));
      mvwprintw( w_main, y, x, SYMBOL_EMPTY );
      attroff( COLOR_PAIR( color_EMPTY ));
      refresh(  );
      usleep( 5000 );
    }
  }
}

void Ncurses_UI::display_score( void )
{
  this->display(  );
  this->clear_board(  );

  attron( COLOR_PAIR( color_TEXT ));
  attron( A_BOLD );
  mvwprintw( this->w_main,  2, 5, "level cleared !" );
  mvwprintw( this->w_main,  4, 5, "moves : %i", this->engine->nb_moves );
  mvwprintw( this->w_main,  5, 5, "time  : %i seconds", this->engine->get_duration(  ) );
  mvwprintw( this->w_main,  6, 5, "      ---------" );
  mvwprintw( this->w_main,  7, 5, "Score : %i", this->scores->get( config->current_level ) );
  attroff( A_BOLD );
  attroff( COLOR_PAIR( color_TEXT ));
  wrefresh( this->w_main );

  this->display_info_panel();
  refresh();

  sleep( 1 );

  this->clear_board(  );
}

void Ncurses_UI::display_finished_levelset( void )
{
  int i;
  bool all_levels_cleared = true;
  string *remaining_levels = new string( "\t" );

  for ( i=0 ; i<this->config->nb_levels ; ++i ) {
    if ( this->scores->get( i ) == -1 ) {
      all_levels_cleared = false;
      remaining_levels->append( itos( i + 1 ) );
      remaining_levels->append( " " );
      if ( ( ( i + 1 ) % 8 ) == 0 )
	remaining_levels->append( "\n\t" );
    }
  }

  this->display(  );
  this->clear_board(  );

  attron( COLOR_PAIR( color_TEXT ));
  attron( A_BOLD );
  mvwprintw( this->w_main,  2, 5, "levelset ends here !" );
  if ( all_levels_cleared == true )
    mvwprintw( this->w_main,  4, 5, "all levels cleared !" );
  else {
    mvwprintw( this->w_main,  4, 5, "levels not cleared :" );
    mvwprintw( this->w_main,  5, 5, "%s", remaining_levels->c_str(  ) );
  }
  attroff( A_BOLD );
  mvwprintw( this->w_main,  9, 5, "(press a key to exit)" );
  attroff( COLOR_PAIR( color_TEXT ));
  wrefresh( this->w_main );

  this->display_info_panel();
  refresh();

  //  sleep( 1 );

  //  this->clear_board(  );
}

void Ncurses_UI::display( void )
{
  this->display_board();
  this->display_info_panel();
  refresh();
}

void Ncurses_UI::keys_handling( int key, bool allow_moving )
{
//  beep();
  switch ( key )
  {
  case ' ' :
    this->engine->switch_moving();
    break;
  case 'n' :
  case 'N' :
  case '>' :
    try {
      this->engine->next_level(  );
    }
    catch ( LoadingLevelException lle ) {
      cerr << lle << endl;
      this->~Ncurses_UI(  );
      exit( -1 );
    }
    break;
  case 'p' :
  case 'P' :
  case '<' :
    try {
      this->engine->previous_level(  );
    }
    catch ( LoadingLevelException lle ) {
      cerr << lle << endl;
      this->~Ncurses_UI(  );
      exit( -1 );
    }
    break;
  case 'r' :
  case 'R' :
    try {
      this->engine->load_level(  );
    }
    catch ( LoadingLevelException lle ) {
      cerr << lle << endl;
      this->~Ncurses_UI(  );
      exit( -1 );
    }
    break;
  case 'q' :
  case 'Q' :
    break ;
  default:
    if ( allow_moving == true ) {
      switch( key )
	{
	case 'o' :
	case KEY_UP    :
	  this->engine->move( go_up );
	  break;
	case 'l' :
	case KEY_DOWN  :
	  this->engine->move( go_down );
	  break;
	case 'k' :
	case KEY_LEFT  :
	  this->engine->move( go_left );
	  break;
	case 'm' :
	case KEY_RIGHT :
	  this->engine->move( go_right );
	  break;
	default:
	  break;
	}
    }
  }
}

void Ncurses_UI::run( void )
{
  int key;

  do {
    this->display();
    key = getch();
    this->keys_handling( key, true );

    // if the level is over, load the next one
    if ( this->engine->is_it_over() == true ) {
      this->scores->set(  );
      this->display_score(  );
      this->keys_handling( key, false );
      
      if ( config->current_level < config->nb_levels ) {
	try {
	  this->engine->next_level(  );
	}
	catch ( InvalidLevelFile ilf ) {
	  cerr << ilf << endl;
	  this->~Ncurses_UI(  );
	  exit( -1 );
	}
	catch ( InvalidCellValue icv ) {
	  cerr << icv << endl;
	  this->~Ncurses_UI(  );
	  exit( -1 );
	}
	catch ( InvalidCellCoordinates icc ) {
	  cerr << icc << endl;
	  this->~Ncurses_UI(  );
	  exit( -1 );
	}
	this->engine->nb_moves = 0;
      }
      else {
	// if there isn't a next one, quit
	// but let's draw the board a last time so it can be seen
	this->display(  );
	this->display_finished_levelset(  );

	getch(  );

	this->~Ncurses_UI(  );
	cout << "So you've finished this levelset..." << endl;
	exit( 0 );
      }
    }
  } while (( key != 'q' ) && ( key != 'Q' ));

  // Have to call ui's destructor explicitly because it won't do it itself ?!..
  this->~Ncurses_UI(  );

}
