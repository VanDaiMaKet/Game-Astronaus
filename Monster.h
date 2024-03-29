#ifndef MONSTER_H_
#define MONSTER_H_
#include "BaseFunction.h"
#include "BaseObject.h"
#include "Map.h"
#define MONSTER_SPEED 1
#define MONSTER_FALLING_SPEED 1
#define MAX_MONSTER_FALLING_SPEED 2
#define MONSTER_SPEED 0.4

class Monster : public BaseObject {
public:
	Monster();
	~Monster();
	void SetXchange(const int& x) { x_change = x; }
	void SetYchange(const int& y) { y_change = y; }
	int GetXchange() const { return x_change; }
	int GetYchange() const { return y_change; }
	void SetXpos(const int& x) { x_pos = x; }
	void SetYpos(const int& y) { y_pos = y; }
	int GetXpos() const { return x_pos; }
	int GetYpos() const { return y_pos; }
	int GetWframe() const { return w_frame; }
	int GetHframe() const { return h_frame; }
	void SetMapXY(const int& x_start_, const int& y_start_) {
		map_x = x_start_;
		map_y = y_start_;
	}
	void LoadFrame();
	virtual bool LoadImg(string path, SDL_Renderer* des);
	void Show(SDL_Renderer* des);
	void Update(Map& map);
	void MapCollisionCheck(Map& map);// hàm kiểm tra va chạm bản đồ
	void SetTypeMove(const int& typeMove_) { type_move = typeMove_; }
	void SetMovePos(const int& posa, const int& posb) {
		move_limit_a = posa;
		move_limit_b = posb;
	}
	void SetInputLeft(const int& inputLeft_) { input_type.left = inputLeft_; }
	void HanleMoveType(SDL_Renderer* des);
	enum TypeMove {
		MONSTER = 0,
		MOVE_MONSTER = 1,
	};

private:
	double x_change;
	double y_change;
	double x_pos;
	double y_pos;
	int w_frame;
	int h_frame;
	SDL_Rect frame_clip[5];
	int frame;
	bool onGround;
	int map_x;
	int map_y;
	int type_move;
	KeyPress input_type;
	int move_limit_a;
	int move_limit_b;
};


#endif // !MONSTER_H_

