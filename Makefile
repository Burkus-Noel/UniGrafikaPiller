all:
    gcc main.c world.c graphics.c player.c map_data.c -o p.exe -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lopengl32