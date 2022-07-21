#include "renderer/render.h"
#include "terminal/term.h"

#define MAP_WIDTH           24
#define MAP_HEIGHT          24

/** @breif Player information
 * This holds the player information to for the terminal to access
 */
typedef struct PlayerInfo {
        float posX, posY; // player position
        float dirX, dirY; // direction of the player
        float planeX, planeY; // camera plane

        float moveSpeed; // forward speed
        float rotSpeed; // rotational speed

        float accel; // acceleration
} PlayerInfo;


const Uint8 *inkeys = 0;

int worldMap[MAP_WIDTH * MAP_HEIGHT]=
{
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1,
  1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1,
  1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};

static void
clean_up(SDL_Window *window)
{
        terminal_buffer_destroy(); 

        SDL_DestroyWindow(window);
        SDL_Quit();
}

static void
read_keys()
{
        SDL_PumpEvents();
        inkeys = SDL_GetKeyboardState(NULL);
}

static int
key_down(Uint8 key)
{
        if(!inkeys) return 0;
        return (inkeys[key] != 0);
}

static unsigned int
time_left(const unsigned int next_time)
{
        const unsigned int now = SDL_GetTicks();

        if(next_time <= now){
                return 0;
        } else {
                return next_time - now;
        }
}

int
main(void)
{
        PlayerInfo pInfo;
        pInfo.posX = 22, pInfo.posY = 12; // x and y starts
        pInfo.dirX = -1, pInfo.dirY = 0;  // initial direction
        pInfo.planeX = 0, pInfo.planeY = 0.66; // camera plane

        pInfo.moveSpeed = 0.08, pInfo.rotSpeed = 0.05;

        insert_terminal_text("hello");
        insert_terminal_text("sup");

        insert_terminal_text("terminal");
        insert_terminal_text("super powers");

        // time until next tick
        unsigned int next_time = SDL_GetTicks() + TICK_INTERVAL;

        // window flags like RENDER_ACCELERATED could be used
        SDL_Window  *window  = SDL_CreateWindow("Erich's Mod", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
        SDL_Surface *surface = SDL_GetWindowSurface(window); 
        TTF_Init();
        TTF_Font *sans = TTF_OpenFont("resources/sans.ttf", TERMIANL_FONT_SIZE);

        unsigned int quit = 0;

        // go until quit is requested
        while(!quit){
                // loop through each vertical line on the screen
                // this is a faster way to loop through code
                for(unsigned int x = SCREEN_WIDTH; x--;){
                        // we dont have a camera vector like in true 3d so we only use
                        // X camera
                        float cameraX = 2 * x * (float)SCREEN_WIDTH_RECIP - 1;
                        // direction of the rays
                        float rayDirX = pInfo.dirX + pInfo.planeX * cameraX;
                        float rayDirY = pInfo.dirY + pInfo.planeY * cameraX;

                        // position on the map
                        int mapX = (int)pInfo.posX;
                        int mapY = (int)pInfo.posY;

                        float sideDistX, sideDistY; // length of ray from current position

                        // we want to avoid division by 0 so if it is 0 then set it to a really high value
                        float deltaDistX = (rayDirX == 0) ? INFINITY : fabsf(1 / rayDirX);
                        float deltaDistY = (rayDirY == 0) ? INFINITY : fabsf(1 / rayDirY);

                        float perpWallDist; // wall distance, not eucalidian to avoid fish eye

                        int stepX, stepY; // which map coord to step to
                        int hit = 0;
                        int side; // NS or EW hit

                        // calculate step and initial sideDist
                        if(rayDirX < 0){
                                stepX = -1;
                                sideDistX = (pInfo.posX - mapX) * deltaDistX;
                        } else {
                                stepX = 1;
                                sideDistX = (mapX + 1.0 - pInfo.posX) * deltaDistX;
                        }
                        if(rayDirY < 0){
                                stepY = -1;
                                sideDistY = (pInfo.posY - mapY) * deltaDistY;
                        } else {
                                stepY = 1;
                                sideDistY = (mapY + 1.0 - pInfo.posY) * deltaDistY;
                        }

                        // preform DDA to find wall
                        while(hit == 0) {
                                if(sideDistX < sideDistY){
                                        sideDistX += deltaDistX;
                                        mapX += stepX;
                                        side = 0;
                                } else {
                                        sideDistY += deltaDistY;
                                        mapY += stepY;
                                        side = 1;
                                }
                                if(worldMap[mapY * MAP_WIDTH + mapX] > 0) hit = 1;
                        }

                        // calculate wall distance, this will avoid fisheye
                        if(side == 0) perpWallDist = (sideDistX - deltaDistX);
                        else          perpWallDist = (sideDistY - deltaDistY);

                        // this might have to stay division
                        int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);

                        // change to mult
                        int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
                        if(drawStart < 0) drawStart = 0;
                        int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
                        if(drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

                        unsigned int color;
                        switch(worldMap[mapY * MAP_WIDTH + mapX]){
                                case 1:  color = CC_Red;     break;
                                case 2:  color = CC_Green;   break;
                                case 3:  color = CC_Blue;    break;
                                case 4:  color = CC_White;   break;
                                default: color = CC_Yellow; break;
                        }

                        if(side == 1) {color /= 2;}

                        // TODO: imp enum or other to avoid calling func in loop
                        // TODO: limit args to 4
                        ver_line(x, drawStart, drawEnd, color, surface);
                }

                draw_terminal(surface);
                draw_terminal_text(surface, sans);
                SDL_UpdateWindowSurface(window);
                SDL_FillRect(surface, NULL, 0); // fill window black

                // delay game to reach perfect 30 fps
                SDL_Delay(time_left(next_time));
                next_time += TICK_INTERVAL;

                read_keys();
                if(key_down(SDL_SCANCODE_W)){
                        if(worldMap[(int)(pInfo.posY) * MAP_WIDTH + (int)(pInfo.posX + pInfo.dirX * pInfo.moveSpeed)] == 0) pInfo.posX += pInfo.dirX * pInfo.moveSpeed;
                        if(worldMap[(int)(pInfo.posY + pInfo.dirY * pInfo.moveSpeed) * MAP_WIDTH + (int)(pInfo.posX)] == 0) pInfo.posY += pInfo.dirY * pInfo.moveSpeed;
                }
                if(key_down(SDL_SCANCODE_S)){
                        if(worldMap[(int)(pInfo.posY) * MAP_WIDTH + (int)(pInfo.posX - pInfo.dirX * pInfo.moveSpeed)] == 0) pInfo.posX -= pInfo.dirX * pInfo.moveSpeed;
                        if(worldMap[(int)(pInfo.posY - pInfo.dirY * pInfo.moveSpeed) * MAP_WIDTH + (int)(pInfo.posX)] == 0) pInfo.posY -= pInfo.dirY * pInfo.moveSpeed;
                }
                if(key_down(SDL_SCANCODE_D)){
                        float oldDirX = pInfo.dirX;
                        // TODO: maybe implement a look up table for cos/sin(-rotspeed)
                        pInfo.dirX = pInfo.dirX * cos(-pInfo.rotSpeed) - pInfo.dirY * sin(-pInfo.rotSpeed);
                        pInfo.dirY = oldDirX * sin(-pInfo.rotSpeed) + pInfo.dirY * cos(-pInfo.rotSpeed);
                        float oldPlaneX = pInfo.planeX;
                        pInfo.planeX = pInfo.planeX * cos(-pInfo.rotSpeed) - pInfo.planeY * sin(-pInfo.rotSpeed);
                        pInfo.planeY = oldPlaneX * sin(-pInfo.rotSpeed) + pInfo.planeY * cos(-pInfo.rotSpeed);
                }
                if(key_down(SDL_SCANCODE_A)){
                        float oldDirX = pInfo.dirX;
                        // TODO: maybe implement a look up table for cos/sin(-rotspeed)
                        pInfo.dirX = pInfo.dirX * cos(pInfo.rotSpeed) - pInfo.dirY * sin(pInfo.rotSpeed);
                        pInfo.dirY = oldDirX * sin(pInfo.rotSpeed) + pInfo.dirY * cos(pInfo.rotSpeed);
                        float oldPlaneX = pInfo.planeX;
                        pInfo.planeX = pInfo.planeX * cos(pInfo.rotSpeed) - pInfo.planeY * sin(pInfo.rotSpeed);
                        pInfo.planeY = oldPlaneX * sin(pInfo.rotSpeed) + pInfo.planeY * cos(pInfo.rotSpeed);
                }
                if(key_down(SDL_SCANCODE_Q)){
                        quit = 1;
                }
        }
        TTF_CloseFont(sans);
        clean_up(window);
}
