#ifndef _IMAGE_INCLUDE
#define _IMAGE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "HealthGUI.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Image
{

public:
	void init(string file, const glm::ivec2 &size, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

private:

	Texture spritesheet;
	Sprite *sprite;

};


#endif // _PLAYER_INCLUDE

