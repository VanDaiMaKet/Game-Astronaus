#include "Bullet.h"
Bullet::Bullet() {
	x_change = 0;
	y_change = 0;
	is_move = false;

}
Bullet::~Bullet() {
	Free();
}

void Bullet::HandleMove(const int& x_, const int& y_) {
	rect_.x += x_change ;
	if ((rect_.x) > x_) is_move = false;

}
