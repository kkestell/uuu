game: src/main.c src/window.c src/renderer.c
	cc -Wall -o bin/uuu src/main.c src/window.c src/renderer.c -lm -lSDL2 -lcglm

clean:
	rm bin/libgame