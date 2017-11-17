#include "chip8.h"
 
//CPU Operations
unsigned short opcode;
unsigned char memory[4096];
unsigned char V[16];
unsigned short I;
unsigned short pc;
unsigned char gfx[64 * 32];
unsigned char delay_timer;
unsigned char sound_timer;
unsigned short stack[16];
unsigned short sp;
unsigned char key[16];
unsigned char drawFlag;
unsigned char chip8_fontset[80] =
{ 
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};
char keymap[16] = {'X','1','2','3','Q','W','E','A','S','D','Z','C','4','R','F','V'};


int main(int argc, char **argv) 
{
	// Set up render system and register input callbacks
	SDL_Event event;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	setupGraphics(&window, &renderer);

	// Initialize the Chip8 system and load the game into the memory  
	initialise();
	loadGame(argv[1]);

	// Emulation loop
	while(1)
	{
		bool isEvent = SDL_PollEvent(&event);
		if(isEvent){
			//Clears state of pressed keys so that the operation is not done twice
			//weclearKeys();
			if(event.type == SDL_QUIT){
				break;
			}
			else{
				setKeys(event);
			}
		}

		// Emulate one cycle
		emulateCycle(&event);
		// If the draw flag is set, update the screen
		if(drawFlag){
			drawGraphics(&renderer);
		}
	}
	cleanup(&window, &renderer);

	return 0;
}