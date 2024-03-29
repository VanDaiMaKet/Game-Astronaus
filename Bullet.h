#ifndef BULLET_H_
#define BULLET_H_
#include "BaseFunction.h"
#include "BaseObject.h"

class Bullet : public BaseObject {
public:
	Bullet();
	~Bullet();
	void SetXchange(const int& x) { x_change = x; }
	void SetYchange(const int& y) { y_change = y; }
	int GetXchange() const { return x_change; }
	int GetYchange() const { return y_change; }
	void SetMove(const bool& move_) { is_move = move_; }
	bool GetMove() const { return is_move; }
	void HandleMove(const int& x_limit, const int& y_limit);
private:
	int x_change;
	int y_change;
	bool is_move;
};



#endif

