#include "BaseFunction.h"
#include "BaseObject.h"
#include "MainCharacter.h"
#include "Map.h"
#include "Bullet.h"
#include "Monster.h"
#include "Time.h"

TTF_Font* gFont = NULL;


vector <Monster*> CreateMonsterList() {
	vector <Monster*> monsterList;
	Monster* monsters = new Monster[30];
	for (int i = 0; i < 30; i++) {
		Monster* onemonster = (monsters + i);
		if (onemonster != NULL) {
			onemonster->LoadImg("image/alien_fly_left.png", gScreen);
			onemonster->pathHealthFlyMonster = "image/health_monster.png";
			onemonster->healthFlyMonsterMax = 60.0;
			onemonster->healthFlyMonster = onemonster->healthFlyMonsterMax;
			onemonster->LoadHealFlyMonster(gScreen);
			onemonster->LoadFrame();
			onemonster->SetTypeMove(Monster::F_LEFT);
			onemonster->SetXpos(400+i*150);
			onemonster->SetYpos(448-(rand()%7)*64);
			int pos_a = onemonster->GetXpos() - 300;
			int pos_b = onemonster->GetXpos() + 300;
			onemonster->SetMovePos(pos_a, pos_b);
			monsterList.push_back(onemonster);
		}
	}
	return monsterList;
}

bool init();
void close();

bool BaseObject::loadFromRenderedText(string textureText, SDL_Color textColor, TTF_Font* font) {
	Free();
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if (textSurface != NULL)
	{
		object_ = SDL_CreateTextureFromSurface(gScreen, textSurface);
		if (object_ == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			rect_.w = textSurface->w;
			rect_.h = textSurface->h;
		}
		SDL_FreeSurface(textSurface);
	}
	else
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	return object_ != NULL;
}


int ShowMenu(SDL_Renderer* des, TTF_Font* font) {
	BaseObject Menu;
	BaseObject textmenu[2];
	Menu.LoadImg("image/menu.png",des);
	if (Menu.GetObject() == NULL) return 1;
	Menu.Render(des, NULL);
	textmenu[0].SetRect(170, 200);
	textmenu[1].SetRect(170, 260);
	textmenu[0].loadFromRenderedText("START", whileblue,  font);
	textmenu[1].loadFromRenderedText("EXIT", whileblue,  font);
	textmenu[0].Render(des, NULL);
	textmenu[1].Render(des,NULL);
	bool selected[2] = { false,false };
	SDL_Event e;
	int xm = 0;
	int ym = 0;
	bool q = false;
	while (q == false) {
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case SDL_QUIT:
				return 1;
				break;
			case SDL_MOUSEMOTION:
			{
				xm = e.motion.x;
				ym = e.motion.y;
				for (int i = 0; i < 2; i++) {
					if (xm >= textmenu[i].GetRect().x && xm <= textmenu[i].GetRect().x + textmenu[i].GetRect().w && ym >= textmenu[i].GetRect().y && ym <= textmenu[i].GetRect().y + textmenu[i].GetRect().h) {
						if (selected[i] == false) {
							selected[i] = true;
							if (i==0)textmenu[0].loadFromRenderedText("START", pink,font);
							if (i==1)textmenu[1].loadFromRenderedText("EXIT", pink, font);
						}
					}
					else {
						if (selected[i] == true) {
							selected[i] == false;
							textmenu[i].Free();
							if (i == 0)textmenu[0].loadFromRenderedText("START", whileblue, font);
							if (i == 1)textmenu[1].loadFromRenderedText("EXIT", whileblue, font);
						}
					}
				}
			}
			break;
			case SDL_MOUSEBUTTONDOWN:
			{
				if (e.button.button == SDL_BUTTON_LEFT) {
					xm = e.button.x;
					ym = e.button.y;
					for (int i = 0; i < 2; i++) {
						if (xm >= textmenu[i].GetRect().x && xm <= textmenu[i].GetRect().x + textmenu[i].GetRect().w && ym >= textmenu[i].GetRect().y && ym <= textmenu[i].GetRect().y + textmenu[i].GetRect().h) {
							textmenu[i].Free();
							Menu.Free();
							return i;
						}
					}
				}
			}
			break;
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_ESCAPE) {
					return 1;
				}
			default:
				break;
			}
			
		}
		SDL_SetRenderDrawColor(gScreen, 0xFF, 0xFF, 0xFF, 0xFF);
		//Clear screen
		SDL_RenderClear(gScreen);
		Menu.Render(gScreen, NULL);
		textmenu[0].Render(gScreen, NULL);
		textmenu[1].Render(gScreen, NULL);
		SDL_RenderPresent(gScreen);

	}
	return 1;
}




bool init()
{
	bool success = true;
	//Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
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
 
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}


				if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
					success = false;
				}
				
				gSoundBullet = Mix_LoadWAV("sound/bullet.wav");
				gSoundHit = Mix_LoadWAV("sound/hit.wav");
				gFont = TTF_OpenFont("image/SuperCorn-X3xGg.ttf",36);
				if (gFont == NULL)
				{
					printf("Failed to load gFont! SDL_ttf Error: %s\n", TTF_GetError());
				}
			}
		}
	}
	return success;
}


