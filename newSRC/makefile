CC=x86_64-w64-mingw32-gcc

all:
	$(CC) -c renderer/render.c
	$(CC) -c terminal/term.c
	$(CC) term.o render.o raycaster.c -o ray -lSDL2 -lSDL2_ttf -lm

clean:
	rm *.o
	rm ray
