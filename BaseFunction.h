#ifndef BASE_FUNCTION_H_
#define BASE_FUNCTION_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include <vector>
#include <fstream>

using namespace std;

static SDL_Window* gWindow = NULL;
static SDL_Renderer* gScreen = NULL;
static SDL_Event gEvent;
static SDL_Texture* gBackGround = NULL;
static SDL_Rect srcRect = { 0, 0, 0, 0 };

//Window
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;//Bits Per Pixel

const int COLLOR_KEY_R = 255;//transparent white background of image
const int COLLOR_KEY_G = 255;
const int COLLOR_KEY_B = 255;

struct KeyPress {
	int left;
	int right;
	int up;
	int down;
};
//Map
#define TILE_SIZE 64
#define MAX_MAP_WIDTH 80
#define MAX_MAP_HEIGHT 10
#define MAX_TILE_IMAGE 10

typedef struct Map {
	int x_start;//vi tri mep man hinh ben trai so voi ban do full
	int y_start;
	int x_max; //chieu dai toi da cua ban do map
	int y_max;
	int tile[MAX_MAP_HEIGHT][MAX_MAP_WIDTH];// mang cac tile
	string filePath;
	bool LoadBackground(string path, SDL_Renderer* des) {
		SDL_Texture* newTexture = NULL;
		SDL_Surface* loadSurface = IMG_Load(path.c_str());
		if (loadSurface != NULL)
		{
			newTexture = SDL_CreateTextureFromSurface(des, loadSurface);
			if (newTexture != NULL) {
				srcRect.w = loadSurface->w;
				srcRect.h = loadSurface->h;
			}
			SDL_FreeSurface(loadSurface);
		}
		gBackGround = newTexture;
		return gBackGround != NULL;
	}

	void RenderBackground(SDL_Texture* texture, SDL_Renderer* des) {
		srcRect = { x_start, y_start, SCREEN_WIDTH, SCREEN_HEIGHT };
		SDL_Rect desQuad = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
		SDL_RenderCopy(des, texture, &srcRect, &desQuad);
	}
};
#endif