void close()
{
	SDL_DestroyRenderer(gScreen);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gScreen = NULL;
	Mix_FreeChunk(gSoundBullet);
	Mix_FreeChunk(gSoundHit);
	gSoundHit = NULL;
	gSoundBullet = NULL;
	TTF_CloseFont(gFont);
	gFont = NULL;
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}


int main(int argc, char* args[]) {
	if (!init()) printf("Failed to initialize!\n");//Start up SDL and create window

	bool quit = false;
	bool thoat_ra_menu = false;
	int statemenu = ShowMenu(gScreen, gFont);
	if (statemenu == 1) {
		quit = true;
	}

	while (!quit) {

		MainCharacter player;
		MapGame map;
		BaseObject gTime;
		BaseObject gFMDie;
		map.GetMap().LoadBackground("image/gBackGround.png", gScreen);
		Timer timer;
		stringstream timeText;
		gTime.SetRect(1040, 4);
		gFMDie.SetRect(850, 4);
		string s;
		map.LoadMap("image/maptile.txt");
		map.LoadTileImage(gScreen);

		player.LoadImg("image//player_run_right.png", gScreen);
		player.LoadFrame();
		player.pathHealthPlayer = "image//health_player.png";
		player.healthPlayerMax = 100.0;
		player.healthPlayer = player.healthPlayerMax;
		player.LoadHealPlayer(gScreen);

		vector<Monster*> monsterList = CreateMonsterList();
		while (thoat_ra_menu == false && quit==false) {
			int mfdie = 0;
			statemenu = 2;
			SDL_RenderClear(gScreen);
			while (SDL_PollEvent(&gEvent) != 0) {//Handle events on queue
				//User requests quit
				if (gEvent.type == SDL_QUIT) {
					quit = true;
				}
				if (statemenu == 2) {
					if (!timer.isStarted()) timer.start();
				}
				player.HandleAction(gEvent, gScreen, gSoundBullet);
			}

			SDL_SetRenderDrawColor(gScreen, 0xFF, 0xFF, 0xFF, 0xFF);
			//Clear screen
			Map mapgame = map.GetMap();
			mapgame.RenderBackground(gBackGround, gScreen);
			player.SetMapXY(mapgame.x_start, mapgame.y_start);
			player.Update(mapgame);
			player.Show(gScreen);
			for (int i = 0; i < monsterList.size(); i++) {
				Monster* onemonster = monsterList.at(i);
				if (onemonster != NULL) {
					if (onemonster->healthFlyMonster > 0) {
						onemonster->SetXYPosPlayer(player.GetXPosPlayer(), player.GetYPosPlayer());
						onemonster->SetMapXY(mapgame.x_start, mapgame.y_start);
						onemonster->HanleMoveType(gScreen, player);
						onemonster->Update(mapgame);
						onemonster->HandHealFlyMonster(gScreen);
						onemonster->Show(gScreen);
					}
					else if (onemonster->healthFlyMonster <= 0) {
						mfdie++;
						onemonster->Free();
					}
				}
			}

			map.SetMap(mapgame);
			map.RenderMap(gScreen);
			player.HandleBulletList(gScreen, mapgame, monsterList, gSoundHit);
			player.HandHealPlayer(gScreen);
			timeText.str("");
			timeText << "Time : " << (timer.getTicks() / 1000.f);
			if (!gTime.loadFromRenderedText(timeText.str().c_str(), violet, gFont))
			{
				printf("Unable to render time texture!\n");
			}

			gTime.Render(gScreen, NULL);
			s = "KILLED: " + to_string(mfdie);
			gFMDie.loadFromRenderedText(s, violet, gFont);
			gFMDie.Render(gScreen, NULL);

			SDL_RenderPresent(gScreen);
			if (player.healthPlayer <= 0) {
				gTime.loadFromRenderedText("GAME OVER", red, gFont);
				gTime.SetRect(400, 300);
				gTime.Render(gScreen, NULL);
				SDL_RenderPresent(gScreen);
				SDL_Delay(3000);
				thoat_ra_menu = true;
				quit = true;
			}
			if (mfdie == 30 || (player.GetXPosPlayer() > 4383 && player.GetXPosPlayer() < 4480 && player.GetYPosPlayer() > 384)) {
				gTime.loadFromRenderedText("YOU WIN", red, gFont);
				gTime.SetRect(400, 300);
				gTime.Render(gScreen, NULL);
				SDL_RenderPresent(gScreen);
				SDL_Delay(3000);
				thoat_ra_menu = true;
				quit = true;

			}
			
			/*if (thoat_ra_menu == true) {
				
				int m = ShowMenu(gScreen, gFont);
				if (m == 1) {
					quit = true;
					thoat_ra_menu == true;
				}
				else {
					player.Free();
					gTime.Free();
					player.healPlayer.Free();
					gFMDie.Free();
					for (int i = 0; i < monsterList.size(); i++) {
						Monster* onemonster = monsterList.at(i);
						if (onemonster != NULL) {
							onemonster->Free();
						}
					}
					quit = false;
					thoat_ra_menu == false;

				}
			}*/
			if (quit == true) break;
		}

	}

	close();
	return 0;
}