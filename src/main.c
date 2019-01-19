#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "config.h"
#include "window.h"
#include "platform.h"

int main()
{
    /*
    size_t size;
    const char* data = getFileData(&size, "bin/assets/mesh/cube.obj");
    for (int i = 0; i < size; i++)
        printf("%c", data[i]);
    */

    windowInit();

    uint64_t oldTime = SDL_GetPerformanceCounter();
    uint64_t newTime = 0;
    float deltaTime = 0;

    while (true)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) break;
        }

        oldTime = newTime;

        do
        {
            newTime = SDL_GetPerformanceCounter();
            deltaTime = (float)((newTime - oldTime) * 1000 / 
                (double)SDL_GetPerformanceFrequency());
        }
        while (deltaTime <= 1.0f / FRAMERATE * 1000);
    
        windowDraw(deltaTime);
    }

    return 0;
}