BIN=./bin
SRC=./src
SRCLIB=./src/libwam
SRCPSM=./src/psm
SRCGUI=./src/gui
SRCENGINE=./src/engine
CXX=g++

LIBWAM_OBJ=$(SRCLIB)/trigocossin.o \
           $(SRCLIB)/random.o \
		   $(SRCLIB)/intervals.o \
		   $(SRCLIB)/color.o \
		   $(SRCLIB)/stringutils.o \
		   $(SRCLIB)/configreader.o \
		   $(SRCLIB)/timeit.o \
		   $(SRCLIB)/astar.o \
		   $(SRCLIB)/vector2.o
PSM_OBJ=$(SRCPSM)/psm_common.o \
        $(SRCPSM)/psm_render.o \
		$(SRCPSM)/psm_color.o \
		$(SRCPSM)/psm_emitter.o \
		$(SRCPSM)/psm_vector.o \
		$(SRCPSM)/psm_mover.o \
		$(SRCPSM)/psm_systemBase.o
GUI_OBJ=$(SRCGUI)/widget.o \
        $(SRCGUI)/decorator.o
ENGINE_OBJ=$(SRCENGINE)/anim.o \
           $(SRCENGINE)/eventdispatcher.o \
		   $(SRCENGINE)/input.o \
		   $(SRCENGINE)/scene.o \
		   $(SRCENGINE)/ressourcemanager.o \
		   $(SRCENGINE)/mixer.o
OBJ=$(SRC)/main.o \
	$(SRC)/globals.o \
	$(SRC)/game.o \
	$(SRC)/chooselvl.o \
	$(SRC)/scene_ext.o \
	$(SRC)/decorator_ext.o \
	$(SRC)/board.o \
	$(SRC)/boardshape.o \
	$(SRC)/psmeffects.o \
	$(SRC)/gamebuffer.o \
	$(SRC)/gameconfig.o \
	$(ENGINE_OBJ) \
	$(LIBWAM_OBJ) \
	$(GUI_OBJ) \
	$(PSM_OBJ)

CXXFLAGS=-DSFML_STATIC -W -Ie:\opt\SFML\include -Isrc -Isrc/libwam -Isrc/psm -Isrc/gui -Isrc/engine -g
LFLAGS=-Le:\opt\SFML\lib -lsfml-main -lsfml-system -lsfml-graphics -lsfml-window -lsfml-audio -lopengl32

EXEC=$(BIN)/main.exe

all: $(EXEC)

%.o:%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

$(BIN)/main.exe: $(OBJ)

$(EXEC):
	$(CXX) -o $@ $^ $(LFLAGS)

.PHONY:clean

clean:
	rm -f $(OBJ) $(EXEC)