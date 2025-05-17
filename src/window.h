#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>

void window_init();

void window_draw(float deltaTime);

void window_shutdown();

// void windowDrawLine(
//     uint32_t x0,
//     uint32_t y0,
//     uint32_t x1,
//     uint32_t y1,
//     uint8_t r,
//     uint8_t g,
//     uint8_t b
// );

void window_put_pixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);

#endif
