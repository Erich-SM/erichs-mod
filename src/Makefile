CC=gcc

all:
	$(CC) -c renderer/render.c
	$(CC) raycaster.c render.o -lSDL2 -lm

clean:
	rm *.o a.out
