#include "Sprite.h"
#include "HealthGUI.h"



HealthGUI::HealthGUI()
{
}


HealthGUI::~HealthGUI()
{
}

void HealthGUI::init(const glm::ivec2 &pos, int points, ShaderProgram &shaderProgram)
{
	health = points;
	spritesheet.loadFromFile("images/hearts.png",TEXTURE_PIXEL_FORMAT_RGBA);
	lifebar = Sprite::createSprite(glm::vec2(64, 64), glm::vec2(64, 64), &spritesheet, &shaderProgram);
}

bool HealthGUI::damage(int amount)
{
	return false;
}

bool HealthGUI::cure(int amount)
{
	return false;
}
