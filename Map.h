#ifndef MAP_H_
#define MAP_H_
#include "BaseFunction.h"
#include "BaseObject.h"
//Class type tiles
class TileMapImage : public BaseObject
{
public:
	TileMapImage() { ; }
	~TileMapImage() { ; }
};

class MapGame 
{
public:
	MapGame() { ; }
	~MapGame() { ; };
	void LoadMap(string filetext);
	void LoadTileImage(SDL_Renderer* screen);
	void RenderMap(SDL_Renderer* screen);
	Map GetMap() const { return map_; }
	void SetMap(Map& map) {
		map_ = map;
	}
	void FreeTile() {
		for (int i = 0; i < MAX_TILE_IMAGE; i++) {
			tileMap_[i].Free();
		}
	}
private:
	Map map_;
	TileMapImage tileMap_[MAX_TILE_IMAGE];
};
#endif