#include "Sprite.h"
#include "HealthGUI.h"
#include <vector>

#define MAX_LIFES 3

#define SPRITESHEET_X 1.f/2.f
#define SPRITESHEET_Y 1.f/2.f

#define ENEMY 0
#define PRINCE 1

enum heartStates {
	EMPTY,FULL,NONE
};



HealthGUI::HealthGUI()
{
}


HealthGUI::~HealthGUI()
{
}

void HealthGUI::init(const glm::ivec2 &pos, int points, ShaderProgram &shaderProgram, int type)
{
	health = points;
	tileMapDispl = pos;
	maxHealth = points;
	lifebar = vector<Sprite*>(maxHealth);
	lifebar_values = vector<bool>(maxHealth, false);
	spritesheet.loadFromFile("images/Life.png",TEXTURE_PIXEL_FORMAT_RGBA);
	for (int i = 0; i < maxHealth; ++i){
		if (type == PRINCE) {
			lifebar[i] = Sprite::createSprite(glm::vec2(8, 8), glm::vec2(SPRITESHEET_X, SPRITESHEET_Y), &spritesheet, &shaderProgram);
			lifebar[i]->setNumberAnimations(2);
			lifebar[i]->addKeyframe(FULL, glm::vec2(SPRITESHEET_X * 0, SPRITESHEET_Y * 0));
			lifebar[i]->addKeyframe(EMPTY, glm::vec2(SPRITESHEET_X * 1, SPRITESHEET_Y * 0));
			lifebar[i]->changeAnimation(EMPTY);
			lifebar[i]->setPosition(glm::vec2(float(tileMapDispl.x + 20 + 8 * i), float(tileMapDispl.y-10)));
		}
		else {
			lifebar[i] = Sprite::createSprite(glm::vec2(32, 32), glm::vec2(SPRITESHEET_X, SPRITESHEET_Y), &spritesheet, &shaderProgram);
			lifebar[i]->setNumberAnimations(2);
			lifebar[i]->addKeyframe(FULL, glm::vec2(SPRITESHEET_X * 0, SPRITESHEET_Y * 1));
			lifebar[i]->addKeyframe(EMPTY, glm::vec2(SPRITESHEET_X * 1, SPRITESHEET_Y * 1));
			lifebar[i]->changeAnimation(EMPTY);
			lifebar[i]->setPosition(glm::vec2(float(tileMapDispl.x + 100 - 32 * i), float(tileMapDispl.y + 580)));
		}
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

void HealthGUI::show() {
	for (int i = 0; i < maxHealth; ++i) lifebar[i]->changeAnimation(FULL);
}

void HealthGUI::render()
{
	for (int i = 0; i < maxHealth; ++i) lifebar[i]->render();
}

bool HealthGUI::damage(int amount)
{
	health -= amount;
	if (health < 0) health = 0;
	return health > 0;
}

void HealthGUI::cure()
{
	if (health < maxHealth) ++health;
}
