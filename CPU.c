#include "chip8.h"

void emulateCycle(SDL_Event *event)
{
	// Fetch opcode
	opcode = memory[pc] << 8 | memory[pc + 1];
 
	// Decode opcode
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;
	unsigned char N = opcode & 0x000F;
	unsigned char NN = opcode & 0x00FF;
	unsigned short NNN = opcode & 0x0FFF;
	
	switch(opcode & 0xF000){
		case(0x0000):
			switch(opcode & 0x000F){
				case(0x0000):	// 0x00E0: Clears the screen 
					clearGfx();
					drawFlag = 1;
					pc+=2;
				break;

				case(0x000E):	// 0x00EE: Returns from subroutine
		   			sp--;
					pc = stack[sp];
					pc+=2;
				break;	
			}
		break;

		case(0x1000):	//0x1NNN: Jumps to address NNN
			pc = NNN;
		break;

		case(0x2000):	//0x2NNN: Calls subroutine at NNN
			stack[sp] = pc;
			pc = NNN;
			sp++;
		break;

		case(0x3000):	//0x3XNN: Skips the next instruction if VX equals NN. (Usually the next instruction is a jump to skip a code block)
			if(V[x] == NN){
				pc+=2;
			}
			pc+=2;
		break;
	
		case(0x4000):	//0x4XNN: Skips the next instruction if VX doesn't equal NN. (Usually the next instruction is a jump to skip a code block)
			if(V[x] != NN){
				pc+=2;
			}
			pc+=2;
		break;

		case(0x5000):	//0x5XY0: Skips the next instruction if VX equals VY. (Usually the next instruction is a jump to skip a code block)
			if(V[x] == V[y]){
				pc+=2;
			}
			pc+=2;
		break;

		case(0x6000):	//0x6XNN: Sets VX to NN
			V[x] = NN;
			pc+=2;
		break;

	    case(0x7000):	//0x7XNN: Adds NN to VX
			V[x] += NN;
			pc+=2;
		break;

		case(0x8000):
			switch(opcode & 0x000F){
				case(0x0000):	//0x8XY0: Sets VX to the value of VY
					V[x] = V[y];
					pc+=2;
				break;

				case(0x0001):	//0x8XY1: Sets VX to VX or VY. (Bitwise OR operation)
					V[x] |= V[y];
					pc+=2;
				break;

				case(0x0002):	//0x8XY2: Sets VX to VX and VY. (Bitwise AND operation)
					V[x] &= V[y];
					pc+=2;
				break;

				case(0x0003):	//0x8XY3: Sets VX to VX xor VY.
					V[x] ^= V[y];
					pc+=2;
				break;
		
				case(0x0004):	//0x8XY4: Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
					if(V[y] > (0xFF - V[x]))
					{
						V[0xF] = 1;
					}
					else
					{
						V[0xF] = 0;
					}
					V[x] += V[y];
					pc+=2;
				break;
		
				case(0x0005):	//0x8XY5: VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
					if(V[y] > V[x])
					{
						V[0xF] = 0;
					}
					else
					{
						V[0xF] = 1;
					}
					V[x] -= V[y];
					pc+=2;
				break;

				case(0x0006):	//0x8XY6: 	Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift.
					V[0xF] = V[x] & 0x1;
					V[x] >>= 1;
					pc+=2;
				break;
				
				case(0x0007):	//0x8XY7: Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
					if(V[y] > V[x])
					{
						V[0xF] = 1;
					}
					else
					{
						V[0xF] = 0;
					}
					V[x] = V[y] - V[x];
					pc+=2;
				break;
				
				case(0x000E):	//0x8XYE: Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift.
					V[0xF] = V[x] >> 7;
					V[x] <<= 1;
					pc+=2;
				break;
			}
		break;

		case(0x9000):	//0x9XY0:  Skips the next instruction if VX doesn't equal VY. (Usually the next instruction is a jump to skip a code block) 
			if(V[x] != V[y])
			{
				pc+=2;
			}
			pc+=2;
		break;
	
	 	case(0xA000):	//0xANNN: Sets I to the address NNN.
			I = NNN;
			pc += 2;
		break;
	
		case(0xB000):	//0xBNNN: 	Jumps to the address NNN plus V0.
			pc = NNN + V[0x0];
		break;

		case(0xC000):	//0xCXNN: Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
			V[x] = (rand() % 0xFF) & NN;
			pc+=2;
		break;

		case(0xD000):	//0xDXYN: Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. Each row of 8 pixels is read as 
						//bit-coded starting from memory location I; I value doesn’t change after the execution of this instruction. As described above, VF is 
						//set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn’t happen
						; // Empty statement as case cannot be followed by a character declaration
			unsigned char vx = V[x];
			unsigned char vy = V[y];
			unsigned short height = opcode & 0x000F;
			unsigned short pixel;
				
			V[0xF] = 0;
			for(int yline = 0; yline < height; yline++){
				pixel = memory[I + yline];
				for(int xline = 0; xline < 8; xline++){
					if((pixel & (0x80 >> xline)) != 0){
						if(gfx[vx + xline + ((vy + yline) * 64)] == 1){
							V[0xF] = 1; 
						}                                
						gfx[vx + xline + ((vy + yline) * 64)] ^= 1;
					}
				}
			}

			drawFlag = true;
			pc += 2;
		break;
	
		case(0xE000):
			switch(opcode & 0x000F){
				case(0x000E):	//0xEX9E: Skips the next instruction if the key stored in VX is pressed. (Usually the next instruction is a jump to skip a code block)
					if(key[V[x]] == 1){
						pc+=2;
					}
					pc+=2;
				break;

				case(0x0001):	//0xEXA1: Skips the next instruction if the key stored in VX isn't pressed. (Usually the next instruction is a jump to skip a code block)
					if(key[V[x]] == 0){
						pc+=2;
					}
					pc+=2;
				break;
			}
		break;

		case(0xF000):
			switch(opcode & 0x000F){
				case(0x0007):	//0xFX07: Sets VX to the value of the delay timer.
					V[x] = delay_timer;
					pc+=2;
				break;

				case(0x000A):	//0xFX0A: A key press is awaited, and then stored in VX. (Blocking Operation. All instruction halted until next key event)
					while(true){
						if(inputState(*event)){
							V[x] = getKeys(*event);
							printf("is this being called\n");
							break;
						}
						SDL_PollEvent(event);
					}
				break;

				case(0x0005):	
					switch(opcode & 0x00F0){
						case(0x0010):	//0xFX15: Sets the delay timer to VX.
							delay_timer = V[x];
							pc+=2;
						break;

						case(0x0050):	//0xFX55: Stores V0 to VX (including VX) in memory starting at address I.
							for(int pos = 0; pos <= x; pos++)
							{
								memory[I + pos] = V[pos];
							}
							pc+=2;
						break;
						
						case(0x0060):	//0xFX65: Fills V0 to VX (including VX) with values from memory starting at address I.
							for(int pos = 0; pos <= x; pos++)
							{
								V[pos] = memory[I + pos];
							}
							pc+=2;
						break;
					}
				break;

				case(0x0008):	//0xFX18: Sets the sound timer to VX.
					sound_timer = V[x];
					pc+=2;
				break;

				case(0x000E):	//0xFX1E: Adds VX to I.
					I += V[x];
					pc+=2;
				break;

				case(0x0009):	//0xFX29: Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font.
					I = memory[FONT_MEM_ST + FONT_SIZE*V[x]];
					pc+=2;
				break;

				case(0x0003):	//0xFX33: Stores the binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the 
											//middle digit at I plus 1, and the least significant digit at I plus 2. (In other words, take the decimal representation of VX, 
											//place the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.)
					memory[I] = V[x] / 100;
					memory[I+1] = (V[x] - memory[I] * 100) / 10;
					memory[I+2] = V[x] - memory[I] * 100 - memory[I+1] * 10;
					pc+=2;
				break;
			}
		break;

		default:
			printf ("Unknown opcode [0x0000]: 0x%X\n", opcode);  
		break;
	}

  
	// Update timers, make sure they decrease at 60Hz
	if(delay_timer > 0)
	{ 
		delay_timer--;
	}  

	if(sound_timer > 0)
	{
		if(sound_timer == 1)
		{
		  printf("BEEP!\n");
		}
		sound_timer--;
	}
}