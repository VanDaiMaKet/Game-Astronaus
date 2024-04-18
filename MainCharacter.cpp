#include "MainCharacter.h"

MainCharacter::MainCharacter()
{
	frame_ = 0;
	x_pos_ = 0.0;
	y_pos_ = 500.0;
	x_change_ = 0.0;
	y_change_ = 0.0;
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
	healthPlayerMax = 0;
	healthPlayer = 0;
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
		if (frame_ / 15 > 5) frame_ = 0;
	}
	else frame_ = 0;
	rect_.x = x_pos_ - map_x;// vị trí xuất hiện trên màn hình
	rect_.y = y_pos_ - map_y + 6;// vị trí xuất hiện trên màn hình
	SDL_Rect* currentClip = &frame_clip_[frame_ / 15];//nguồn ảnh
	SDL_Rect renderQuad = { rect_.x, rect_.y, w_frame_, h_frame_ };// điểm xuất hiện trên màn hình
	SDL_RenderCopy(des, object_, currentClip, &renderQuad);
}

void MainCharacter::HandleAction(SDL_Event e, SDL_Renderer* screen, Mix_Chunk* soundBullet) {
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
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		if (e.button.button == SDL_BUTTON_LEFT) {
			Bullet* onebullet = new Bullet();
			onebullet->BaseObject::LoadImg("image/bullet.png", screen);
			if (status_ == RIGHT || status_ == UP_RIGHT) {
				onebullet->SetRect(this->rect_.x + 40, rect_.y + 20);
				onebullet->SetXstart(this->rect_.x + 40);
				onebullet->SetYstart(rect_.y + 20);
				onebullet->SetXchange(1);
				onebullet->SetMove(true);
				bulletlist.push_back(onebullet);
				Mix_PlayChannel(-1, soundBullet, 0);
			}
			else {
				onebullet->SetRect(this->rect_.x , rect_.y + 20);
				onebullet->SetXstart(this->rect_.x);
				onebullet->SetYstart(rect_.y + 20);
				onebullet->SetXchange(-1);
				onebullet->SetMove(true);
				bulletlist.push_back(onebullet);
				Mix_PlayChannel(-1, soundBullet, 0);
			}
		}
	}
}
void MainCharacter::HandleBulletList(SDL_Renderer* des, Map& map, vector<Monster*>& monsterList_, Mix_Chunk* soundHit) {
	for (int i = 0; i < bulletlist.size(); i++) {
		Bullet* onebullet = bulletlist.at(i);
		if (onebullet != NULL) {
			if (onebullet->GetMove() == true) {
				onebullet->HandleMove(500,500);
				onebullet->BulletCollisionCheck(map,monsterList_,soundHit);
				onebullet->Render(des);
			}
			else {
				bulletlist.erase(bulletlist.begin() + i);
				if (onebullet != NULL) {
					onebullet->Free();
					delete onebullet;
				}
			}
		}
	}
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
	int heightMin = (h_frame_ < TILE_SIZE ? h_frame_ : TILE_SIZE) - 30 ;
	x1 = (x_pos_ + x_change_) / TILE_SIZE;//tìm nhân vật ở ô thứ bao nhiêu
	x2 = (x_pos_ + x_change_ + w_frame_ - 1) / TILE_SIZE;
	y1 = (y_pos_ + 5 )/ TILE_SIZE;
	y2 = (y_pos_ + heightMin - 1) / TILE_SIZE;
	if (x1 >= 0 && x2 <= MAX_MAP_WIDTH && y1 >= 0 && y2 <= MAX_MAP_HEIGHT) {
		if (x_change_ > 0) {//tiến
			if (map.tile[y1][x2]==1||map.tile[y1][x2]==2 || map.tile[y1][x2]==3 || map.tile[y1][x2]==6 || map.tile[y1][x2]==7 || map.tile[y2][x2]==1||map.tile[y2][x2] ==2 || map.tile[y2][x2] ==3 || map.tile[y2][x2] ==6 || map.tile[y2][x2] ==7 ) {
				x_pos_ = x2 * TILE_SIZE;
				x_pos_ -= w_frame_ + 1;
				x_change_ = 0;
			}
			else if (map.tile[y1][x2] == 4 || map.tile[y1][x2] == 5 || map.tile[y2][x2] == 4 || map.tile[y2][x2] == 5) {
				healthPlayer -= DAM_TREE;
			}
		}
		else if (x_change_ < 0) {//lùi
			if (map.tile[y1][x1] == 1 || map.tile[y1][x1] == 2 || map.tile[y1][x1] == 3 || map.tile[y1][x1] == 6 || map.tile[y1][x1] == 7 || map.tile[y2][x1] == 1 || map.tile[y2][x1] == 2 || map.tile[y2][x1] == 3 || map.tile[y2][x1] == 6 || map.tile[y2][x1] == 7) {
				x_pos_ = (x1 + 1) * TILE_SIZE;
				x_change_ = 0;
			}
			else if (map.tile[y1][x1] == 4 || map.tile[y1][x1] == 5 || map.tile[y2][x1] == 4 || map.tile[y2][x1] == 5) {
				healthPlayer -= DAM_TREE;
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
			if (map.tile[y2][x1] == 1 || map.tile[y2][x1] == 2 || map.tile[y2][x1] == 3 || map.tile[y2][x1] == 6 || map.tile[y2][x1] == 7 || map.tile[y2][x2] == 1 || map.tile[y2][x2] == 2 || map.tile[y2][x2] == 3 || map.tile[y2][x2] == 6 || map.tile[y2][x2] == 7) {
				y_pos_ = y2 * TILE_SIZE;
				y_pos_ -= (h_frame_ + 1);
				y_change_ = 0;
				onGround = true;
			}
			else if (map.tile[y2][x1]==4||map.tile[y2][x1]==5||map.tile[y2][x2] == 4 || map.tile[y2][x2] == 5) {
				healthPlayer -= DAM_TREE;
			}
		}
		else if (y_change_ < 0) {
			if (map.tile[y1][x1] == 1 || map.tile[y1][x1] == 2 || map.tile[y1][x1] == 3 || map.tile[y1][x1] == 6 || map.tile[y1][x1] == 7 || map.tile[y1][x2] == 1 || map.tile[y1][x2] == 2 || map.tile[y1][x2] == 3 || map.tile[y1][x2] == 6 || map.tile[y1][x2] == 7) {
				y_pos_ = (y1 + 1) * TILE_SIZE;
				y_change_ = 0;
			}
			else if (map.tile[y1][x1] == 4 || map.tile[y1][x1] == 5 || map.tile[y1][x2] == 4 || map.tile[y1][x2] == 5) {
				healthPlayer -= DAM_TREE;
			}
		}
	}
	x_pos_ += x_change_;
	y_pos_ += y_change_;
	if (x_pos_ < 0) x_pos_ = 0;
	if (x_pos_ + w_frame_ > map.x_max) x_pos_ = map.x_max - w_frame_ - 1;
	if (y_pos_ < 0) y_pos_ = 0;
}
void MainCharacter::CenterMap(Map& map) {
	map.x_start = x_pos_ - (SCREEN_WIDTH / 2);//Nhân vật sẽ ở giữa màn hình
	map.y_start = y_pos_ - (SCREEN_HEIGHT / 2);
	if (map.x_start < 0) map.x_start = 0;
	else if (map.x_start + SCREEN_WIDTH >= map.x_max) map.x_start = map.x_max - SCREEN_WIDTH;
	if (map.y_start < 0) map.y_start = 0;
	else if (map.y_start + SCREEN_HEIGHT >= map.y_max) map.y_start = map.y_max - SCREEN_HEIGHT;
}

bool MainCharacter::LoadHealPlayer(SDL_Renderer* des) {
	return healPlayer.LoadImg(pathHealthPlayer, des);
}
void MainCharacter::HandHealPlayer( SDL_Renderer* des) {
	int w_heal = 64 + (healthPlayer / healthPlayerMax) * 150;
	SDL_Rect src = { 0,0, w_heal , healPlayer.GetRect().h };
	SDL_Rect desrect = { 20,20,src.w,src.h };
	SDL_RenderCopy(des, healPlayer.GetObject(),&src, &desrect);
}
