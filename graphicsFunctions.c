#include "chip8.h"

void setupGraphics(SDL_Window** window, SDL_Renderer** renderer){
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("Failed to initialise SDL: %s",SDL_GetError());
		exit(1);
	}
	else{
		drawFlag = 0;
		SDL_CreateWindowAndRenderer(640, 320, 0, window, renderer);
		if(*window == NULL){
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else if(*renderer == NULL){
			printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		}
	}
}

void drawGraphics(SDL_Renderer** renderer){
	//Set colour to black and then clear the screen
	SDL_SetRenderDrawColor(*renderer, 0x00, 0x00, 0x00, 255);	
	SDL_RenderClear(*renderer);
	//Set new colour to draw pixels
	SDL_SetRenderDrawColor(*renderer, 0xFF, 0xFF, 0x00, 255);	
	for(int y = 0; y < SCREEN_H/PIXEL_SIZE; y++){
		for(int x = 0; x < SCREEN_W/PIXEL_SIZE; x++){
			if(gfx[y * SCREEN_W/PIXEL_SIZE + x]){
				printPixel(PIXEL_SIZE*x, PIXEL_SIZE*y, renderer);
			}
		}
	}
	drawFlag = 0;
	SDL_RenderPresent(*renderer);
	SDL_Delay(1000/30);
}

void printPixel(int x, int y, SDL_Renderer** renderer){
	for(int i = 0; i < PIXEL_SIZE; i++){
		for(int j = 0; j < PIXEL_SIZE; j++){
			SDL_RenderDrawPoint(*renderer,x + i, y + j); 
		}
	}
}

void clearGfx(){
	for(int i = 0; i < 64 * 32; i++){
		gfx[i] = 0;
	}
}

void cleanup(SDL_Window** window, SDL_Renderer** renderer)
{
	SDL_DestroyRenderer(*renderer);
	SDL_DestroyWindow(*window);
	SDL_Quit();
}

void debugRender()
{
	for(int y = 0; y < 32; ++y)
	{
		for(int x = 0; x < 64; ++x)
		{
			if(gfx[(y*64) + x] == 0) 
				printf("O");
			else 
				printf(" ");
		}
		printf("\n");
	}
	printf("\n");
}

