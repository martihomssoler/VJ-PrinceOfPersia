#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "HealthGUI.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setTileBackMap(TileMap *tileMap);
	void setTileWallMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	void setHealthGUI(HealthGUI *lifebar);
	void cure();
	void damage(int amount);
	glm::ivec2 getPostion();
	
private:

	void createAnimation(int r_animation, int l_animation, int x, int y, int size, int speed);

	bool bJumping;
	bool pick_sword, pick_potion;
	int orientation;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *backMap;
	TileMap *map;
	TileMap *wallMap;
	HealthGUI *lifebar;

};


#endif // _PLAYER_INCLUDE


