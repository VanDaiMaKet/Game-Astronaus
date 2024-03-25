#include "MainCharacter.h"

MainCharacter::MainCharacter()
{
	frame_ = 0;
	x_pos_ = 0;
	y_pos_ = 0;
	x_change_ = 0;
	y_change_ = 0;
	w_frame_ = 0;
	h_frame_ = 0;
	status_ = RIGHT;
	input_type_.left = 0;
	input_type_.right = 0;
	input_type_.up = 0;
	input_type_.down = 0;
	onGround = false;
	map_x = 0;
	map_y = 0;
}

MainCharacter::~MainCharacter()
{
	Free();
}

bool MainCharacter::LoadImg(string path, SDL_Renderer* des)
{
	bool load = BaseObject::LoadImg(path, des);
	if (load = true)
	{
		w_frame_ = rect_.w / 6;
		h_frame_ = rect_.h;
	}
	return load;
}

void MainCharacter::LoadFrame() {
	if (w_frame_ > 0 && h_frame_ > 0)
	{
		for (int i = 0; i < 6; ++i)
		{
			frame_clip_[i].x = i * w_frame_;
			frame_clip_[i].y = 0;
			frame_clip_[i].w = w_frame_;
			frame_clip_[i].h = h_frame_;
		}
	}
}

void MainCharacter::Show(SDL_Renderer* des)
{
	if (status_ == LEFT)
	{
		LoadImg("image//player_run_left.png", des);
	}
	else if (status_ == RIGHT)
	{
		LoadImg("image//player_run_right.png", des);
	}
	else if (status_ == UP_LEFT)
	{
		LoadImg("image//player_up_left.png", des);
	}
	else if (status_ == UP_RIGHT)
	{
		LoadImg("image//player_up_right.png", des);
	}
	if (input_type_.left == 1 || input_type_.right == 1 || input_type_.up == 1)
	{
		frame_++;
		if (frame_ / 50 > 5) frame_ = 0;
	}
	else frame_ = 0;
	rect_.x = x_pos_ - map_x;// vị trí xuất hiện trên màn hình
	rect_.y = y_pos_ - map_y + 6;// vị trí xuất hiện trên màn hình
	SDL_Rect* currentClip = &frame_clip_[frame_ / 50];//nguồn ảnh
	SDL_Rect renderQuad = { rect_.x, rect_.y, w_frame_, h_frame_ };// điểm xuất hiện trên màn hình
	SDL_RenderCopy(des, object_, currentClip, &renderQuad);
}

void MainCharacter::HandleAction(SDL_Event e) {
	if (e.type == SDL_KEYDOWN)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_RIGHT:
			status_ = RIGHT;
			input_type_.right = 1;
			input_type_.left = 0;
			break;
		case SDLK_LEFT:
			status_ = LEFT;
			input_type_.left = 1;
			input_type_.right = 0;
			break;
			case SDLK_UP:
				if (input_type_.right == 1 || input_type_.left == 0)
				{
					status_ = UP_RIGHT;
					input_type_.up = 1;
				}
				else
				{
					status_ = UP_LEFT;
					input_type_.up = 1;
				}
		default:
			break;

		}
	}
	else if (e.type == SDL_KEYUP)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_RIGHT:
			input_type_.right = 0;
			input_type_.left = 0;
			break;
		case SDLK_LEFT:
			input_type_.left = 0;
			input_type_.right = 0;
			break;
		case SDLK_UP:
			input_type_.up = 0;
			if (input_type_.right == 1) status_ = RIGHT;
			if (input_type_.left == 1) status_ = LEFT;
		}
	}

	/*if (e.type == SDL_MOUSEBUTTONDOWN) {
		if (e.button.button == SDL_BUTTON_RIGHT) {
				input_type_.up = 1;
		}
	}*/
}

