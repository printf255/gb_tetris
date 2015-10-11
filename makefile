
CC=$(GBDKDIR)bin/lcc
BIN_RELEASE=bin/release.gb
BIN_DEBUG=bin/debug.gb

#GB_FRAMEWORK_DIR=../gb_framework/src/
GB_FRAMEWORK_SRC=../gb_framework/src/sound.c ../gb_framework/src/gfx.c
GB_FRAMEWORK_TILES=../gb_framework/src/tiles/colorRects.c

SRC=src/main.c src/block/block.c src/controller/controller.c src/model/model.c src/view/view.c
#TILES=src/tiles/white.c src/tiles/light_grey.c src/tiles/dark_grey.c src/tiles/black.c
TILES=

release: $(SRC) $(GB_FRAMEWORK_SRC)
	$(CC) -o $(BIN_RELEASE) $(SRC) $(TILES) $(GB_FRAMEWORK_SRC) $(GB_FRAMEWORK_TILES)

run_release: release
	gvba $(BIN_RELEASE)

debug: $(SRC) $(GB_FRAMEWORK_SRC)
	$(CC) -Wf-debug -o $(BIN_DEBUG) $(SRC) $(TILES) $(GB_FRAMEWORK_SRC) $(GB_FRAMEWORK_TILES)

run_debug: debug
	gvba $(BIN_DEBUG)

clean:
	rm $(BIN_RELEASE) $(BIN_DEBUG)

#debug: 





