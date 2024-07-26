#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#define STDLIB_H
#define TIME_H

// Modify this macro to change the input filename
#define FILE_NAME "test_opcode.ch8"

#include <stdio.h>
#include "chiplib.h"

char buffer[MAX_ROM_SIZE];
void read_rom(char const* filename)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		printf("Error: failed to open file '%s',\n", filename);
		exit(EXIT_FAILURE);
	}

	int i, c;
	while ((c = fgetc(file)) != EOF)
	{
		buffer[i] = c;
		++i;
	}
	fclose(file);
}

#define WINDOW_WIDTH 	640
#define WINDOW_HEIGHT 	480

#define DISPLAY_AREA_OFFSET 32
#define DISPLAY_AREA_WIDTH 64
#define DISPLAY_AREA_HEIGHT 32
#define DISPLAY_AREA_SIZE 8


int WinMain()
{
        struct Chip8Machine Machine = {0};		// Instantiate the machine

	SDL_Event event;
	SDL_Renderer *renderer;
	SDL_Window *window;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC, &window, &renderer);

	read_rom(FILE_NAME);
	init_machine(&Machine, buffer, sizeof buffer);
	for (int i = 0; i < 0x200; ++i)
	{
		printf("%d ", Machine.Memory.map[i]);
	}

	printf("\nPC: %x\n", Machine.Registers.PC);
	printf("%x\n", sizeof(Machine.display));
	printf("%x\n", Machine.opcode);

	SDL_Rect rectangles[64][32];

    	for (int i = 0; i < DISPLAY_AREA_WIDTH; i++)
    	{
        	for (int j = 0; j < DISPLAY_AREA_HEIGHT; j++)
        	{
            		SDL_Rect rect = {DISPLAY_AREA_OFFSET + i * DISPLAY_AREA_SIZE, DISPLAY_AREA_OFFSET + j * DISPLAY_AREA_SIZE, DISPLAY_AREA_SIZE, DISPLAY_AREA_SIZE};
            		rectangles[i][j] = rect;
        	}
    	}


	while (1)
	{
		SDL_PumpEvents();
		for (int i = 0; i < 64; ++i)
		{
			for (int j = 0; j < 32; ++j)
			{
				if (Machine.display[(j * 64) + i])
				{
		                    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, SDL_ALPHA_OPAQUE);
				}
				else
				{
		                    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
				}

				SDL_RenderFillRect(renderer, &rectangles[i][j]);
				// printf("%c", Machine.display[(j * 64) + i] - 128);
			}
		}
		do_cycle(&Machine);
		SDL_RenderPresent(renderer);
	}
	return EXIT_SUCCESS;
};
