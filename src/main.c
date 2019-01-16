#include <stdbool.h>
#include <stdint.h>

#include <SDL2/SDL.h>

#include "config.h"
#include "window.h"

int main()
{
    window_init();

    uint64_t old_time = SDL_GetPerformanceCounter();
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
        new_time = SDL_GetPerformanceCounter();

        delta_time = (float)((new_time - old_time) * 1000 / 
            (double)SDL_GetPerformanceFrequency());
        //printf("%f\n", delta_time);

        window_draw(delta_time);
    }

    return 0;
}