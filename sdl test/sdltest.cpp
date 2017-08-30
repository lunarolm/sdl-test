// sdltest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <time.h>
#include <windows.h>
#include <math.h>
#include <iostream>
#include <string>
#include <SDL.h>
using namespace std;

//constant declarations
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

//errorcode variable
int errorCode = 0;

//sdl variables
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;

Uint32* buffer = new Uint32[SCREEN_WIDTH*SCREEN_HEIGHT];//reserves memory for the pixel colours, with each pixel on the screen requiring 32 bits of information.

//functions
bool init() {
	bool success = true;

	//initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	else {
		//window init
		window = SDL_CreateWindow("Particle Fire Test", 50, 50, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if (window == NULL) {
			cout << "WINDOW FAILED TO INITIALIZE" << SDL_GetError() << endl;

			success = false;
		}
	}

	return success;
}

bool initTexture() {
	bool success = true;

	//initialize the renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

	if (renderer == NULL) {
		cout << "RENDERER FAILED TO INITIALIZE: " << SDL_GetError() << endl;
		success = false;
	}

	//initalise the texture
	texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA8888,//Represents each pixel as 4 bytes, with one byte for each colour and one for alpha. That is to say, each colour can have 255 shades
		SDL_TEXTUREACCESS_STATIC,//writes numeric values directly to the texture every iteration
		SCREEN_WIDTH,
		SCREEN_WIDTH
	);

	if (texture == NULL) {
		cout << "TEXTURE FAILED TO INITIALIZE" << SDL_GetError() << endl;
		success = false;
	}

	return success;

}

void quit() {
	if (renderer != NULL) {
		SDL_DestroyRenderer(renderer);
		printf("Destroyed Renderer\n");
	}
	if (texture != NULL) {
		SDL_DestroyTexture(texture);
		printf("Destroyed Texture\n");
	}
	if (window != NULL) {
		SDL_DestroyWindow(window);
		printf("Destroyed Window\n");
	}
	SDL_Quit();
}

//updates the screen
void update(Uint32* buffer) {
	SDL_UpdateTexture(texture, NULL, buffer, SCREEN_HEIGHT * sizeof(Uint32));
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue) {
	Uint32 color = 0;

	color += red;
	color <<= 8;
	color += green;
	color <<= 8;
	color += blue;
	color <<= 8;
	color += 0xFF;

	buffer[(y * SCREEN_WIDTH) + x] = color;
}

/******************************************************************************MAIN*********************************************************************************/
/*******************************************************************************************************************************************************************/
int main(int argc, char* argv[]) {

	//initialising window and SDL
	if (!init()) {
		return 1;
	}

	//initialize renderer and texture
	if (!initTexture()) {
		quit();
		return 2;
	}

	//Uint32* buffer = new Uint32[SCREEN_WIDTH*SCREEN_HEIGHT];//reserves memory for the pixel colours, with each pixel on the screen requiring 32 bits of information.
	memset(buffer, 0x00, SCREEN_WIDTH*SCREEN_HEIGHT * 4);

	for (int y = 0; y < SCREEN_HEIGHT; y++) {
		for (int x = 0; x < SCREEN_WIDTH; x++) {
			setPixel(x, y, 0xFF, 0, 0xFF);
		}
	}

	//setPixel(400, 300, 0xFF, 0xFF, 0xFF);

	update(buffer);

	bool quitLoop = false;
	SDL_Event event;

	//main loop
	while (!quitLoop) {

		int elapsed = SDL_GetTicks();
		double red = (1 + sin(elapsed * 0.001)) * 128;
		double green = (1 + sin(elapsed * 0.002)) * 128;
		double blue = (1 + sin(elapsed * 0.003)) * 128;

		for (int y = 0; y < SCREEN_HEIGHT; y++) {
			for (int x = 0; x < SCREEN_WIDTH; x++) {
				setPixel(x, y, (Uint8)floor(red), (Uint8)floor(green), (Uint8)floor(blue));
			}
		}

		//updates the screen
		update(buffer);

		while (SDL_PollEvent(&event)) {	//checks to see if the event queue has any more events, stops if it doesn't
										//also, it changes the event variable to be the next event in the queue

			if (event.type == SDL_QUIT) {
				quitLoop = true;
			}
		}
	}

	quit();

	delete [] buffer;

	return 0;
}
