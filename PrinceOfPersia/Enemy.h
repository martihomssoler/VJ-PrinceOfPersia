#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


class Enemy
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, int enemy_type);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setTileBackMap(TileMap *tileMap);
	void setTileWallMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

private:

	void createAnimation(int r_animation, int l_animation, int x, int y, int size, int speed);

	glm::ivec2 tileMapDispl, posPlayer;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *backMap;
	TileMap *map;
	TileMap *wallMap;
	int enemy_type;

};


#endif // _PLAYER_INCLUDE

