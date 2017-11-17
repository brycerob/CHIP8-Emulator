#include "chip8.h"

void initialise()
{
	int i;

	pc     = 0x200;  // Program counter starts at 0x200
	opcode = 0;      // Reset current opcode	
	I      = 0;      // Reset index register
	sp     = 0;      // Reset stack pointer
	drawFlag = 0;    // Reset draw flag
 
	// Clear display
	clearGfx();

	// Clear stack
	// Clear registers V0-VF
	// Clear input
	for(i = 0; i<16; i++){
		stack[i] = 0;
		V[i] = 0;
		key[i] = 0;
	}

	// Clear memory
	for(i = 0; i<4096; i++){
		memory[i] = 0;
	}
 
	// Load fontset
	for(i = 0; i < 80; i++){
		memory[i] = chip8_fontset[i];		
	}
	// Reset timers
	sound_timer = 0;
	delay_timer = 0;	
}

void loadGame(char* gameName){
	FILE *game;
	unsigned char buffer[sizeof(memory)-GAME_MEM_ST];
	int bufferSize;
	bufferSize = sizeof(buffer)/sizeof(buffer[0]);
	printf("Buffer size is: %d\n",bufferSize);
	game = fopen(gameName,"rb");
	fread(buffer,bufferSize,1,game);
	for(int i = 0; i < bufferSize; i++){
		memory[i + GAME_MEM_ST] = buffer[i];
	}
	printGameMemory();
}

void printGameMemory(){
	for(int i = 512; i < 1000; i++){
		printf("%x\n",memory[i]);
	}
	SDL_Delay(2000);
}

void setKeys(SDL_Event event){
	if(inputState(event)){
		char* key_name = SDL_GetKeyName(event.key.keysym.sym);
		printf("%s\n",key_name);
		if(strcmp(key_name, "Escape") == 0){
			exit(0);
		}
		else{
			for(int i = 0; i < 0x10; i++){
				if(keymap[i] == key_name[0]){
					key[i] = 1;
				}
			}
		}
	}
}

int getKeys(SDL_Event event){
	if(inputState(event)){
		char* key_name = SDL_GetKeyName(event.key.keysym.sym);
		for(int i = 0; i < 16; i++){
			if(keymap[i] == key_name[0]){
				return i;
			}
		}
		return -1;
	}
}

void clearKeys(){
	for(int i = 0; i < 0xF; i++){
		key[i] = 0;
	}
}

bool inputState(SDL_Event event){
	if(event.type == SDL_KEYDOWN){
		return true;
	}
	return false;
}