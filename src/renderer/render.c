#include "render.h"

int
ver_line(int x, int y1, int y2, unsigned int color, SDL_Surface *scr)
{
        if(y2 < y1) {y1 += y2; y2 = y1 - y2; y1 -= y2;}
        if(y2 < 0 || y1 >= screenHeight || x < 0 || x >= screenWidth) return 0;
        if(y1 < 0) y1 = 0;
        if(y2 >= screenWidth) y2 = screenHeight - 1;

        unsigned int *bufp;

        bufp = (unsigned int *)scr->pixels + y1 * scr->pitch / 4 + x;
        unsigned add = scr->pitch / 4;
        for(int y = y1; y <= y2; y++){
                *bufp = color;
                bufp += add;
        }
        return 1;
}

int
hor_line(int y, int x1, int x2, unsigned int color, SDL_Surface *scr)
{
        if(x2 < x1) {x1 += x2; x2 = x1 - x2; x1 -= x2;} //swap x1 and x2, x1 must be the leftmost endpoint
        if(x2 < 0 || x1 >= screenWidth || y < 0 || y >= screenHeight) return 0; //no single point of the line is on screen
        if(x1 < 0) x1 = 0; //clip
        if(x2 >= screenWidth) x2 = screenWidth - 1; //clip

        unsigned int *bufp;
        bufp = (unsigned int *)scr->pixels + y * scr->pitch / 4 + x1;
        for(int x = x1; x <= x2; x++){
                *bufp = color;
                bufp++;
        }
        return 1;
}

unsigned int
time_left(unsigned int next_time)
{
        unsigned int now;

        now = SDL_GetTicks();
        if(next_time <= now)
                return 0;
        else
                return next_time - now;
}