
ifeq ($(OS),Windows_NT)
    EXE = mino.exe
    LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lopengl32
else
    EXE = mino
    LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lGL -lGLU -lm
endif

CC = gcc
CFLAGS = -Wall -std=c11
SRC = main.c world.c graphics.c player.c map_data.c sound.c draw.c UI.c engine.c

all: $(EXE)

$(EXE): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(EXE) $(LDFLAGS)

clean:
	rm -f $(EXE)