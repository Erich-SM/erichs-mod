#include "term.h"
#include "../renderer/render.h"
#include <stdio.h>

struct TextNode {
        char cmd[20];
        int pos;
        int where;
        struct TextNode *next;
} TextNode;

unsigned int terminal_buffer_number = 0;

struct TextNode *head = NULL;
struct TextNode *current = NULL;

void
draw_terminal(SDL_Surface *surface)
{
        unsigned int innerColor = CC_Gray;
        unsigned int outerColor = CC_Grey;

        SDL_Rect innerRect = {
                .x = 600,
                .y = 20,
                .w = TERMINAL_WIDTH,
                .h = TERMINAL_HEIGHT,
        };
        SDL_FillRect(surface, &innerRect, innerColor);
        innerRect.x += 5;
        innerRect.y += 5;
        innerRect.w -= 10;
        innerRect.h -= 10;
        SDL_FillRect(surface, &innerRect, outerColor);

        unsigned int lineUp = 473;
        for(unsigned int i = 0; i < TERMINAL_LINES; i++){
                hor_line(lineUp, 600, 1245, innerColor, surface);
                lineUp -= TERMIANL_FONT_SIZE + TERMINAL_PADDING;
        }
}

void
terminal_buffer_destroy()
{
        while(!is_terminal_buffer_empty){
                free(head);
                head->next;
        }
}

void
draw_terminal_text(SDL_Surface *surface, TTF_Font *sans)
{
        int x = -610, y = -475;

        struct TextNode *ptr = head;

        while(ptr != NULL){
                print_text_to_screen(sans, surface, ptr->cmd, x, y); 
                ptr = ptr->next;
                y += TERMIANL_FONT_SIZE + TERMINAL_PADDING;
        }
}

int
is_terminal_buffer_empty()
{
        return head == NULL;
}

static int
terminal_list_length()
{
        int length = 0;
        struct TextNode *current;

        // check if while loop is better; DONE = 0
        for(current = head; current != NULL; current = current->next){
                length++;
        }

        return length;
}

struct TextNode *
delete_terminal_overflow()
{
        struct TextNode *tempLink = head;

        head = head->next;

        return tempLink;
}

/** @brief inserts lines into term
 */
void 
insert_terminal_text(const char *cmd)
{
        if(terminal_buffer_number == TERMINAL_LINES){
                struct TextNode *deleted = delete_terminal_overflow();
                printf("%s\n", deleted->cmd);
                terminal_buffer_number--;
        }
        terminal_buffer_number++;
        struct TextNode *link = (struct TextNode *)malloc(sizeof(struct TextNode));

        printf("%s\n", cmd);

        strcpy(link->cmd, cmd);
        link->pos = terminal_buffer_number;

        link->next = head;

        head = link;
}

/*
int
handle_term_input(SDL_Event *ev, struct TextNode *text, char *buffer)
{
        if(ev->type == SDL_KEYDOWN){
                if(ev->key.keysym.scancode == SDL_SCANCODE_RETURN){
                        insert_terminal_text(buffer);
                        return 1;
                }
                if(ev->key.keysym.scancode == SDL_SCANCODE_BACKSPACE){
                        text->cmd[where--]
                }
                if(ev->key.keysym.scancode == SDL_SCANCODE_SPACE){
                        strcat(buffer, " ");
                }
                if(ev->key.keysym.scancode == SDL_SCANCODE_SEMICOLON){
                        strcat(text->text, ";");
                        text->where++;
                }
                if(ev->key.keysym.scancode == SDL_SCANCODE_A){
                        //text->text[text->where] = 'A';
                        strcat(text->text, "A");
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_B){
                        strcat(text->text, "B");
                        //text->text[text->where] = 'B';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_C){
                        strcat(text->text, "C");
                        //text->text[text->where] = 'C';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_D){
                        strcat(text->text, "D");
                        //text->text[text->where] = 'D';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_E){
                        strcat(text->text, "E");
                        //text->text[text->where] = 'E';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_F){
                        strcat(text->text, "F");
                        //text->text[text->where] = 'F';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_G){
                        strcat(text->text, "G");
                        //text->text[text->where] = 'G';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_H){
                        strcat(text->text, "H");
                        //text->text[text->where] = 'H';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_I){
                        strcat(text->text, "I");
                        //text->text[text->where] = 'I';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_J){
                        strcat(text->text, "J");
                        //text->text[text->where] = 'J';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_K){
                        strcat(text->text, "K");
                        //text->text[text->where] = 'K';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_L){
                        strcat(text->text, "L");
                        //text->text[text->where] = 'L';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_M){
                        strcat(text->text, "M");
                        //text->text[text->where] = 'M';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_N){
                        strcat(text->text, "N");
                        //text->text[text->where] = 'N';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_O){
                        strcat(text->text, "O");
                        //text->text[text->where] = 'O';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_P){
                        strcat(text->text, "P");
                        //text->text[text->where] = 'P';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_Q){
                        strcat(text->text, "Q");
                        //text->text[text->where] = 'Q';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_R){
                        strcat(text->text, "R");
                        //text->text[text->where] = 'R';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_S){
                        strcat(text->text, "S");
                        //text->text[text->where] = 'S';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_T){
                        strcat(text->text, "T");
                        //text->text[text->where] = 'T';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_U){
                        strcat(text->text, "U");
                        //text->text[text->where] = 'U';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_V){
                        strcat(text->text, "V");
                        //text->text[text->where] = 'V';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_W){
                        strcat(text->text, "W");
                        //text->text[text->where] = 'W';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_X){
                        strcat(text->text, "X");
                        //text->text[text->where] = 'X';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_Y){
                        strcat(text->text, "Y");
                        //text->text[text->where] = 'Y';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_Z){
                        strcat(text->text, "Z");
                        //text->text[text->where] = 'Z';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_0){
                        strcat(text->text, "0");
                        //text->text[text->where] = '0';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_1){
                        strcat(text->text, "1");
                        //text->text[text->where] = '1';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_2){
                        strcat(text->text, "2");
                        //text->text[text->where] = '2';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_3){
                        strcat(text->text, "3");
                        //text->text[text->where] = '3';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_4){
                        strcat(text->text, "4");
                        //text->text[text->where] = '4';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_5){
                        strcat(text->text, "5");
                        //text->text[text->where] = '5';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_6){
                        strcat(text->text, "6");
                        //text->text[text->where] = '6';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_7){
                        strcat(text->text, "7");
                        //text->text[text->where] = '7';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_8){
                        strcat(text->text, "8");
                        //text->text[text->where] = '8';
                        text->where++;
                }if(ev->key.keysym.scancode == SDL_SCANCODE_9){
                        strcat(text->text, "9");
                        //text->text[text->where] = '9';
                        text->where++;
                }
        }
        return 0;
}
*/