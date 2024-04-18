#ifndef BULLET_H_
#define BULLET_H_
#include "BaseFunction.h"
#include "BaseObject.h"
#include "Map.h"
#include "Monster.h"
#include "MainCharacter.h"

class Monster;
class Bullet : public BaseObject {
public:
	Bullet();
	~Bullet();
	void SetXstart(const int& x) { x_start = x; }
	int GetXstart() const { return x_start; }
	void SetYstart(const int& y) { y_start = y; }
	int GetYstart() const { return y_start; }
	void SetXchange(const int& x) { x_change = x; }
	void SetYchange(const int& y) { y_change = y; }
	int GetXchange() const { return x_change; }
	int GetYchange() const { return y_change; }
	void SetMove(const bool& move_) { is_move = move_; }
	bool GetMove() const { return is_move; }
	void HandleMove(const int& x_limit, const int& y_limit);
	void BulletCollisionCheck(Map& map, vector<Monster*>& monsterList_, Mix_Chunk* soundHit);
private:
	int x_start;
	int y_start;
	int x_change;
	int y_change;
	bool is_move;
};



#endif

