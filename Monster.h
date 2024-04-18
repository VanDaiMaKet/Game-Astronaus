#ifndef MONSTER_H_
#define MONSTER_H_
#include "BaseFunction.h"
#include "BaseObject.h"
#include "Map.h"
#include "MainCharacter.h"

class MainCharacter;
class Monster : public BaseObject {
public:
	Monster();
	~Monster();
	void SetXchange(const int& x) { x_change = x; }
	void SetYchange(const int& y) { y_change = y; }
	double GetXchange() const { return x_change; }
	double GetYchange() const { return y_change; }
	void SetXpos(const int& x) { x_pos = x; }
	void SetYpos(const int& y) { y_pos = y; }
	double GetXpos() const { return x_pos; }
	double GetYpos() const { return y_pos; }
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
	void HanleMoveType(SDL_Renderer* des, MainCharacter& Player);
	enum TypeMove {
		STAND_MONSTER = 0,
		F_LEFT = 1,
		F_RIGHT = 2,
	};
	void SetXYPosPlayer(const double& x_pos_layer_, const double& y_pos_layer_) { x_pos_player = x_pos_layer_; y_pos_player = y_pos_layer_; }
	void HandHealFlyMonster(SDL_Renderer* des);
	bool LoadHealFlyMonster(SDL_Renderer* des);
	BaseObject healFlyMonster;
	string pathHealthFlyMonster;
	double healthFlyMonster;
	double healthFlyMonsterMax;
private:
	double x_change;
	double y_change;
	double x_pos;
	double y_pos;
	double x_pos_player;
	double y_pos_player;
	int w_frame;
	int h_frame;
	SDL_Rect frame_clip[5];
	int frame;
	int map_x;
	int map_y;
	int type_move;
	KeyPress input_type;
	int move_limit_a;
	int move_limit_b;
	bool va_cham_ngang_tien;
	bool va_cham_ngang_lui;
	bool va_cham_tren;
	bool onGround;
};


#endif // !MONSTER_H_

