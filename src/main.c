#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "config.h"
#include "window.h"
#include "platform.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <mesh_filename>\n", argv[0]);
        exit(1);
    }

    window_init(argv[1]);

    uint64_t old_time = 0;
    uint64_t new_time = 0;
    float delta_time = 0;

    while (true)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) break;
        }

        old_time = new_time;

        do
        {
            new_time = SDL_GetPerformanceCounter();
            delta_time = (float)(((double)(new_time - old_time) * 1000.0) / (double)SDL_GetPerformanceFrequency());
        }
        while (delta_time <= 1.0f / FRAMERATE * 1000);

        window_draw(delta_time);
    }

    return 0;
}