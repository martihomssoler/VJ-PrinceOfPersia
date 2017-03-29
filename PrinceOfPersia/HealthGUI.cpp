#include "Sprite.h"
#include "HealthGUI.h"
#include <vector>

#define MAX_LIFES 3



HealthGUI::HealthGUI()
{
}


HealthGUI::~HealthGUI()
{
}

void HealthGUI::init(const glm::ivec2 &pos, int points, ShaderProgram &shaderProgram)
{
	health = points;
	lifebar = vector<Sprite*>(MAX_LIFES);
	lifebar_values = vector<bool>(MAX_LIFES, false);
	spritesheet.loadFromFile("images/hearts.png",TEXTURE_PIXEL_FORMAT_RGBA);
	lifebar[0] = Sprite::createSprite(glm::vec2(64, 64), glm::vec2(64, 64), &spritesheet, &shaderProgram);
	lifebar[1] = Sprite::createSprite(glm::vec2(64, 64), glm::vec2(64, 64), &spritesheet, &shaderProgram);
	lifebar[2] = Sprite::createSprite(glm::vec2(64, 64), glm::vec2(64, 64), &spritesheet, &shaderProgram);
	
}

void HealthGUI::update(int deltaTime) {
	for (int i = 0; i < health; ++i) lifebar_values[i] = true;

	for (int i = 0; i < MAX_LIFES; ++i) {
		lifebar[i]->changeAnimation(0); //Pintar l'sprite que toqui
		lifebar_values[i] = false;
	}
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
