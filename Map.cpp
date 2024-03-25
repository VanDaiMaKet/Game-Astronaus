#include "Map.h"

void MapGame::LoadMap(const char* filetext) {
	FILE* file = NULL;
	fopen_s(&file, filetext, "rb");
	if (file == NULL) {
		cout << "Error!can't open file";
		return;
	}
	map_.x_max = 0;
	map_.y_start = 0;
	for (int i = 0; i < MAX_MAP_HEIGHT; i++) {
		for (int j = 0; j < MAX_MAP_WIDTH; j++) {
			fscanf_s(file, "%d", &map_.tile[i][j]);
			int value = map_.tile[i][j];
			if (value > 0) {
				if (j > map_.x_max) map_.x_max = j;
				if (i > map_.y_max) map_.y_max = i;
			}
		}
	}
	map_.x_max = (map_.x_max + 1) * TILE_SIZE;
	map_.y_max = (map_.y_max + 1) * TILE_SIZE;
	map_.x_start = 0;
	map_.y_start = 0;
	map_.filePath = filetext;
	fclose(file);
}
void MapGame::LoadTileImage(SDL_Renderer* screen) {
	char fileimg[30];
	FILE* file = NULL;
	for (int i = 0; i < MAX_TILE_IMAGE; i++) {
		sprintf_s(fileimg, "image/%d.png", i);
		fopen_s(&file, fileimg, "rb");
		if (file == NULL) continue;
		fclose(file);
		tileMap_[i].LoadImg(fileimg, screen);
	}
}
void MapGame::RenderMap(SDL_Renderer* screen) {
	int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	int map_x = 0, map_y = 0;
	map_x = map_.x_start / TILE_SIZE;
	map_y = map_.y_start / TILE_SIZE;
	x1 = (map_.x_start % TILE_SIZE) * -1;
	x2 = x1 + SCREEN_WIDTH + (x1 == 0 ? 0 : TILE_SIZE);
	y1 = (map_.y_start % TILE_SIZE) * -1;
	y2 = y1 + SCREEN_HEIGHT + (y1 == 0 ? 0 : TILE_SIZE);
	for (int i = y1; i < y2; i += TILE_SIZE) {
		map_x = map_.x_start / TILE_SIZE;
		for (int j = x1; j < x2; j += TILE_SIZE) {
			int value = map_.tile[map_y][map_x];
			if (value > 0) {
				tileMap_[value].SetRect(j, i);
				tileMap_[value].Render(screen);
			}
			map_x++;
		}
		map_y++;
	}
}
/*
void MapGame::LoadMap(string filetext) {
	map_.x_max = 0;
	map_.y_max = 0;
	map_.x_start = 0;
	map_.y_start = 0;
	ifstream file(filetext);
	if (!file.is_open()) {
		cout << "Error to open file txt\n";
		return;
	}
	else cout << "file can open\n";
	for (int i = 0; i < MAX_MAP_HEIGHT; i++) {
		for (int j = 0; j < MAX_MAP_WIDTH; j++) {
			file >> map_.tile[i][j];
			int n = map_.tile[i][j];
			if (n > 0) {
				if (i >= map_.y_max) map_.y_max = i;
				if (j >= map_.x_max) map_.x_max = j;
			}
		}
	}
	map_.x_max = (map_.x_max + 1) * TILE_SIZE;
	map_.y_max = (map_.y_max + 1) * TILE_SIZE;
	map_.filePath = filetext;
	file.close();
}
void MapGame::LoadTileImage(SDL_Renderer* des) {
	string file;
	for (int i = 0; i < MAX_TILE_IMAGE; i++) {
		string num = to_string(i);
		file = "image//" + num + ".png";
		tileMap_[i].LoadImg(file, des);
	}
}
void MapGame::RenderMap(SDL_Renderer* des) {
	int x1 = 0;
	int x2 = 0;
	int y1 = 0;
	int y2 = 0;
	int map_x = 0;
	int map_y = 0;

	x1 = (map_.x_start % TILE_SIZE) * -1;
	x2 = x1 + SCREEN_WIDTH + (x1 == 0 ? 0 : TILE_SIZE);

	y1 = (map_.y_start % TILE_SIZE) * -1;
	y2 = y1 + SCREEN_HEIGHT + (y1 == 0 ? 0 : TILE_SIZE);

	map_x = map_.x_start / TILE_SIZE;
	map_y = map_.y_start / TILE_SIZE;
	for (int i = y1; i < y2; i += TILE_SIZE) {
		for (int j = x1; j < x2; j += TILE_SIZE) {
			int val = map_.tile[map_y][map_x];
			if (val > 0) {
				tileMap_[val].SetRect(j, i);
				tileMap_[val].Render(des, NULL);
			}
			map_x++;
		}
		map_y++;
	}
}
*/