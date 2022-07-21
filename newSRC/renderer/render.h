// this define is for TCC compiler to not throw errors
#ifndef RENDER_H
#define RENDER_H

#define SDL_DISABLE_IMMINTRIN_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define SCREEN_WIDTH        1280
#define SCREEN_WIDTH_RECIP  0.000781f
#define SCREEN_HEIGHT       960
#define SCREEN_HEIGHT_RECIP 0.001041f

// ticks per second, FPS limiter
#define TICK_INTERVAL 30


enum ColorCodes {
        CC_Black     = 0,
        CC_Red       = 16711680,
        CC_Green     = 65280,
        CC_Blue      = 255,
        CC_Cyan      = 65535,
        CC_Magenta   = 16711935,
        CC_Yellow    = 16776960,
        CC_White     = 16777215,
        CC_Gray      = 8421504,
        CC_Grey      = 12632256,
        CC_Maroon    = 8388608,
        CC_Darkgreen = 32768,
        CC_Navy      = 128,
        CC_Teal      = 32896,
        CC_Purple    = 8388736,
        CC_Olive     = 8421376,
};

extern int hor_line(int, int, int, const unsigned int, SDL_Surface *);
extern int ver_line(int, int, int, const unsigned int, SDL_Surface *);
extern void print_text_to_screen(TTF_Font *, SDL_Surface *, const char *, int, int);

#endif