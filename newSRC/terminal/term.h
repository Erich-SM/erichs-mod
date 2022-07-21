#ifndef TERM_H
#define TERM_H

#define SDL_DISABLE_IMMINTRIN_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define TERMINAL_LINES      20
#define TERMIANL_FONT_SIZE  20
#define TERMINAL_WIDTH      650
#define TERMINAL_HEIGHT     480
#define TERMINAL_PADDING    3

extern void draw_terminal(SDL_Surface *);
extern void draw_terminal_text(SDL_Surface *, TTF_Font *);
extern int is_terminal_buffer_empty();
extern void insert_terminal_text(const char *);
extern void terminal_buffer_destroy();

#endif