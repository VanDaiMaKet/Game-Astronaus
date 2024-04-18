#include "Monster.h"
Monster::Monster() {
	frame = 0;
	x_pos = 0.0;
	y_pos = 0.0;
	x_change = 0.0;
	y_change = 0.0;
	w_frame = 0;
	h_frame = 0;
	va_cham_ngang_tien = false;
	va_cham_ngang_lui = false;
	va_cham_tren = false;
	onGround = false;
	map_x = 0;
	map_y = 0;
	move_limit_a = 0;
	move_limit_b = 0;
	input_type.left = 1;
	input_type.right = 0;
	input_type.down = 0;
	input_type.up = 0;
	type_move = STAND_MONSTER;
	healthFlyMonsterMax = 0;
	healthFlyMonster = 0;
}
Monster::~Monster() {
	Free();
}
bool Monster::LoadImg(string path, SDL_Renderer* des) {
	bool load = BaseObject::LoadImg(path, des);
	if (load = true)
	{
		w_frame = rect_.w / 5;
		h_frame = rect_.h;
	}
	return load;
}
void Monster::LoadFrame() {
	if (w_frame > 0 && h_frame > 0)
	{
		for (int i = 0; i < 5; ++i)
		{
			frame_clip[i].x = i * w_frame;
			frame_clip[i].y = 0;
			frame_clip[i].w = w_frame;
			frame_clip[i].h = h_frame;
		}
	}
}
void Monster::Show(SDL_Renderer* des) {
	rect_.x = x_pos - map_x;// vị trí xuất hiện trên màn hình
	rect_.y = y_pos - map_y + 6;// vị trí xuất hiện trên màn hình
	frame++;
	if (frame / 17 >= 5) frame = 0;
	SDL_Rect* currentClip = &frame_clip[frame / 17];//nguồn ảnh
	SDL_Rect renderQuad = { rect_.x, rect_.y, w_frame, h_frame };// điểm xuất hiện trên màn hình
	SDL_RenderCopy(des, object_, currentClip, &renderQuad);
}
void Monster::Update(Map& map) {
	x_change = 0;
	y_change = 0;
	/*y_change += MONSTER_FALLING_SPEED;
	if (y_change >= MAX_MONSTER_FALLING_SPEED) {
		y_change = MAX_MONSTER_FALLING_SPEED;
	}*/
	if (input_type.left == 1) {
		x_change -= MONSTER_SPEED;
	}
	if (input_type.right == 1) {
		x_change += MONSTER_SPEED;
	}
	if (input_type.up == 1) {
		y_change -= MONSTER_SPEED;
		onGround = false;
	}
	if (input_type.down == 1) {
		y_change += MONSTER_SPEED;
	}
	MapCollisionCheck(map);
}

void Monster::MapCollisionCheck(Map& map) {
	int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	// kiểm tra theo chiều ngang
	int heightMin = (h_frame < TILE_SIZE ? h_frame : TILE_SIZE) - 30;
	x1 = (x_pos + x_change) / TILE_SIZE;//tìm nhân vật ở ô thứ bao nhiêu
	x2 = (x_pos + x_change + w_frame - 1) / TILE_SIZE;
	y1 = (y_pos + 5) / TILE_SIZE;
	y2 = (y_pos + heightMin - 1) / TILE_SIZE;
	if (x1 >= 0 && x2 <= MAX_MAP_WIDTH && y1 >= 0 && y2 <= MAX_MAP_HEIGHT) {
		if (x_change > 0) {//tiến
			if (map.tile[y1][x2] == 1 || map.tile[y1][x2] == 2 || map.tile[y1][x2] == 3 || map.tile[y1][x2] == 6 || map.tile[y1][x2] == 7 || map.tile[y2][x2] == 1 || map.tile[y2][x2] == 2 || map.tile[y2][x2] == 3 || map.tile[y2][x2] == 6 || map.tile[y2][x2] == 7) {
				x_pos = x2 * TILE_SIZE;
				x_pos -= w_frame + 1;
				x_change = 0;
				va_cham_ngang_tien = true;
			}
			else va_cham_ngang_tien = false;
		}
		else if (x_change < 0) {//lùi
			if (map.tile[y1][x1] == 1 || map.tile[y1][x1] == 2 || map.tile[y1][x1] == 3 || map.tile[y1][x1] == 6 || map.tile[y1][x1] == 7 || map.tile[y2][x1] == 1 || map.tile[y2][x1] == 2 || map.tile[y2][x1] == 3 || map.tile[y2][x1] == 6 || map.tile[y2][x1] == 7) {
				x_pos = (x1 + 1) * TILE_SIZE;
				x_change = 0;
				va_cham_ngang_lui = true;
			}
			else va_cham_ngang_lui = false;
		}
	}
	//kiểm tra theo chiều dọc
	int widthtMin = (w_frame < TILE_SIZE ? w_frame : TILE_SIZE) - 30;//  lấy chiều rộng bằng chân nhân vật
	x1 = (x_pos + 1) / TILE_SIZE;
	x2 = (x_pos + widthtMin) / TILE_SIZE;
	y1 = (y_pos + y_change) / TILE_SIZE;
	y2 = (y_pos + y_change + h_frame - 1) / TILE_SIZE;
	if (x1 >= 0 && x2 <= MAX_MAP_WIDTH && y1 >= 0 && y2 <= MAX_MAP_HEIGHT) {
		if (y_change > 0) {
			if (map.tile[y2][x1] == 1 || map.tile[y2][x1] == 2 || map.tile[y2][x1] == 3 || map.tile[y2][x1] == 6 || map.tile[y2][x1] == 7 || map.tile[y2][x2] == 1 || map.tile[y2][x2] == 2 || map.tile[y2][x2] == 3 || map.tile[y2][x2] == 6 || map.tile[y2][x2] == 7) {
				y_pos = y2 * TILE_SIZE;
				y_pos -= (h_frame + 1);
				y_change = 0;
				onGround = true;
			}
			else onGround = false;
		}
		else if (y_change < 0) {
			if (map.tile[y1][x1] == 1 || map.tile[y1][x1] == 2 || map.tile[y1][x1] == 3 || map.tile[y1][x1] == 6 || map.tile[y1][x1] == 7 || map.tile[y1][x2] == 1 || map.tile[y1][x2] == 2 || map.tile[y1][x2] == 3 || map.tile[y1][x2] == 6 || map.tile[y1][x2] == 7) {
				y_pos = (y1 + 1) * TILE_SIZE;
				y_change = 0;
				va_cham_tren = true;
			}
			else va_cham_tren = false;
		}
	}
	x_pos += x_change;
	y_pos += y_change;
	if (x_pos < 0) x_pos = 0;
	if (x_pos + w_frame > map.x_max) x_pos = map.x_max - w_frame - 1;
	if (y_pos < 0) y_pos = 0;
}

