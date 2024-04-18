#include "Bullet.h"
Bullet::Bullet() {
	x_change = 0;
	y_change = 0;
	x_start = 0;
	y_start = 0;
	is_move = false;
}
Bullet::~Bullet() {
	Free();
}

void Bullet::HandleMove(const int& x_, const int& y_) {
	rect_.x += x_change ;
	if (rect_.x - x_start > x_|| x_start - rect_.x > x_) is_move = false;
}
void Bullet::BulletCollisionCheck(Map& map, vector<Monster*>& monsterList_,Mix_Chunk* soundHit) {
	int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	// kiểm tra theo chiều ngang
	int heightMin = 20;
	x1 = (rect_.x + map.x_start) / TILE_SIZE;
	x2 = (rect_.x + map.x_start + rect_.w - 1) / TILE_SIZE;
	y1 = (y_start) / TILE_SIZE;
	y2 = (y_start + heightMin - 1) / TILE_SIZE;
	if (x1 >= 0 && x2 <= MAX_MAP_WIDTH && y1 >= 0 && y2 <= MAX_MAP_HEIGHT) {
		if (x_change > 0) {//tiến
			if (map.tile[y1][x2] == 1 || map.tile[y1][x2] == 2 || map.tile[y1][x2] == 3 || map.tile[y1][x2] == 6 || map.tile[y1][x2] == 7 || map.tile[y2][x2] == 1 || map.tile[y2][x2] == 2 || map.tile[y2][x2] == 3 || map.tile[y2][x2] == 6 || map.tile[y2][x2] == 7) {
				is_move = false;
			}
		}
		else if (x_change < 0) {//lùi
			if (map.tile[y1][x1] == 1 || map.tile[y1][x1] == 2 || map.tile[y1][x1] == 3 || map.tile[y1][x1] == 6 || map.tile[y1][x1] == 7 || map.tile[y2][x1] == 1 || map.tile[y2][x1] == 2 || map.tile[y2][x1] == 3 || map.tile[y2][x1] == 6 || map.tile[y2][x1] == 7) {
				is_move = false;
			}
		}
	}
	for (int j = 0; j < monsterList_.size(); j++) {
		Monster* onemonster = monsterList_.at(j);
		if (onemonster != NULL) {
			if (onemonster->healthFlyMonster > 0) {
				if ( x_change < 0 && rect_.x + map.x_start - onemonster->GetXpos() < 63 && rect_.x + map.x_start - onemonster->GetXpos() > 0 && onemonster->GetYpos() - rect_.y - map.y_start < rect_.h - 1 && onemonster->GetYpos() - rect_.y - map.y_start > -onemonster->GetRect().h) {
					Mix_PlayChannel(-1, soundHit, 0);
					onemonster->healthFlyMonster -= DAM_BULLET_PLAYER;
					onemonster->SetXpos(onemonster->GetXpos() - REPEL);
					is_move = false;
				}
				if (x_change > 0 && onemonster->GetXpos() - rect_.x - map.x_start < rect_.w - 2 && onemonster->GetXpos() - rect_.x - map.x_start > 0 && onemonster->GetYpos() - rect_.y - map.y_start < rect_.h - 1 && onemonster->GetYpos() - rect_.y - map.y_start > -onemonster->GetRect().h) {
					Mix_PlayChannel(-1, soundHit, 0);
					onemonster->healthFlyMonster -= DAM_BULLET_PLAYER;
					onemonster->SetXpos(onemonster->GetXpos() + REPEL);
					is_move = false;
				}
			}
		}
	}
}