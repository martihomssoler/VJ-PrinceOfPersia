#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


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
	
private:

	void createAnimation(int r_animation, int l_animation, int x, int y, int size, int speed);

	bool bJumping;
	int orientation;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *backMap;
	TileMap *map;
	TileMap *wallMap;

};


#endif // _PLAYER_INCLUDE


