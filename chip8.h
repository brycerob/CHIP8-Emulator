#ifndef HEADERH
#define HEADERH

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <SDL2/SDL.h>

extern unsigned short opcode;
extern unsigned char memory[4096];
extern unsigned char V[16];
extern unsigned short I;
extern unsigned short pc;
extern unsigned char gfx[64 * 32];
extern unsigned char delay_timer;
extern unsigned char sound_timer;
extern unsigned short stack[16];
extern unsigned short sp;
extern unsigned char key[16];
extern unsigned char drawFlag;
extern unsigned char chip8_fontset[80];
extern char keymap[16];

#define FONT_MEM_ST 80 
#define GAME_MEM_ST 512
#define FONT_SIZE 5 
#define SCREEN_W 640
#define SCREEN_H 320
#define PIXEL_SIZE 10

/* CPU.c */
void emulateCycle(SDL_Event *event);

/* generalFunctions.c */
void initialise();
void loadGame(char* gameName);
void getInput(SDL_Event *event);
void printGameMemory();
void setKeys(SDL_Event event);
bool inputState(SDL_Event event);
int getKeys(SDL_Event event);
void clearKeys();

/* graphicsFunctions.c */
void setupGraphics(SDL_Window** window, SDL_Renderer** renderer);
void drawGraphics(SDL_Renderer** renderer);
void printPixel(int x, int y, SDL_Renderer** renderer);
void clearGfx();
void cleanup(SDL_Window** window, SDL_Renderer** renderer);
void debugRender();

/*
Keypad                   Keyboard
+-+-+-+-+                +-+-+-+-+
|1|2|3|C|                |1|2|3|4|
+-+-+-+-+                +-+-+-+-+
|4|5|6|D|                |Q|W|E|R|
+-+-+-+-+       =>       +-+-+-+-+
|7|8|9|E|                |A|S|D|F|
+-+-+-+-+                +-+-+-+-+
|A|0|B|F|                |Z|X|C|V|
+-+-+-+-+                +-+-+-+-+  
*/

#endif