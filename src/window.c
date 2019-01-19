#include <stdint.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "config.h"
#include "renderer.h"

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Surface* surface;
SDL_Texture* texture;

void windowInit()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error: %s", SDL_GetError());
        exit(1);
    }

    window = SDL_CreateWindow(
        "UUU",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
  
    if (window == NULL)
    {
        fprintf(stderr, "Error: %s", SDL_GetError());
        exit(1);
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (renderer == NULL)
    {
        fprintf(stderr, "Error: %s", SDL_GetError());
        exit(1);
    }

    surface = SDL_CreateRGBSurface(
        0,
        FRAMEBUFFER_WIDTH,
        FRAMEBUFFER_HEIGHT,
        32,
        0xff000000,
        0x00ff0000,
        0x0000ff00,
        0x000000ff
    );
  
    if (surface == NULL)
    {
        fprintf(stderr, "Error: %s", SDL_GetError());
        exit(1);
    }

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        FRAMEBUFFER_WIDTH,
        FRAMEBUFFER_HEIGHT
    );

    if (texture == NULL)
    {
        fprintf(stderr, "Error: %s", SDL_GetError());
        exit(1);
    }

    rendererInit();
}

void windowPutPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b)
{
    if (x < 0 || y < 0 || x >= WINDOW_WIDTH || y >= WINDOW_HEIGHT)
        return;
    
    uint32_t* p = (uint32_t*)surface->pixels + y * surface->w + x;
    *p = SDL_MapRGB(surface->format, r, g, b);
}

void windowDrawLine(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint8_t r, uint8_t g, uint8_t b)
{   
    int dx =  abs((int32_t) x1 - (int32_t) x0);
    int dy = -abs((int32_t) y1 - (int32_t) y0);
    
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1; 
    
    int err = dx + dy;
    
    int e2;
 
    while (1) 
    {
        windowPutPixel(x0, y0, r, g, b);

        if (x0 == x1 && y0 == y1) break;

        e2 = 2 * err;

        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }

        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void windowDraw(float deltaTime)
{
    SDL_LockSurface(surface);
    memset(surface->pixels, 0, surface->h * surface->pitch);

    rendererDraw(deltaTime);

    SDL_UnlockSurface(surface);
    SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void windowShutdown()
{
    rendererShutdown();

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
