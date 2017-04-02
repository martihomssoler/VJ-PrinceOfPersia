#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "HealthGUI.h"


class Enemy
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, int enemy_type, int direction);
	void update(int deltaTime, string action);
	void render();

	void setTileMap(TileMap *tileMap);
	void setTileBackMap(TileMap *tileMap);
	void setTileWallMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	void setHealthGUI(HealthGUI *lifebar);
	bool damage(int amount);
	glm::ivec2 getPosition();
	int swordHit();
	void hit();

private:

	void createAnimation(int r_animation, int l_animation, int x, int y, int size, int speed);

	glm::ivec2 tileMapDispl, posEnemy;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *backMap;
	TileMap *map;
	TileMap *wallMap;
	int enemy_type;
	int direction;
	HealthGUI *lifebar;

};


#endif // _PLAYER_INCLUDE

