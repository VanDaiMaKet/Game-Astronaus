#include "Monster.h"
Monster::Monster() {
	frame = 0;
	x_pos = 0.0;
	y_pos = 0.0;
	x_change = 0.0;
	y_change = 0.0;
	w_frame = 0;
	h_frame = 0;
	onGround = false;
	map_x = 0;
	map_y = 0;
	move_limit_a = 0;
	move_limit_b = 0;
	input_type.left = 1;
	type_move = MONSTER;

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
	if (frame / 50 >= 5) frame = 0;
	SDL_Rect* currentClip = &frame_clip[frame / 50];//nguồn ảnh
	SDL_Rect renderQuad = { rect_.x, rect_.y, w_frame, h_frame };// điểm xuất hiện trên màn hình
	SDL_RenderCopy(des, object_, currentClip, &renderQuad);
}

void Monster::Update(Map& map) {
	x_change = 0;
	y_change += MONSTER_FALLING_SPEED;
	if (y_change >= MAX_MONSTER_FALLING_SPEED) {
		y_change = MAX_MONSTER_FALLING_SPEED;
	}
	if (input_type.left == 1) {
		x_change -= MONSTER_SPEED;
	}
	else if (input_type.right == 1) {
		x_change += MONSTER_SPEED;
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
			}
		}
		else if (x_change < 0) {//lùi
			if (map.tile[y1][x1] == 1 || map.tile[y1][x1] == 2 || map.tile[y1][x1] == 3 || map.tile[y1][x1] == 6 || map.tile[y1][x1] == 7 || map.tile[y2][x1] == 1 || map.tile[y2][x1] == 2 || map.tile[y2][x1] == 3 || map.tile[y2][x1] == 6 || map.tile[y2][x1] == 7) {
				x_pos = (x1 + 1) * TILE_SIZE;
				x_change = 0;
			}
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
		}
		else if (y_change < 0) {
			if (map.tile[y1][x1] == 1 || map.tile[y1][x1] == 2 || map.tile[y1][x1] == 3 || map.tile[y1][x1] == 6 || map.tile[y1][x1] == 7 || map.tile[y1][x2] == 1 || map.tile[y1][x2] == 2 || map.tile[y1][x2] == 3 || map.tile[y1][x2] == 6 || map.tile[y1][x2] == 7) {
				y_pos = (y1 + 1) * TILE_SIZE;
				y_change = 0;
			}
		}
	}
	x_pos += x_change;
	y_pos += y_change;
	if (x_pos < 0) x_pos = 0;
	if (x_pos + w_frame > map.x_max) x_pos = map.x_max - w_frame - 1;
	if (y_pos < 0) y_pos = 0;
	if (y_pos + h_frame >= (map.y_max - 64)) {
		onGround = true;
		y_pos = map.y_max - h_frame - 64;
	}
}

void Monster::HanleMoveType(SDL_Renderer* des ){
	if (type_move == MONSTER) {
		;
	}
	else {
		if (onGround) {
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
		else{
			if (input_type.left == 1) {
				LoadImg("image/alien_fly_left.png", des);
			}
		}
	}

}

