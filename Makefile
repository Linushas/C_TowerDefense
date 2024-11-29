CC = gcc
CFLAGS = -Isrc/Include 
LDFLAGS = -Lsrc/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
SRC = src/main.c src/tiles.c src/headsUpDisplay.c src/game.c src/tower.c
OBJ = $(SRC:.c=.o)

main: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q $(subst /,\,$(OBJ)) main.exe
