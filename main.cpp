#include "BaseFunction.h"
#include "BaseObject.h"
#include "MainCharacter.h"
#include "Map.h"
#include "Bullet.h"
#include "Monster.h"
#include "Time.h"

TTF_Font* gFont = NULL;
TTF_Font* ketqua = NULL;

vector <Monster*> CreateMonsterList(int soluongquai, double mau) {
	vector <Monster*> monsterList;
	Monster* monsters = new Monster[soluongquai];
	for (int i = 0; i < soluongquai; i++) {
		Monster* onemonster = (monsters + i);
		if (onemonster != NULL) {
			onemonster->LoadImg("image/alien_fly_left.png", gScreen);
			onemonster->pathHealthFlyMonster = "image/health_monster.png";
			onemonster->healthFlyMonsterMax = mau;
			onemonster->healthFlyMonster = onemonster->healthFlyMonsterMax;
			onemonster->LoadHealFlyMonster(gScreen);
			onemonster->LoadFrame();
			onemonster->SetTypeMove(Monster::F_LEFT);
			onemonster->SetXpos(400+i*(4700/soluongquai));
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
	textmenu[0].loadFromRenderedText("NEW GAME", whileblue,  font);
	textmenu[1].loadFromRenderedText("QUIT", whileblue,  font);
	
	int xm = 0;
	int ym = 0;
	bool selected[2] = { false,false };
	while (1) {
		while (SDL_PollEvent(&gEvent) != 0) {
			switch (gEvent.type) {
			case SDL_QUIT:
				return 1;
				break;
			case SDL_MOUSEMOTION:
				xm = gEvent.motion.x;
				ym = gEvent.motion.y;
				for (int i = 0; i < 2; i++) {
					if (xm >= textmenu[i].GetRect().x && xm <= textmenu[i].GetRect().x + textmenu[i].GetRect().w && ym >= textmenu[i].GetRect().y && ym <= textmenu[i].GetRect().y + textmenu[i].GetRect().h) {
						if (selected[i] == false) {
							selected[i] = true;
							if (i==0)textmenu[0].loadFromRenderedText("NEW GAME", pink,font);
							if (i==1)textmenu[1].loadFromRenderedText("QUIT", pink, font);
						}
					}
					else {
						if (selected[i] == true) {
							selected[i] == false;
							textmenu[i].Free();
							if (i == 0)textmenu[0].loadFromRenderedText("NEW GAME", whileblue, font);
							if (i == 1)textmenu[1].loadFromRenderedText("QUIT", whileblue, font);
						}
					}
				}
			break;
			case SDL_MOUSEBUTTONDOWN:
			{
				if (gEvent.button.button == SDL_BUTTON_LEFT) {
					xm = gEvent.button.x;
					ym = gEvent.button.y;
					for (int i = 0; i < 2; i++) {
						if (xm >= textmenu[i].GetRect().x && xm <= textmenu[i].GetRect().x + textmenu[i].GetRect().w && ym >= textmenu[i].GetRect().y && ym <= textmenu[i].GetRect().y + textmenu[i].GetRect().h) {
							textmenu[0].Free();
							textmenu[1].Free();
							Menu.Free();
							return i;
						}
					}
				}
			}
			break;
			case SDL_KEYDOWN:
				if (gEvent.key.keysym.sym == SDLK_ESCAPE) {
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
int ShowLevel (SDL_Renderer* des, TTF_Font* font) {
	BaseObject Menu;
	BaseObject textmenu[2];
	Menu.LoadImg("image/menu.png", des);
	if (Menu.GetObject() == NULL) return 1;
	Menu.Render(des, NULL);
	textmenu[0].SetRect(170, 200);
	textmenu[1].SetRect(170, 260);
	textmenu[0].loadFromRenderedText("EASY", whileblue, font);
	textmenu[1].loadFromRenderedText("HARD", whileblue, font);
	int xm = 0;
	int ym = 0;
	bool selected[2] = { false,false };
	while (1) {
		while (SDL_PollEvent(&gEvent) != 0) {
			switch (gEvent.type) {
			case SDL_QUIT:
				return 1;
				break;
			case SDL_MOUSEMOTION:
				xm = gEvent.motion.x;
				ym = gEvent.motion.y;
				for (int i = 0; i < 2; i++) {
					if (xm >= textmenu[i].GetRect().x && xm <= textmenu[i].GetRect().x + textmenu[i].GetRect().w && ym >= textmenu[i].GetRect().y && ym <= textmenu[i].GetRect().y + textmenu[i].GetRect().h) {
						if (selected[i] == false) {
							selected[i] = true;
							if (i == 0)textmenu[0].loadFromRenderedText("EASY", pink, font);
							if (i == 1)textmenu[1].loadFromRenderedText("HARD", pink, font);
						}
					}
					else {
						if (selected[i] == true) {
							selected[i] == false;
							textmenu[i].Free();
							if (i == 0)textmenu[0].loadFromRenderedText("EASY", whileblue, font);
							if (i == 1)textmenu[1].loadFromRenderedText("HARD", whileblue, font);
						}
					}
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
			{
				if (gEvent.button.button == SDL_BUTTON_LEFT) {
					xm = gEvent.button.x;
					ym = gEvent.button.y;
					for (int i = 0; i < 2; i++) {
						if (xm >= textmenu[i].GetRect().x && xm <= textmenu[i].GetRect().x + textmenu[i].GetRect().w && ym >= textmenu[i].GetRect().y && ym <= textmenu[i].GetRect().y + textmenu[i].GetRect().h) {
							textmenu[0].Free();
							textmenu[1].Free();
							Menu.Free();
							return i;
						}
					}
				}
			}
			break;
			case SDL_KEYDOWN:
				if (gEvent.key.keysym.sym == SDLK_ESCAPE) {
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
int ShowPause(SDL_Renderer* des, TTF_Font* font) {
	BaseObject Menu;
	BaseObject textmenu[3];
	Menu.LoadImg("image/pause.png", des);
	Menu.SetRect(460, 100);
	if (Menu.GetObject() == NULL) return 1;
	Menu.Render(des, NULL);
	textmenu[0].SetRect(520, 250);
	textmenu[1].SetRect(520, 320);
	textmenu[2].SetRect(520, 390);
	textmenu[0].loadFromRenderedText("CONTINUE...", pink, font);
	textmenu[1].loadFromRenderedText("EXIT TO MAINMENU", pink, font);
	textmenu[2].loadFromRenderedText("QUIT", pink, font);;
	int xm = 0;
	int ym = 0;
	while (1) {
		while (SDL_PollEvent(&gEvent) != 0) {
			switch (gEvent.type) {
			case SDL_QUIT:
				return 2;
				break;
			break;
			case SDL_MOUSEBUTTONDOWN:
			{
				if (gEvent.button.button == SDL_BUTTON_LEFT) {
					xm = gEvent.button.x;
					ym = gEvent.button.y;
					for (int i = 0; i < 3; i++) {
						if (xm >= textmenu[i].GetRect().x && xm <= textmenu[i].GetRect().x + textmenu[i].GetRect().w && ym >= textmenu[i].GetRect().y && ym <= textmenu[i].GetRect().y + textmenu[i].GetRect().h) {
							textmenu[0].Free();
							textmenu[1].Free();
							textmenu[2].Free();
							Menu.Free();
							return i;
						}
					}
				}
			}
			break;
			case SDL_KEYDOWN:
				if (gEvent.key.keysym.sym == SDLK_ESCAPE) {
					return 2;
				}
			default:
				break;
			}

		}
		SDL_SetRenderDrawColor(gScreen, 0xFF, 0xFF, 0xFF, 0xFF);
		//Clear screen
		/*SDL_RenderClear(gScreen);*/
		Menu.Render(gScreen, NULL);
		textmenu[0].Render(gScreen, NULL);
		textmenu[1].Render(gScreen, NULL);
		textmenu[2].Render(gScreen, NULL);
		SDL_RenderPresent(gScreen);

	}
	return 2;
}
int ShowKetqua(SDL_Renderer* des, TTF_Font* font, bool win, bool lose) {
	BaseObject Menu;
	BaseObject textmenu[3];
	if (win == true) Menu.LoadImg("image/win.png", des);
	else if (lose == true) Menu.LoadImg("image/lo.png", des);
	Menu.SetRect(460, 100);
	if (Menu.GetObject() == NULL) return 1;
	Menu.Render(des, NULL);
	textmenu[0].SetRect(520, 300);
	textmenu[1].SetRect(520, 360);
	textmenu[2].SetRect(520, 420);
	textmenu[0].loadFromRenderedText("NEW GAME", pink, font);
	textmenu[1].loadFromRenderedText("EXIT TO MAINMENU", pink, font);
	textmenu[2].loadFromRenderedText("QUIT", pink, font);;
	int xm = 0;
	int ym = 0;
	while (1) {
		while (SDL_PollEvent(&gEvent) != 0) {
			switch (gEvent.type) {
			case SDL_QUIT:
				return 2;
				break;
				break;
			case SDL_MOUSEBUTTONDOWN:
			{
				if (gEvent.button.button == SDL_BUTTON_LEFT) {
					xm = gEvent.button.x;
					ym = gEvent.button.y;
					for (int i = 0; i < 3; i++) {
						if (xm >= textmenu[i].GetRect().x && xm <= textmenu[i].GetRect().x + textmenu[i].GetRect().w && ym >= textmenu[i].GetRect().y && ym <= textmenu[i].GetRect().y + textmenu[i].GetRect().h) {
							textmenu[0].Free();
							textmenu[1].Free();
							textmenu[2].Free();
							Menu.Free();
							return i;
						}
					}
				}
			}
			break;
			case SDL_KEYDOWN:
				if (gEvent.key.keysym.sym == SDLK_ESCAPE) {
					return 2;
				}
			default:
				break;
			}

		}
		SDL_SetRenderDrawColor(gScreen, 0xFF, 0xFF, 0xFF, 0xFF);
		//Clear screen
		/*SDL_RenderClear(gScreen);*/
		Menu.Render(gScreen, NULL);
		textmenu[0].Render(gScreen, NULL);
		textmenu[1].Render(gScreen, NULL);
		textmenu[2].Render(gScreen, NULL);
		SDL_RenderPresent(gScreen);

	}
	return 2;
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

	while (!quit) {
		int soluongquaibay = 30;
		double mauquaivat = 50.0;
		int statemenu = ShowMenu(gScreen, gFont);
		if (statemenu == 1) {
			quit = true;
		}
		else if (statemenu == 0) {
			int l = ShowLevel(gScreen, gFont);
			if (l == 0) {
				soluongquaibay = 30;
				mauquaivat = 50.0;
			}
			else if (l == 1) {
				soluongquaibay = 45;
				mauquaivat = 60.0;
			}
		}

	newgame:

		bool isWin = false;
		bool isLo = false;
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
		player.healthPlayerMax = 150.0;
		player.healthPlayer = player.healthPlayerMax;
		player.LoadHealPlayer(gScreen);
		
		vector<Monster*> monsterList = CreateMonsterList(soluongquaibay, mauquaivat);
		while (quit==false) {
			int mfdie = 0;
			statemenu = 2;
			bool isPause = false;
			while (SDL_PollEvent(&gEvent) != 0) {
				if (gEvent.type == SDL_QUIT) {
					quit = true;
				}
				if (statemenu == 2) {
					if (!timer.isStarted()) timer.start();
				}
				if (isPause == false) {
					if (gEvent.type == SDL_KEYDOWN)
					{
						if (gEvent.key.keysym.sym == SDLK_p) isPause = true;
					}
				}
				player.HandleAction(gEvent, gScreen, gSoundBullet);

			}
			
			SDL_SetRenderDrawColor(gScreen, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(gScreen);
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

			if (player.healthPlayer <= 0) {
				isLo = true;
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
			}
			if (mfdie == soluongquaibay || (player.GetXPosPlayer() > 4383 && player.GetXPosPlayer() < 4480 && player.GetYPosPlayer() > 384)) {
				isWin = true;
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
			}
			SDL_RenderPresent(gScreen);
			if (isPause == true) {
				int m = ShowPause(gScreen, gFont);
				if (m == 0) {
					isPause = false;
				}
				else if (m == 1) {
					break;
				}
				else if (m == 2) {
					quit = true;
					break;
				}
			}
			if (isWin == true || isLo == true) {
				int n = ShowKetqua(gScreen, gFont, isWin, isLo);
				if (n == 2) {
					quit = true;
					break;
				}
				else if (n == 1) {
					break;
				}
				else if (n == 0) {
					goto newgame;
				}
			}



		}


	}



	close();
	return 0;
}