void Monster::HanleMoveType(SDL_Renderer* des, MainCharacter& Player){
	if (type_move == STAND_MONSTER) {
		;
	}
	else {
		if (x_pos_player <= move_limit_b && x_pos_player >= move_limit_a) {
			if (x_pos_player < x_pos ) {
				input_type.left = 1;
				input_type.right = 0;
				if (y_pos_player < y_pos && va_cham_tren == false) {
					input_type.up = 1;
					input_type.down = 0;
				}
				if ((y_pos_player > y_pos ) && (onGround==false)) {
					input_type.down = 1;
					input_type.up = 0;
				}
				else if ((y_pos_player < y_pos) && (va_cham_ngang_lui == true) && (onGround == true)) {
					input_type.up = 1;
					input_type.down = 0;
				}
				else if ((y_pos_player = y_pos) && (va_cham_ngang_lui == true)) {
					input_type.up = 1;
					input_type.down = 0;
				}
				LoadImg("image/alien_fly_left.png", des);
			}
			else if (x_pos_player > x_pos) {
				input_type.left = 0;
				input_type.right = 1;
				if (y_pos_player < y_pos && va_cham_tren == false) {
					input_type.up = 1;
					input_type.down = 0;
				}
				if ((y_pos_player > y_pos) && (onGround == false)) {
					input_type.down = 1;
					input_type.up = 0;
				}
				else if ((y_pos_player < y_pos) && (va_cham_ngang_tien == true) && (onGround == true)) {
					input_type.up = 1;
					input_type.down = 0;
				}
				else if ((y_pos_player = y_pos) && (va_cham_ngang_tien == true)) {
					input_type.up = 1;
					input_type.down = 0;
				}
				LoadImg("image/alien_fly_right.png", des);
			}
			if ((y_pos_player < y_pos && va_cham_tren == true) || (y_pos_player > y_pos && onGround == true)) {
				if (va_cham_ngang_lui == false) {
					input_type.left = 1;
					input_type.right = 0;
				}
				else {
					input_type.left = 0;
					input_type.right = 1;
				}
				LoadImg("image/alien_fly_left.png", des);
			}
			/*if (x_pos_player - x_pos < 20 && x_pos_player - x_pos > -20 && y_pos_player - y_pos < 20 && y_pos_player - y_pos > -20)*/ 
			if (((x_pos_player - x_pos < 50 && x_pos_player - x_pos > 0 )||(x_pos - x_pos_player < 50 && x_pos - x_pos_player > 0))&& y_pos_player - y_pos < 20 && y_pos_player - y_pos > -20)
			{
				input_type.left = 0;
				input_type.right = 0;
				input_type.up = 0;
				input_type.right = 0;
				Player.healthPlayer -= DAM_FLY_MONSTER;
				LoadImg("image/alien_fly_left.png", des);
			}
		}
		if (x_pos > move_limit_b) {
			input_type.left = 1;
			input_type.right = 0;
			LoadImg("image/alien_fly_left.png", des);
		}
		else if (x_pos < move_limit_a) {
			input_type.left = 0;
			input_type.right = 1;
			LoadImg("image/alien_fly_right.png", des);
		}
	}

}

bool Monster::LoadHealFlyMonster(SDL_Renderer* des) {
	return healFlyMonster.LoadImg(pathHealthFlyMonster, des);
}
void Monster::HandHealFlyMonster(SDL_Renderer* des) {
	int w_heal = (healthFlyMonster / healthFlyMonsterMax) * 100;
	SDL_Rect src = { 0,0, w_heal , healFlyMonster.GetRect().h };
	SDL_Rect desrect = { rect_.x - 20,rect_.y - 20,src.w,src.h };
	SDL_RenderCopy(des, healFlyMonster.GetObject(), &src, &desrect);
}