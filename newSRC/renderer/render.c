#include "render.h"

int
hor_line(int y, int x1, int x2, const unsigned int color, SDL_Surface *scr)
{
        if(x2 < x1) {x1 += x2; x2 = x1 - x2; x1 -= x2;} // swap X
        if(x2 < 0 || y < 0 || x1 >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) return 0; // no point of line is on screen
        if(x1 < 0) x1 = 0; // clip
        if(x2 >= SCREEN_WIDTH) x2 = SCREEN_WIDTH - 1; // clip

        unsigned int *bufp;
        bufp = (unsigned int *)scr->pixels + y * (int)(scr->pitch * 0.25) + x1;
        for(int x = x1; x <= x2; x++){
                *bufp = color;
                bufp++;
        }
        return 1;
}

int
ver_line(int x, int y1, int y2, const unsigned int color, SDL_Surface *scr)
{
        // swap y1 and y2
        if(y2 < y1){y1 += y2; y2 = y1 - y2; y1 -= y2;}
        if(y2 < 0 || x < 0 || y1 >= SCREEN_HEIGHT || x >= SCREEN_WIDTH) return 0;
        if(y1 < 0) y1 = 0;
        if(y2 >= SCREEN_WIDTH) y2 = SCREEN_HEIGHT - 1;

        unsigned int *bufp;
        bufp = (unsigned int *)scr->pixels + y1 * (int)(scr->pitch * 0.25) + x;
        unsigned int add = (int)(scr->pitch * 0.25);

        for(int y = y1; y <= y2; y++){
                *bufp = color;
                bufp += add;
        }
        return 1;
}

void 
print_text_to_screen(TTF_Font *sans, SDL_Surface *surface, const char *text, int x, int y)
{
        SDL_Color white = {255, 255, 255, 255};

        SDL_Surface *textSurface = TTF_RenderText_Solid(sans, text, white);
        SDL_Rect messsageRect = {
                .x = x,
                .y = y,
                .w = abs(x * 2),
                .h = abs(y * 2),
        };

        SDL_BlitSurface(textSurface, &messsageRect, surface, NULL);
}