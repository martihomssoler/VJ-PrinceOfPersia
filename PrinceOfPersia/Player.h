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
	void update(int deltaTime, int &events);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setTileBackMap(TileMap *tileMap);
	void setTileWallMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	void setHealthGUI(HealthGUI *lifebar);
	void cure();
	bool damage(int amount);
	glm::ivec2 getPosition();
	int getDirection();
	int swordHit();
	void hit();
	void enterDoor(string level);
	void spikes();
	void slice();
	bool isJumping();
	void powerUp();
	void powerDown();
	bool isPowered();

	
private:

	void createAnimation(int r_animation, int l_animation, int x, int y, int size, int speed);
	void pierdeTiempo(int secs);

	bool bJumping;
	bool blocked;
	bool bFalling;
	bool bAlive;
	int bPowered;
	bool pick_sword, pick_potion;
	int orientation;
	string nextLevel;
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


