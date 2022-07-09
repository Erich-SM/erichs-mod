/****** DEFINES / INCLUDES ******/
// video funcs
#define SURFACE
#include "renderer/render.h"
// tcc lib is the builtin c lib
//#include <tcclib.h>
#include <stdint.h>
#include <stdio.h>

const Uint8 *inkeys = 0;

// TODO: map parsing
int worldMap[mapWidth][mapHeight]=
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

void
read_keys()
{
        SDL_PumpEvents();
        inkeys = SDL_GetKeyboardState(NULL);
}

int
key_down(Uint8 key) //this checks if the key is held down, returns true all the time until the key is up
{
        if(!inkeys) return 0;
        return (inkeys[key] != 0);
}

int
main(void)
{
        double posX = 22, posY = 12; // x and y starts
        double dirX = -1, dirY = 0; // initial direction
        double planeX = 0, planeY = 0.66; // camera plane

        unsigned int next_time = SDL_GetTicks() + TICK_INTERVAL;

        unsigned int drawterm = 0;

        /* NOTE FOR PATCHERS!!!
         * Try to keep SDL2/1 to a minimun, this game aims to be portable and SDL1 might be
         * easier to port. So keep as much SDL1 gimics as you can.
         * e.g: surface (surface uses lots of SDL2 but the interface is similar to v1)
         * SDL events will be hell to make work, but then again most movement code can be left to
         * the minimum.
         */ 
        SDL_Window *window = SDL_CreateWindow("Erich's Mod", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, 0);
        SDL_Surface *surface = SDL_GetWindowSurface(window);

        // TODO: handle quits ourselves
        while(!SDL_QuitRequested()){
                // loop through each vertical stripe of the screen
                for(int x = 0; x < screenWidth; x++){
                        double cameraX = 2 * x / (double)screenWidth - 1; // x-coord in cam space
                        // direction of the rays
                        double rayDirX = dirX + planeX * cameraX;
                        double rayDirY = dirY + planeY * cameraX;
                        // where we are according to the map
                        int mapX = (int)posX;
                        int mapY = (int)posY;

                        double sideDistX, sideDistY; // length of ray from curr pos

                        // I would make this function is assembler but I hate floats. 
                        // might make this integer raycasting not floating point for portabilty
                        double deltaDistX = (rayDirX == 0) ? INFINITY : fabs(1 / rayDirX);
                        double deltaDistY = (rayDirY == 0) ? INFINITY : fabs(1 / rayDirY);

                        double perpWallDist;

                        // what direction to step in
                        int stepX, stepY;
                        int hit = 0; // hit a wall?
                        int side; // NS or EW hit
                        
                        // calculate step and initial sideDist
                        if(rayDirX < 0){
                                stepX = -1;
                                sideDistX = (posX - mapX) * deltaDistX;
                        } else {
                                stepX = 1;
                                sideDistX = (mapX + 1.0 - posX) * deltaDistX;
                        }
                        if(rayDirY < 0){
                                stepY = -1;
                                sideDistY = (posY - mapY) * deltaDistY;
                        } else {
                                stepY = 1;
                                sideDistY = (mapY + 1.0 - posY) * deltaDistY;
                        }

                        // DDA
                        while(hit == 0){
                                if(sideDistX < sideDistY){
                                        sideDistX += deltaDistX;
                                        mapX += stepX;
                                        side = 0;
                                } else {
                                        sideDistY += deltaDistY;
                                        mapY += stepY;
                                        side = 1;
                                }
                                if(worldMap[mapX][mapY] > 0) hit = 1;
                        }


                        if(side == 0) perpWallDist = (sideDistX - deltaDistX);
                        else          perpWallDist = (sideDistY - deltaDistY);

                        int lineHeight = (int)(screenHeight / perpWallDist);

                        // these are asm-able things
                        int drawStart = -lineHeight / 2 + screenHeight / 2;
                        if(drawStart < 0) drawStart = 0;
                        int drawEnd = lineHeight / 2 + screenHeight / 2;
                        if(drawEnd >= screenHeight) drawEnd = screenHeight - 1;

                        ColorRGB color;
                        switch(worldMap[mapX][mapY]){
                                case 1:  color = RGB_Red;     break;
                                case 2:  color = RGB_Green;   break;
                                case 3:  color = RGB_Blue;    break;
                                case 4:  color = RGB_White;   break;
                                default: color = RGB_Yellow; break;
                        }

                        if(side == 1) {color.b /= 2; color.g /= 2; color.r /= 2;}

                        unsigned int colorSDL = SDL_MapRGB(surface->format, color.r, color.g, color.b);
                        ver_line(x, drawStart, drawEnd, colorSDL, surface);
                }

                SDL_UpdateWindowSurface(window);
                SDL_FillRect(surface, NULL, 65536 * 0 + 256 * 0 + 0);

                // sleeping is bad for porting
                // FIXXXXMMEEE!!
                SDL_Delay(time_left(next_time));
                next_time += TICK_INTERVAL;

                double moveSpeed = 0.08;
                double rotSpeed =  0.05;

                read_keys();

                if(key_down(SDLK_LSHIFT)){
                        printf("Drawing term\n");
                        if(drawterm == 0)
                                drawterm = 1;
                        else
                                drawterm = 0;
                }
                if(key_down(SDLK_UP)){
                        if(worldMap[(int)(posX + dirX * moveSpeed)][(int)(posY)] == 0) posX += dirX * moveSpeed;
                        if(worldMap[(int)(posX)][(int)(posY + dirY * moveSpeed)] == 0) posY += dirY * moveSpeed;
                }
                if(key_down(SDLK_DOWN)){
                        if(worldMap[(int)(posX - dirX * moveSpeed)][(int)(posY)] == 0) posX -= dirX * moveSpeed;
                        if(worldMap[(int)(posX)][(int)(posY - dirY * moveSpeed)] == 0) posY -= dirY * moveSpeed;
                }
                if(key_down(SDLK_RIGHT)){
                        //both camera direction and camera plane must be rotated
                        double oldDirX = dirX;
                        dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
                        dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
                        double oldPlaneX = planeX;
                        planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
                        planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
                }
                if(key_down(SDLK_LEFT)){
                        //both camera direction and camera plane must be rotated
                        double oldDirX = dirX;
                        dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
                        dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
                        double oldPlaneX = planeX;
                        planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
                        planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
                }
        }
}