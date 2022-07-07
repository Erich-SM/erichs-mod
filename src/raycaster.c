/****** DEFINES / INCLUDES ******/
// video funcs
#define SDL_DISABLE_IMMINTRIN_H
#include <SDL2/SDL.h>
// tcc lib is the builtin c lib
//#include <tcclib.h>
#include <stdio.h>


#define screenWidth  1280
#define screenHeight 960
#define mapWidth  24
#define mapHeight 24

#define TICK_INTERVAL 30

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

//Fast vertical line from (x,y1) to (x,y2), with rgb color
int
verLine(int x, int y1, int y2, unsigned int color, SDL_Surface *scr)
{
  if(y2 < y1) {y1 += y2; y2 = y1 - y2; y1 -= y2;} //swap y1 and y2
  if(y2 < 0 || y1 >= screenHeight  || x < 0 || x >= screenWidth) return 0; //no single point of the line is on screen
  if(y1 < 0) y1 = 0; //clip
  if(y2 >= screenWidth) y2 = screenHeight - 1; //clip

  unsigned int *bufp;

  bufp = (Uint32*)scr->pixels + y1 * scr->pitch / 4 + x;
  unsigned add = scr->pitch / 4;
  for(int y = y1; y <= y2; y++)
  {
     *bufp = color;
     bufp += add;
  }
  return 1;
}

static int
keyDown(SDL_Event *event, int code)
{
        switch(event->type){
                case SDL_KEYDOWN:
                        if(event->key.keysym.scancode == code){
                                return 1;
                        } else {
                                return 0;
                        }
                default:
                        return 0; 
        }
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

int
main(void)
{
        double posX = 22, posY = 12; // x and y starts
        double dirX = -1, dirY = 0; // initial direction
        double planeX = 0, planeY = 0.66; // camera plane

        unsigned int next_time = SDL_GetTicks() + TICK_INTERVAL;

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

                        if(side == 1) {color.b / 2; color.g / 2; color.r / 2;}

                        unsigned int colorSDL = SDL_MapRGB(surface->format, color.r, color.g, color.b);
                        verLine(x, drawStart, drawEnd, colorSDL, surface);
                }

                SDL_UpdateWindowSurface(window);
                SDL_FillRect(surface, NULL, 65536 * 0 + 256 * 0 + 0);

                // sleeping is bad for porting
                // FIXXXXMMEEE!!
                SDL_Delay(time_left(next_time));
                next_time += TICK_INTERVAL;

                double moveSpeed = 2.0;
                double rotSpeed =  0.5;

                SDL_Event event;

                SDL_PollEvent(&event);                

                //move forward if no wall in front of you
                // add much better collision
                if(keyDown(&event, SDL_SCANCODE_W)){
                        if(worldMap[(int)(posX + dirX * moveSpeed)][(int)(posY)] == 0) posX += dirX * moveSpeed;
                        if(worldMap[(int)(posX)][(int)(posY + dirY * moveSpeed)] == 0) posY += dirY * moveSpeed;
                }
                //move backwards if no wall behind you
                if(keyDown(&event, SDL_SCANCODE_S)){
                        if(worldMap[(int)(posX - dirX * moveSpeed)][(int)(posY)] == 0) posX -= dirX * moveSpeed;
                        if(worldMap[(int)(posX)][(int)(posY - dirY * moveSpeed)] == 0) posY -= dirY * moveSpeed;
                }
                //rotate to the right
                if(keyDown(&event, SDL_SCANCODE_D)){
                        //both camera direction and camera plane must be rotated
                        double oldDirX = dirX;
                        dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
                        dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
                        double oldPlaneX = planeX;
                        planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
                        planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
                }
                //rotate to the left
                if(keyDown(&event, SDL_SCANCODE_A)){
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