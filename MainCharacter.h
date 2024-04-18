#ifndef MAIN_CHARACTER_H_
#define MAIN_CHARACTER_H_
#include <vector>
#include "BaseFunction.h"
#include "BaseObject.h"
#include "Bullet.h"
#include "Monster.h"

class Bullet;
class Monster;
class BaseObject;
class MainCharacter : public BaseObject
{
public:
	MainCharacter();
	~MainCharacter();

	virtual bool LoadImg(string path, SDL_Renderer* des);

	void Show(SDL_Renderer* des);

	void HandleAction(SDL_Event e, SDL_Renderer* screen, Mix_Chunk* soundBullet);

	void LoadFrame();

	void Update(Map& map);

	void MapCollisionCheck(Map& map);// hàm kiểm tra va chạm bản đồ

	void SetMapXY(const int& x_start_, const int& y_start_) {
		map_x = x_start_;
		map_y = y_start_;
	}
	void CenterMap(Map& map);// tính vị trí màn hình trên bản đồ full
	enum TypeOfMovement
	{
		RIGHT,
		LEFT,
		UP_LEFT,
		UP_RIGHT,
		
	};
	vector <Bullet*> GetBulletList() const {
		return bulletlist;
	}
	void SetBulletList(vector<Bullet*> bulletlist_) {
		bulletlist = bulletlist_;
	}
	void HandleBulletList(SDL_Renderer* des, Map& map, vector<Monster*>& monsterList_, Mix_Chunk* soundHit);
	double GetXPosPlayer() const { return x_pos_; }
	double GetYPosPlayer() const { return y_pos_; }

	
	void HandHealPlayer(SDL_Renderer* des);
	bool LoadHealPlayer(SDL_Renderer* des);
	BaseObject healPlayer;
	string pathHealthPlayer;
	double healthPlayer;
	double healthPlayerMax;
private:
	vector <Bullet*> bulletlist;
	//Change of x, y after event action
	double x_pos_;// vị trí người chơi trên bản đồ full
	double y_pos_;
	double x_change_;
	double y_change_;
	//Current position of character
	int w_frame_;//width of frame     rect_.w
	int h_frame_;//height of frame
	SDL_Rect frame_clip_[6];
	KeyPress input_type_;
	int frame_;
	int status_;
	bool onGround;
	int map_x;//lượng bản đồ bị cuốn về phía sau
	int map_y;
};
#endif 

