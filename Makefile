CC = gcc
CFLAGS = -Isrc/Include -Wall
LDFLAGS = -Lsrc/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
SRC = src/main.c src/tiles.c src/headsUpDisplay.c src/game.c
OBJ = $(SRC:.c=.o)

main: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q $(subst /,\,$(OBJ)) main.exe