void MainCharacter::Update(Map& map) {
	x_change_ = 0;
	y_change_ += FALLING_SPEED;// nhan vat roi xuong
	if (y_change_ >= MAX_FALLING_SPEED) {
		y_change_ = MAX_FALLING_SPEED;
	}
	if (input_type_.left == 1) {
		x_change_ -= PLAYER_SPEED;
	}
	if (input_type_.right == 1) {
		x_change_ += PLAYER_SPEED;
	}
	/*if (input_type_.up == 1) {
		y_change_ = -1;
		if (y_change_ >= 1) y_change_ = 1;
	}*/
	if (input_type_.up == 1) {
		if (onGround==true) { y_change_ = -UP_CHANGE; }
		onGround = false;
	}
	MapCollisionCheck(map);
	CenterMap(map);
}

void MainCharacter::MapCollisionCheck(Map& map) {
	int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	// kiểm tra theo chiều ngang
	int heightMin = (h_frame_ < TILE_SIZE ? h_frame_ : TILE_SIZE) - 30;
	x1 = (x_pos_ + x_change_) / TILE_SIZE;//tìm nhân vật ở ô thứ bao nhiêu
	x2 = (x_pos_ + x_change_ + w_frame_ - 1) / TILE_SIZE;
	y1 = (y_pos_ + 5 )/ TILE_SIZE;
	y2 = (y_pos_ + heightMin - 1) / TILE_SIZE;
	if (x1 >= 0 && x2 <= MAX_MAP_WIDTH && y1 >= 0 && y2 <= MAX_MAP_HEIGHT) {
		if (x_change_ > 0) {//tiến
			if (map.tile[y1][x2] != 0 || map.tile[y2][x2] != 0) {
				x_pos_ = x2 * TILE_SIZE;
				x_pos_ -= w_frame_ + 1;
				x_change_ = 0;
			}
		}
		else if (x_change_ < 0) {//lùi
			if (map.tile[y1][x1] != 0 || map.tile[y2][x1] != 0) {
				x_pos_ = (x1 + 1) * TILE_SIZE;
				x_change_ = 0;
			}
		}
	}
	//kiểm tra theo chiều dọc
	int widthtMin = (w_frame_ < TILE_SIZE ? w_frame_ : TILE_SIZE) - 30;//  lấy chiều rộng bằng chân nhân vật
	x1 = (x_pos_ + 1)/ TILE_SIZE;
	x2 = (x_pos_ + widthtMin) / TILE_SIZE;
	y1 = (y_pos_ + y_change_) / TILE_SIZE;
	y2 = (y_pos_ + y_change_ + h_frame_ - 1) / TILE_SIZE;
	if (x1 >= 0 && x2 <= MAX_MAP_WIDTH && y1 >= 0 && y2 <= MAX_MAP_HEIGHT) {
		if (y_change_ > 0) {
			if (map.tile[y2][x1] != 0 || map.tile[y2][x2] != 0) {
				y_pos_ = y2 * TILE_SIZE;
				y_pos_ -= (h_frame_ + 1);
				y_change_ = 0;
				onGround = true;
			}
		}
		else if (y_change_ < 0) {
			if (map.tile[y1][x1] != 0 || map.tile[y1][x2] != 0) {
				y_pos_ = (y1 + 1) * TILE_SIZE;
				y_change_ = 0;
			}
		}
	}
	x_pos_ += x_change_;
	y_pos_ += y_change_;
	if (x_pos_ < 0) x_pos_ = 0;
	if (x_pos_ + w_frame_ > map.x_max) x_pos_ = map.x_max - w_frame_ - 1;
	if (y_pos_ < 0) y_pos_ = 0;
	if (y_pos_ + h_frame_ >= (map.y_max - 64)) {
		onGround = true;
		y_pos_ = map.y_max - h_frame_ - 64;
	}
}
void MainCharacter::CenterMap(Map& map) {
	map.x_start = x_pos_ - (SCREEN_WIDTH / 2);//Nhân vật sẽ ở giữa màn hình
	map.y_start = y_pos_ - (SCREEN_HEIGHT / 2);
	if (map.x_start < 0) map.x_start = 0;
	else if (map.x_start + SCREEN_WIDTH >= map.x_max) map.x_start = map.x_max - SCREEN_WIDTH;
	if (map.y_start < 0) map.y_start = 0;
	else if (map.y_start + SCREEN_HEIGHT >= map.y_max) map.y_start = map.y_max - SCREEN_HEIGHT;
}

