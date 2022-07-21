CC=gcc

all:
	$(CC) -Wall -Wextra -c renderer/render.c -g
	$(CC) raycaster-int.c render.o -lSDL2 -lSDL2_ttf -lm -Wextra -Wall -g -fno-stack-protector

clean:
	rm *.o a.out
