#ifndef _ACTIVABLE_INCLUDE
#define _ACTIVABLE_INCLUDE

#include "Sprite.h"

class Activable
{
public:
	Activable();
	~Activable();

	void init(const glm::ivec2 &pos, ShaderProgram &shaderProgram, int type);
	void update(int deltaTime);
	void setPosition(const glm::vec2 &pos);
	void render();
	void activate();
	void deactivate();
	bool isActive();

private:
	Texture spritesheet;
	Sprite* spikes;
	int type;
	glm::ivec2 tileMapDispl, pos;
	bool active;
};

#endif // _ACTIVABLE_INCLUDE

