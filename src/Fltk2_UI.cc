/*
 *  Fltk2_UI.cc
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

#include "Fltk2_UI.hh"

Fltk2_UI::Fltk2_UI( NGStar2 *engine, Configuration *config, Scores *scores )
{
  string *strtmp_path;
  string *strtmp_items;
  
  // first take the hook on configuration and engine
  this->config = config;
  this->engine = engine;
  this->scores = scores;
  
  // then create the main (and only) window 
  this->w_main = new Window( W_MAIN_WIDTH, W_MAIN_HEIGHT, APPLICATION_VERSION );
  this->w_main->resizable( this->w_main );

  // and populate it
  this->w_main->begin(  );
  { // no, putting this in a block has no utility aside from making the code easier to read ;)

    // first line of controls widgets
    this->menus = new MenuBar(0, 0, 320, WIDGETS_HEIGHT);
    this->menus->user_data(this); // make this be passed to all menu callbacks
    this->menus->add( "&File/&Restart Level", 'r', (Callback*)this->cb_menu_restart_level );
    this->menus->add( "&File/Show scores", 0, (Callback*)this->cb_menu_show_scores );
    this->menus->add( "&File/Exit", 'q', (Callback*)this->cb_menu_quit );
    
    this->sm_levelsets = new ItemGroup( "Levelsets" );
    { // idem
      strtmp_items = new string(  );
      strtmp_path = new string( this->config->levelsets_path->c_str(  ) );
      this->make_list( strtmp_path, strtmp_items, this->sm_levelsets, this->cb_menu_levelset );
      strtmp_path = new string( getenv( "HOME" ) );
      strtmp_path->append( "/.ngstar/levelsets/" );
      if ( check_dir_existence( strtmp_path->c_str(  ) ) == true )
	this->make_list( strtmp_path, strtmp_items, this->sm_levelsets, this->cb_menu_levelset ); 
      strtmp_path = new string( "./datas/levelsets/" );
      if ( check_dir_existence( strtmp_path->c_str(  ) ) == true )
	this->make_list( strtmp_path, strtmp_items, this->sm_levelsets, this->cb_menu_levelset );
    }
    this->menus->add( this->sm_levelsets );

    this->sm_themes = new ItemGroup( "Themes" );
    { // idem
      strtmp_items = new string(  );
      strtmp_path = new string( this->config->themes_path->c_str(  ) );
      this->make_list( strtmp_path, strtmp_items, this->sm_themes, this->cb_menu_theme );
      strtmp_path = new string( getenv( "HOME" ) );
      strtmp_path->append( "/.ngstar/themes/" );
      if ( check_dir_existence( strtmp_path->c_str(  ) ) == true )
	this->make_list( strtmp_path, strtmp_items, this->sm_themes, this->cb_menu_theme ); 
      strtmp_path = new string( "./datas/themes/" );
      if ( check_dir_existence( strtmp_path->c_str(  ) ) == true )
	this->make_list( strtmp_path, strtmp_items, this->sm_themes, this->cb_menu_theme );
    }
    this->menus->add( this->sm_themes );

    this->menus->add( "&Help/&License", 0, (Callback*)cb_menu_license );
    this->menus->add( "&Help/&About", 0, (Callback*)cb_menu_about );
    
    // second line of controls widgets
    this->vi_current_level = new ValueInput( 40, WIDGETS_HEIGHT, VI_CURRENT_LEVEL_WIDTH, WIDGETS_HEIGHT );
    this->vi_current_level->value( 1 );
    this->vi_current_level->range( 1, this->config->nb_levels );
    this->vi_current_level->step( 1 );
    this->vi_current_level->label( "Level" );
    this->vi_current_level->align( ALIGN_LEFT );
    this->vi_current_level->tooltip( "Go to a previous (p) or next (n) level" );
    this->vi_current_level->box( THIN_DOWN_BOX );
    ATTACH_CALLBACK( this->vi_current_level, this->cb_vi_current_level );

    this->o_nb_gifts = new Output( W_MAIN_WIDTH / 3, WIDGETS_HEIGHT, 30, WIDGETS_HEIGHT );
    this->o_nb_gifts->value( ( itos( this->engine->nb_gifts ) ).c_str(  ) );
    this->o_nb_gifts->box( THIN_DOWN_BOX );
    this->o_nb_gifts->label( "gift(s)" );
    this->o_nb_gifts->align( ALIGN_RIGHT );
    this->o_nb_gifts->tooltip( "How many gifts are left to be eaten" );
     ATTACH_CALLBACK( this->o_nb_gifts, this->cb_o_nb_gifts );

    this->o_nb_moves = new Output( ( W_MAIN_WIDTH / 3 ) * 2, WIDGETS_HEIGHT, 30, WIDGETS_HEIGHT );
    this->o_nb_moves->value( ( itos( this->engine->nb_moves ) ).c_str(  ) );
    this->o_nb_moves->box( THIN_DOWN_BOX );
    this->o_nb_moves->label( "move(s)" );
    this->o_nb_moves->align( ALIGN_RIGHT );
    this->o_nb_moves->tooltip( "How many moves you have made so far" );
    ATTACH_CALLBACK( this->o_nb_moves, this->cb_o_nb_moves );

    // and finally the board
    this->board = new Board( 0, WIDGETS_HEIGHT * 2, W_MAIN_WIDTH, W_MAIN_HEIGHT - ( WIDGETS_HEIGHT * 2 ) );
    this->board->init( this->engine, this->config, this->scores );
    ATTACH_CALLBACK( this->board, this->cb_board );

    focus( this->board );
  }
  // and it's over, we're ready to show the window
  this->w_main->end(  );
}

void Fltk2_UI::show(  )
{
  // show the new born window to the world, it's a... window
  this->w_main->show(  );
}

void Fltk2_UI::make_list( string* path, string *items, ItemGroup *menu, void (cb)(fltk::Item*, Fltk2_UI*) ) {
  DIR* dir;
  struct dirent* file;

  // Opening directory
  if ( ( dir = opendir( path->c_str() ) ) == NULL ) {
    cerr << "Can't open directory : " << path->c_str(  ) << endl;
    exit( 2 );
  }
  
  // Reading entries
  while ( ( file = readdir( dir ) ) != NULL ) {
    string* path_tmp = new string( path->c_str(  ) );
    path_tmp->append( "/" );
    path_tmp->append( file->d_name );
    
    if ( ( check_dir_existence( path_tmp->c_str(  ) ) == true ) &&
         ( (file->d_name)[0] != '.' ) &&
	 ( items->find( file->d_name ) == string::npos ) ) {
      if ( items->empty(  ) == false ) {
	items->append( "|" );
      }
      items->append( file->d_name );

      Item *tmp = new Item( file->d_name );
      ATTACH_CALLBACK( tmp, cb );
      menu->add( tmp );
    }
  }
}
    
// Callbacks
void Fltk2_UI::cb_menu_quit( Widget *, void * )
{
  exit( 0 );
}

void Fltk2_UI::cb_menu_restart_level( Widget *, Fltk2_UI *gui )
{
  try {
    gui->engine->load_level(  );
  }
  catch ( InvalidLevelFile ilf ) {
    cerr << ilf << endl;
    exit( -1 );
  }
  catch ( InvalidCellValue icv ) {
    cerr << icv << endl;
    exit( -1 );
  }
  catch ( InvalidCellCoordinates icc ) {
    cerr << icc << endl;
    exit( -1 );
  }

  gui->board->redraw(  );

  focus( gui->board );
}

void Fltk2_UI::cb_vi_current_level( ValueInput *vi_current_level, Fltk2_UI *gui )
{
  if ( vi_current_level->value(  ) < vi_current_level->minimum(  ) )
    vi_current_level->value( vi_current_level->minimum(  ) );
  else
    if ( vi_current_level->value(  ) > vi_current_level->maximum(  ) )
      vi_current_level->value( vi_current_level->maximum(  ) );
  
  gui->config->set_level( (int)vi_current_level->value(  ) );
  try {
    gui->engine->load_level(  );
  }
  catch ( InvalidLevelFile ilf ) {
    cerr << ilf << endl;
    exit( -1 );
  }
  catch ( InvalidCellValue icv ) {
    cerr << icv << endl;
    exit( -1 );
  }
  catch ( InvalidCellCoordinates icc ) {
    cerr << icc << endl;
    exit( -1 );
  }

  gui->board->do_callback(  );
  gui->board->redraw(  );

  focus( gui->board );
}

void Fltk2_UI::cb_menu_levelset( Item *m_levelset, Fltk2_UI *gui )
{
  try {
    gui->config->set_levelset( new string( m_levelset->label(  ) ) );
    gui->scores->init(  );
  }
  catch ( CannotFindLevelsets cfl ) {
    cerr << cfl << endl;
    exit( -1 );
  }
  try {
    gui->engine->load_level(  );
  }
  catch ( InvalidLevelFile ilf ) {
    cerr << ilf << endl;
    exit( -1 );
  }
  catch ( InvalidCellValue icv ) {
    cerr << icv << endl;
    exit( -1 );
  }
  catch ( InvalidCellCoordinates icc ) {
    cerr << icc << endl;
    exit( -1 );
  }

  gui->vi_current_level->value( gui->config->current_level );
  gui->vi_current_level->range( 1, gui->config->nb_levels );
  gui->board->redraw(  );

  focus( gui->board );
}

void Fltk2_UI::cb_menu_theme( Item *m_theme, Fltk2_UI *gui )
{
  try {
    gui->config->set_theme( new string( m_theme->label(  ) ) );
  }
  catch ( CannotFindThemes cft ) {
    cerr << cft << endl;
    exit( -1 );
  }
  gui->board->load_textures(  );
  gui->board->redraw(  );

  focus( gui->board );
}

void Fltk2_UI::cb_o_nb_moves( Output *o_nb_moves, Fltk2_UI *gui )
{
  o_nb_moves->value( ( itos( gui->engine->nb_moves ) ).c_str(  ) );
//   if ( gui->engine->nb_moves > 1 )
//     gui->o_nb_moves->copy_label( "moves" );
//   else
//     gui->o_nb_moves->copy_label( "move" );

  focus( gui->board );
}

void Fltk2_UI::cb_o_nb_gifts( Output *o_nb_gifts, Fltk2_UI *gui )
{
  o_nb_gifts->value( ( itos( gui->engine->nb_gifts ) ).c_str(  ) );
//   if ( gui->engine->nb_gifts > 1 )
//     o_nb_gifts->label( "gifts" );
//   else
//     o_nb_gifts->label( "gift" );

  focus( gui->board );
}

void Fltk2_UI::cb_board( Board *board, Fltk2_UI *gui )
{
  gui->o_nb_moves->do_callback(  );
  gui->o_nb_gifts->do_callback(  );
  // we don't want the level reloaded at each move so no do_callback for
  // gui->vi_current_level
  gui->vi_current_level->value( gui->config->current_level );

  gui->w_main->redraw(  );

  focus( gui->board );
}

void Fltk2_UI::cb_menu_show_scores( Widget*, Fltk2_UI *gui )
{
  int i;
  string *strtmp;

  // get levelset name
  strtmp = new string( "Levelset : " );
  strtmp->append( gui->config->levelset->c_str(  ) );
  strtmp->append( "\n\nlevel: score (The lower the better)\n" );
  // now add scores
  for ( i = 0 ; i < gui->config->nb_levels ; ++i ) {
    strtmp->append( ( itos( i + 1 ) ).c_str(  ) );
    if ( i < 9 )
      strtmp->append( "  : " );
    else
      strtmp->append( ": " );
    if ( gui->scores->get( i ) < 0 )
      strtmp->append( "not yet completed" );
    else
      strtmp->append( ( itos( gui->scores->get( i ) ) ).c_str(  ) );
    strtmp->append( "\n" );
  }

  message( strtmp->c_str(  ) );
}

void Fltk2_UI::cb_menu_about(Widget*, void*) {
  message( "NGStar\n©Gwenhael Le Moine 2003, 2004, 2005, 2006, 2006" );
}

void Fltk2_UI::cb_menu_license (Widget *, void *)
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
	  " *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.\n"
	  "\n"
	  "(See COPYING file for the full license text)"
);
}

