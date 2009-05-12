include ../Makefile.config

CC_INCLUDES = `fltk2-config --use-images --cxxflags`
CC_LIBS = -L../ -lngstar2 
CC_LIBS_STATIC = -L../ -lngstar2 
BIN_DEPS = 
ifdef USE_FLTK2
	CPPFLAGS += -D 'THEMES_PATH="$(THEMES_DIR)"'
	CC_LIBS += `fltk2-config --use-images --ldflags` -lpng -lz
	CC_LIBS_STATIC += `fltk2-config --use-images --ldflags` -lpng -lz -lfontconfig -lexpat -lfreetype -lXrender -lX11
else 
ifdef BUILD_EDITOR
	CPPFLAGS += -D 'THEMES_PATH="$(THEMES_DIR)"'
	CC_LIBS += `fltk2-config --use-images --ldflags` -lpng -lz
	CC_LIBS_STATIC += `fltk2-config --use-images --ldflags` -lpng -lz -lfontconfig -lexpat -lfreetype -lXrender -lX11
endif
endif
ifdef USE_FLTK2
	FLTK2_UI_OBJ=Fltk2_UI.o
	BOARD_OBJ=Board.o

	BIN_DEPS += $(FLTK2_UI_OBJ) $(BOARD_OBJ)
endif
ifdef BUILD_EDITOR
	LM_UI_OBJ=LevelMaker_UI.o
	EBOARD_OBJ=EditorBoard.o
	BOARD_OBJ=Board.o


	BIN_DEPS += $(LM_UI_OBJ) $(EBOARD_OBJ)
ifndef USE_FLTK2
	BIN_DEPS += $(BOARD_OBJ)
endif
endif
ifdef USE_NCURSES
	CC_LIBS += -lncurses -lgpm
	CC_LIBS_STATIC += -lncurses -lgpm

	NCURSES_UI_OBJ = Ncurses_UI.o

	BIN_DEPS += $(NCURSES_UI_OBJ)
endif
SOURCE=    main.cc
BIN=       ../ngstar
BIN_FLTK2= ../xngstar
BIN_EDITOR= ../xngstar-editor
BIN_STATIC=../ngstar_static
BIN_STATIC_FLTK2=../xngstar_static
BIN_STATIC_EDITOR= ../xngstar-editor_static

all : 
	$(MAKE) $(BIN)

static : 
	$(MAKE) $(BIN_STATIC)

$(BIN) : $(SOURCE) $(BIN_DEPS)
	echo "Compiling $(BIN)"
	$(CXX) $< $(BIN_DEPS) -o $@ $(CC_LIBS) $(CC_INCLUDES) $(CPPFLAGS)
	strip -s $@
ifdef USE_FLTK2
	-ln -sf `basename $(BIN)` $(BIN_FLTK2)
endif
ifdef BUILD_EDITOR
	-ln -sf `basename $(BIN)` $(BIN_EDITOR)
endif

$(BIN_STATIC) : $(SOURCE) $(BIN_DEPS)
	echo "Compiling $(BIN)"
	$(CXX) -static $< $(BIN_DEPS) -o $@ $(CC_LIBS_STATIC) $(CC_INCLUDES) $(CPPFLAGS)
	strip -s $@
ifdef USE_FLTK2
	-ln -sf `basename $(BIN_STATIC)` $(BIN_STATIC_FLTK2)
endif
ifdef BUILD_EDITOR
	-ln -sf `basename $(BIN_STATIC)` $(BIN_STATIC_EDITOR)
endif

install : $(BIN)
	mkdir -p $(BIN_DIR)/
	$(INSTALL) $(BIN) $(BIN_DIR)/
ifdef USE_FLTK2
	$(INSTALL) $(BIN_FLTK2) $(BIN_DIR)/
endif
ifdef BUILD_EDITOR
	$(INSTALL) $(BIN_EDITOR) $(BIN_DIR)/
endif

uninstall : $(BIN_DIR)/$(BIN)
	-rm $(BIN_DIR)/$(BIN)
ifdef USE_FLTK2
	-rm $(BIN_DIR)/$(BIN_FLTK2)
endif
ifdef BUILD_EDITOR
	-rm $(BIN_DIR)/$(BIN_EDITOR)
endif

install_static : $(BIN_STATIC)
	mkdir -p $(BIN_DIR)/
	$(INSTALL) $(BIN_STATIC) $(BIN_DIR)/
ifdef USE_FLTK2
	$(INSTALL) $(BIN_STATIC_FLTK2) $(BIN_DIR)/
endif
ifdef BUILD_EDITOR
	$(INSTALL) $(BIN_STATIC_EDITOR) $(BIN_DIR)/
endif

uninstall_static : $(BIN_DIR)/$(BIN_STATIC)
	-rm $(BIN_DIR)/$(BIN_STATIC)
ifdef USE_FLTK2
	-rm $(BIN_DIR)/$(BIN_STATIC_FLTK2)
endif
ifdef BUILD_EDITOR
	-rm $(BIN_DIR)/$(BIN_STATIC_EDITOR)
endif

clean :
	-rm ./*.o ./*~

real-clean :
	$(MAKE) clean
	-rm $(BIN)
	-rm $(BIN_STATIC)
ifdef USE_FLTK2
	-rm $(BIN_FLTK2)
	-rm $(BIN_STATIC_FLTK2)
endif
ifdef BUILD_EDITOR
	-rm $(BIN_EDITOR)
	-rm $(BIN_STATIC_EDITOR)
endif
