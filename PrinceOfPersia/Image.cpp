#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Image.h"
#include "Game.h"

void Image::init(string file, const glm::ivec2 &size, ShaderProgram &shaderProgram)
{

	spritesheet.loadFromFile(file, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(size, glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->changeAnimation(0);
	if (size.y == 320) sprite->setPosition(glm::vec2(0, 80));
}

void Image::update(int deltaTime)
{
	sprite->update(deltaTime);
}

void Image::render()
{
	sprite->render();
}

