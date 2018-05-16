CC = gcc
OBJS = SPMiniMax.o SPArrayList.o SPFIARParser.o \
SPMainWindow.o SPLoadWindow.o GameModeWindow.o SPDifficultyWindow.o \
SPColorWindow.o SPGameWindow.o SPGUImanager.o main.o ConsoleMode.o LoadStore.o SimpleButton.o \
SimpleChessPiece.o Widget.o SPChessGame.o TwoStateButton.o
EXEC = chessprog
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors
SDL_COMP_FLAG = -I/usr/local/lib/sdl_2.0.5/include/SDL2 -D_REENTRANT
SDL_LIB = -L/usr/local/lib/sdl_2.0.5/lib -Wl,-rpath,/usr/local/lib/sdl_2.0.5/lib -Wl,--enable-new-dtags -lSDL2 -lSDL2main

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(SDL_LIB) -o $@
SPChessGame.o: SPChessGame.c SPChessGame.h SPMiniMax.h SPArrayList.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPGUImanager.o: SPGUImanager.c SPGUImanager.h SPMainWindow.h SPLoadWindow.h \
GameModeWindow.h SPDifficultyWindow.h SPColorWindow.h SPGameWindow.h SPChessGame.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPMainWindow.o: SPMainWindow.h SPMainWindow.c SPWindow.h active_window.h Widget.h SimpleButton.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
ConsoleMode.o: ConsoleMode.h ConsoleMode.c SPFIARParser.h LoadStore.h SPChessGame.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
LoadStore.o: LoadStore.h LoadStore.c SPChessGame.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SimpleButton.o: SimpleButton.h SimpleButton.c Widget.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
TwoStateButton.o: TwoStateButton.h TwoStateButton.c Widget.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SimpleChessPiece.o: SimpleChessPiece.h SimpleChessPiece.c Widget.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
Widget.o: Widget.h Widget.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPLoadWindow.o: SPLoadWindow.h SPLoadWindow.c active_window.h  TwoStateButton.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
GameModeWindow.o: GameModeWindow.h GameModeWindow.c SPWindow.h active_window.h Widget.h \
SimpleButton.h TwoStateButton.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPDifficultyWindow.o: SPDifficultyWindow.h SPDifficultyWindow.c SimpleButton.h \
SPWindow.h active_window.h Widget.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPColorWindow.o: SPColorWindow.h SPColorWindow.c SPWindow.h SimpleButton.h \
active_window.h Widget.h TwoStateButton.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPGameWindow.o: SPGameWindow.h SPGameWindow.c SPChessGame.h active_window.h SPWindow.h \
Widget.h SimpleChessPiece.h LoadStore.h SimpleButton.h  SPGameWindow.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPArrayList.o: SPArrayList.h SPArrayList.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPFIARParser.o: SPFIARParser.c SPFIARParser.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPMiniMax.o: SPMiniMax.c SPMiniMax.h SPChessGame.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
main.o: main.c SPChessGame.h SPMiniMax.h SPGUImanager.h ConsoleMode.h SPFIARParser.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
all: $(OBJS)
	$(CC) $(OBJS) $(SDL_LIB) -o $(EXEC)
clean:
	rm -f *.o $(EXEC)
