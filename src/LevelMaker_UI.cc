/*
 *  LevelMaker_UI.cc
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

#include "LevelMaker_UI.hh"

LevelMaker_UI::LevelMaker_UI( NGStar2 *engine, Configuration *config )
{
  // first take the hook on configuration and engine
  this->config = config;
  this->engine = engine;

  this->filename = new string( ( itos( this->config->current_level ) ).c_str(  ) );
  
  // then create the main (and only) window 
  this->w_main = new Window( W_MAIN_WIDTH, 215, APPLICATION_VERSION );
  this->w_main->resizable( this->w_main );

  // and populate it
  this->w_main->begin(  );
  { // no, putting this in a block has no utility aside from making the code easier to read ;)
    this->g_controls = new Group( 0, 0, W_MAIN_WIDTH, WIDGETS_HEIGHT * 2 );

    // first line of controls widgets
    this->menus = new MenuBar(0, 0, 320, WIDGETS_HEIGHT);
    this->menus->user_data(this); // make this be passed to all menu callbacks
    this->menus->add( "&File/&New Level", CTRL+'n', (Callback*)cb_menu_new );
    this->menus->add( "&File/&New Levelset...", CTRL+'l', (Callback*)cb_menu_new_levelset );
    this->menus->add( "&File/&Open File...", CTRL+'o', (Callback*)cb_menu_open );
    this->menus->add( "&File/&Save File", CTRL+'s', (Callback*)cb_menu_save );
    this->menus->add( "&File/Save File &As...", CTRL+SHIFT+'S', (Callback*)cb_menu_saveas );
    this->menus->add( "&File/E&xit", CTRL+'q', (Callback*)cb_menu_quit );

    this->menus->add( "&Help/&License", 0, (Callback*)cb_menu_license );
    this->menus->add( "&Help/&About", 0, (Callback*)cb_menu_about );

    // second line of controls widgets
    this->rlb_ball = new RadioLightButton( 0, WIDGETS_HEIGHT, 40, 20, "" );
    this->rlb_cube = new RadioLightButton( 40, WIDGETS_HEIGHT, 40, 20, "" );
    this->rlb_empty = new RadioLightButton( 80, WIDGETS_HEIGHT, 40, 20, "" );
    this->rlb_gift = new RadioLightButton( 120, WIDGETS_HEIGHT, 40, 20, "" );
    this->rlb_wall = new RadioLightButton( 160, WIDGETS_HEIGHT, 40, 20, "" );
    
    // by default we're putting walls
    this->rlb_wall->value( true );

    this->b_clear_board = new Button( W_MAIN_WIDTH - 20, WIDGETS_HEIGHT, 20, 20, "@square" );

    // adding controls to g_controls
    // first line
    this->g_controls->add( this->menus );
    // second line
    this->g_controls->add( this->b_clear_board );
    this->g_controls->add( this->rlb_ball );
    this->g_controls->add( this->rlb_cube );
    this->g_controls->add( this->rlb_empty );
    this->g_controls->add( this->rlb_gift );
    this->g_controls->add( this->rlb_wall );

    // and finally the board
    this->board = new EditorBoard( 0, 35, W_MAIN_WIDTH, W_MAIN_HEIGHT - ( WIDGETS_HEIGHT * 2 ), "" );
    this->board->init( this->engine, this->config );

    // Put images on radio light buttons
    this->rlb_ball->image( this->board->tex_ball );
    this->rlb_cube->image( this->board->tex_cube );
    this->rlb_empty->image( this->board->tex_empty );
    this->rlb_gift->image( this->board->tex_gift );
    this->rlb_wall->image( this->board->tex_wall );

    // Attaching callbacks to widgets that need them
    ATTACH_CALLBACK( this->b_clear_board, this->cb_b_clear_board );
    ATTACH_CALLBACK( this->board, this->cb_board );
    ATTACH_CALLBACK( this->rlb_ball, this->cb_rlb );
    ATTACH_CALLBACK( this->rlb_cube, this->cb_rlb );
    ATTACH_CALLBACK( this->rlb_empty, this->cb_rlb );
    ATTACH_CALLBACK( this->rlb_gift, this->cb_rlb );
    ATTACH_CALLBACK( this->rlb_wall, this->cb_rlb );

    // Tooltips looks good
    this->b_clear_board->tooltip( "Clear the board" );
    this->rlb_ball->tooltip( "Draw ball (thou shall only draw one)" );
    this->rlb_cube->tooltip( "Draw cube (thou shall only draw one)" );
    this->rlb_empty->tooltip( "Draw empty space" );
    this->rlb_gift->tooltip( "Draw gift" );
    this->rlb_wall->tooltip( "Draw wall" );
  }
  // and it's over, we're ready to show the window
  this->w_main->end(  );
}

void LevelMaker_UI::show(  )
{
  // show the new born window to the world, it's a... window !
  this->w_main->show(  );
}
    
// Callbacks
void LevelMaker_UI::cb_menu_quit(Widget*, LevelMaker_UI *gui ) {
  if ( gui->board->nb_changes_since_save != 0 )
    cerr << "You should have saved ..." << endl;
  exit(0);
}

void LevelMaker_UI::cb_menu_new(Widget*, LevelMaker_UI *gui ) {
  delete gui->filename;
  gui->filename = NULL;
  gui->cb_b_clear_board( gui->b_clear_board, gui );
}

void LevelMaker_UI::cb_menu_new_levelset(Widget*, LevelMaker_UI *gui ) {
  string *levelset_name;
  string *levelset_path;

  delete gui->filename;

  levelset_name = new string( input( "Enter the name of the new levelset : " ) );
  levelset_path = new string( getenv( "HOME" ) );
  levelset_path->append( "/.ngstar/levelsets/" );
  if ( check_dir_existence( levelset_path->c_str(  ) ) == false ) {
    alert( "Please create $HOME/.ngstar/ with\nngstar --create-default-config" );
  }
  else {
    levelset_path->append( levelset_name->c_str(  ) );
    mkdir( levelset_path->c_str(  ), S_IRWXO | S_IRWXG | S_IRWXU );
 
    levelset_name = new string( levelset_path->c_str(  ) );
    levelset_name->append( "/1" );
    gui->filename = new string( levelset_name->c_str(  ) );
    gui->cb_b_clear_board( gui->b_clear_board, gui );
  }
}

void LevelMaker_UI::cb_menu_open(Widget*, LevelMaker_UI *gui ) {
  //use_system_file_chooser( true );
  try {
    gui->filename = new string( file_chooser( "Select a level" , "[0-9]*", "./" ) );
    
    gui->engine->load_level( gui->filename->c_str(  ) );
    gui->board->redraw(  );
  }
  catch ( logic_error le ) {}
}

void LevelMaker_UI::cb_menu_save(Widget*, LevelMaker_UI* gui) {
  if ( gui->filename == NULL )
    gui->cb_menu_saveas( (Widget *)0, gui );
  else
    gui->save_to_file(  );
}

void LevelMaker_UI::cb_menu_saveas(Widget*, LevelMaker_UI *gui ) {
  try {
    gui->filename = new string( file_chooser( "Please remember that level are named numerically" , "[0-9]*", "./" ) );
  
    gui->save_to_file(  );
  }
  catch ( logic_error le ) {}
}

void LevelMaker_UI::cb_menu_about(Widget*, void*) {
  message( "NGStar2 Level Maker\n©Cycojesus 2005, 2006, 2006" );
}

void LevelMaker_UI::cb_menu_license (Widget *, void *)
{
  message(" * This program is free software; you can redistribute it and/or modify\n"
	  " *  it under the terms of the GNU General Public License as published by\n"
	  " *  the Free Software Foundation; either version 2 of the License, or\n"
	  " *  (at your option) any later version.\n"
	  " *\n"
	  " *  This program is distributed in the hope that it will be useful,\n"
	  " *  but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
	  " *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
	  " *  GNU Library General Public License for more details.\n"
	  " *\n"
	  " *  You should have received a copy of the GNU General Public License\n"
	  " *  along with this program; if not, write to the Free Software\n"
	  " *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.\n");
}

void LevelMaker_UI::cb_b_clear_board( Button *b_clear_board, LevelMaker_UI *gui )
{
  gui->board->clear(  );
  gui->board->redraw(  );

  focus( gui->board );
}

void LevelMaker_UI::cb_board( EditorBoard *board, LevelMaker_UI *gui )
{
  focus( gui->board );
}

void LevelMaker_UI::cb_rlb( RadioLightButton *rlb, LevelMaker_UI *gui )
{
  if ( rlb == gui->rlb_ball )
    gui->board->cell_to_put = ball;
  else
    if ( rlb == gui->rlb_cube )
      gui->board->cell_to_put = cube;
    else
      if ( rlb == gui->rlb_empty )
	gui->board->cell_to_put = ngstar2::empty;
      else
	if ( rlb == gui->rlb_gift )
	  gui->board->cell_to_put = gift;
	else
	  if ( rlb == gui->rlb_wall )
	    gui->board->cell_to_put = wall;
}

void LevelMaker_UI::save_to_file(  )
{
  int i, j;
  int nb_balls=0,
    nb_cubes=0,
    nb_gifts=0;

  // Validating level
  for ( i=this->engine->level_h ; i-- ; ) {
    for ( j=this->engine->level_w ; j-- ; ) {
      switch( this->engine->board[i*this->engine->level_w+j] ) {
      case ball:
	++nb_balls;
	break;
      case cube:
	++nb_cubes;
	break;
      case ngstar2::empty:
	break;
      case gift:
	++nb_gifts;
	break;
      case wall:
	break;
      default:
	cerr << "What was that ?!" << endl;
	break;
      }
    }
  }
  if ( ( nb_balls != 1 ) ||
       ( nb_cubes != 1 ) ||
       ( nb_gifts == 0 ) ) {
    alert( "Invalid level." );
  }
  else {
    ofstream file;
    
    file.open( this->filename->c_str(  ), ios::out );
    
    file << this->board;
    
    this->board->nb_changes_since_save = 0;
    
    file.close(  );
    file.clear(  );
  }
}
