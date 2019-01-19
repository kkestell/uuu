uuu: src/main.c src/mesh.c src/window.c src/renderer.c src/platform.c src/math.c
	cc -Wall -std=c99 -o bin/uuu \
	src/main.c \
	src/mesh.c \
	src/window.c \
	src/renderer.c \
	src/platform.c \
	src/math.c \
	-lm -lSDL2 -lcglm -lassimp

profile:
	instruments -t "Time Profile" bin/uuu

clean:
	rm bin/libgame