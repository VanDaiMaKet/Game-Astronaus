#include "BaseFunction.h"
#include "BaseObject.h"
#include "MainCharacter.h"
#include "Map.h"

MainCharacter player;
MapGame map;

bool init();
//Loads Background
bool loadBackGround();
//Frees media and shuts down SDL
void close();

bool init()
{
	bool success = true;
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}
		//Create window
		gWindow = SDL_CreateWindow("Game Astronaut", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gScreen = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gScreen == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gScreen, 0xFF, 0xFF, 0xFF, 0xFF);
				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}
	return success;
}

bool loadBackGround() {
	bool load = map.GetMap().LoadBackground("image/gBackGround.png", gScreen);
	if (load == NULL) return false;
	return true;
}

void close()
{
	SDL_DestroyTexture(gBackGround);
	player.Free();

	//Destroy window	
	SDL_DestroyRenderer(gScreen);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gScreen = NULL;
	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}


int main(int argc, char* args[]) {
	if (!init()) printf("Failed to initialize!\n");//Start up SDL and create window
	if (!loadBackGround()) printf("Failed to load media!\n");//Load media

	map.LoadMap("image/maptile.txt");
	map.LoadTileImage(gScreen);
	player.LoadImg("image//player_run_right.png", gScreen);
	player.LoadFrame();
	bool quit = false;
	while (!quit) {
		while (SDL_PollEvent(&gEvent) != 0) {//Handle events on queue
			//User requests quit
			if (gEvent.type == SDL_QUIT) {
				quit = true;
			}
			player.HandleAction(gEvent);
		}
		SDL_SetRenderDrawColor(gScreen, 0xFF, 0xFF, 0xFF, 0xFF);
		//Clear screen
		SDL_RenderClear(gScreen);
		Map mapgame = map.GetMap();
		mapgame.RenderBackground(gBackGround, gScreen);
		player.SetMapXY(mapgame.x_start, mapgame.y_start);
		player.Update(mapgame);
		player.Show(gScreen);

		map.SetMap(mapgame);
		map.RenderMap(gScreen);

		SDL_RenderPresent(gScreen);
	}
	//Free resources and close SDL
	close();
	return 0;
}