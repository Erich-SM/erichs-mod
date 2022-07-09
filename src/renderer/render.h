#pragma once

// for tcc
#define SDL_DISABLE_IMMINTRIN_H
#include <SDL2/SDL.h>

#define screenWidth  1280
#define screenHeight 960
#define mapWidth     24
#define mapHeight    24 

#define TICK_INTERVAL 30 // or 60, hell even 120

typedef struct ColorRGB {
        int r, g, b;
} ColorRGB;

static const ColorRGB RGB_Black     = {  0,   0,   0};
static const ColorRGB RGB_Red       = {255,   0,   0};
static const ColorRGB RGB_Green     = {  0, 255,   0};
static const ColorRGB RGB_Blue      = {  0,   0, 255};
static const ColorRGB RGB_Cyan      = {  0, 255, 255};
static const ColorRGB RGB_Magenta   = {255,   0, 255};
static const ColorRGB RGB_Yellow    = {255, 255,   0};
static const ColorRGB RGB_White     = {255, 255, 255};
static const ColorRGB RGB_Gray      = {128, 128, 128};
static const ColorRGB RGB_Grey      = {192, 192, 192};
static const ColorRGB RGB_Maroon    = {128,   0,   0};
static const ColorRGB RGB_Darkgreen = {  0, 128,   0};
static const ColorRGB RGB_Navy      = {  0,   0, 128};
static const ColorRGB RGB_Teal      = {  0, 128, 128};
static const ColorRGB RGB_Purple    = {128,   0, 128};
static const ColorRGB RGB_Olive     = {128, 128,   0};

extern int ver_line(int x, int y1, int y2, unsigned int color, SDL_Surface *);
extern int hor_line(int y, int x1, int x2, unsigned int color, SDL_Surface *);
extern unsigned int time_left(unsigned int next_time);