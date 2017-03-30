#include "Sprite.h"
#include "HealthGUI.h"
#include <vector>

#define MAX_LIFES 3

#define SPRITESHEET_X 1.f/2.f
#define SPRITESHEET_Y 1.f/1.f

enum heartStates {
	EMPTY,FULL
};



HealthGUI::HealthGUI()
{
}


HealthGUI::~HealthGUI()
{
}

void HealthGUI::init(const glm::ivec2 &pos, int points, ShaderProgram &shaderProgram)
{
	health = points;
	tileMapDispl = pos;
	lifebar = vector<Sprite*>(MAX_LIFES);
	lifebar_values = vector<bool>(MAX_LIFES, false);
	spritesheet.loadFromFile("images/life.png",TEXTURE_PIXEL_FORMAT_RGBA);
	lifebar[0] = Sprite::createSprite(glm::vec2(32, 32), glm::vec2(SPRITESHEET_X, SPRITESHEET_Y), &spritesheet, &shaderProgram);
	lifebar[1] = Sprite::createSprite(glm::vec2(32, 32), glm::vec2(SPRITESHEET_X, SPRITESHEET_Y), &spritesheet, &shaderProgram);
	lifebar[2] = Sprite::createSprite(glm::vec2(32, 32), glm::vec2(SPRITESHEET_X, SPRITESHEET_Y), &spritesheet, &shaderProgram);
	for (int i = 0; i < MAX_LIFES; ++i){
		lifebar[i]->setNumberAnimations(2);
		lifebar[i]->addKeyframe(FULL, glm::vec2(SPRITESHEET_X * 0, SPRITESHEET_Y * 0));
		lifebar[i]->addKeyframe(EMPTY, glm::vec2(SPRITESHEET_X * 1, SPRITESHEET_Y * 0));
		lifebar[i]->changeAnimation(EMPTY);
		lifebar[i]->setPosition(glm::vec2(float(tileMapDispl.x + 32*i), float(tileMapDispl.y + 580)));
	}
	
	
	
}

void HealthGUI::update(int deltaTime) {

	switch (health) {
	case 0:
		lifebar[0]->changeAnimation(EMPTY);
		lifebar[1]->changeAnimation(EMPTY);
		lifebar[2]->changeAnimation(EMPTY);
		break;
	case 1:
		lifebar[0]->changeAnimation(FULL);
		lifebar[1]->changeAnimation(EMPTY);
		lifebar[2]->changeAnimation(EMPTY);
		break;
	case 2:
		lifebar[0]->changeAnimation(FULL);
		lifebar[1]->changeAnimation(FULL);
		lifebar[2]->changeAnimation(EMPTY);
		break;
	case 3:
		lifebar[0]->changeAnimation(FULL);
		lifebar[1]->changeAnimation(FULL);
		lifebar[2]->changeAnimation(FULL);
		break;
	default: break;

	}
}

void HealthGUI::render()
{
	for (int i = 0; i < MAX_LIFES; ++i) lifebar[i]->render();
}

bool HealthGUI::damage(int amount)
{
	health -= amount;
	return health > 0;
}

void HealthGUI::cure()
{
	if (health < MAX_LIFES) ++health;

}
