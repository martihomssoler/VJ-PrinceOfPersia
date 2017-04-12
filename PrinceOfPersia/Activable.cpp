#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Activable.h"
#include "Game.h"


#define SPRITESHEET_X 1.f/5.f
#define SPRITESHEET_Y 1.f/5.f


Activable::Activable()
{
}


Activable::~Activable()
{
}

void Activable::init(const glm::ivec2 &pos, ShaderProgram &shaderProgram, int type)
{
	tileMapDispl = pos;
	active = false;
	blocked = false;
	if (type == 2) active = true;
	auxCounter = 0;
	this->type = type;
	spritesheet.loadFromFile("images/Activables.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::vec2(64, 64), glm::vec2(SPRITESHEET_X, SPRITESHEET_Y), &spritesheet, &shaderProgram);
	if (type > 2) 
	{
		sprite->setNumberAnimations(1);
		if (type == 3) sprite->addKeyframe(0, glm::vec2(SPRITESHEET_X * 0, SPRITESHEET_Y*3));
		else sprite->addKeyframe(0, glm::vec2(SPRITESHEET_X * 1, SPRITESHEET_Y * 3));
	}
	else {
		sprite->setNumberAnimations(4);
		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(SPRITESHEET_X * 0, SPRITESHEET_Y*type));
		sprite->addKeyframe(0, glm::vec2(SPRITESHEET_X * 1, SPRITESHEET_Y*type));
		sprite->addKeyframe(0, glm::vec2(SPRITESHEET_X * 2, SPRITESHEET_Y*type));
		sprite->addKeyframe(0, glm::vec2(SPRITESHEET_X * 3, SPRITESHEET_Y*type));
		sprite->addKeyframe(0, glm::vec2(SPRITESHEET_X * 4, SPRITESHEET_Y*type));
		sprite->setAnimationSpeed(1, 8);
		sprite->addKeyframe(1, glm::vec2(SPRITESHEET_X * 4, SPRITESHEET_Y*type));
		sprite->addKeyframe(1, glm::vec2(SPRITESHEET_X * 3, SPRITESHEET_Y*type));
		sprite->addKeyframe(1, glm::vec2(SPRITESHEET_X * 2, SPRITESHEET_Y*type));
		sprite->addKeyframe(1, glm::vec2(SPRITESHEET_X * 1, SPRITESHEET_Y*type));
		sprite->addKeyframe(1, glm::vec2(SPRITESHEET_X * 0, SPRITESHEET_Y*type));
		sprite->setAnimationSpeed(2, 8);
		sprite->addKeyframe(2, glm::vec2(SPRITESHEET_X * 4, SPRITESHEET_Y*type));
		sprite->setAnimationSpeed(3, 8);
		sprite->addKeyframe(3, glm::vec2(SPRITESHEET_X * 5, SPRITESHEET_Y*type));
		sprite->changeAnimation(3);
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x), float(tileMapDispl.y)));
	
}

void Activable::update(int deltaTime) {
	if (blocked)
	{
		// Posar la tampa al últim keyframe de la seva animació
		sprite->setKeyFrame(sprite->numberKeyFrames(sprite->animation()));
		return;
	}

	switch (type) {
	default:
		if (active && sprite->animation() == 3) {
			sprite->changeAnimation(0);
			PlaySound(TEXT("media/slice.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
		else if (!active && sprite->animation() == 2) {
			sprite->changeAnimation(1);
		}
		if (sprite->animation() == 0) {
			if (sprite->keyFrame() == sprite->numberKeyFrames(0)) {
				sprite->changeAnimation(2);
			}
		}
		if (sprite->animation() == 1) {
			if (sprite->keyFrame() == sprite->numberKeyFrames(1)) {
				sprite->changeAnimation(3);
			}
		}
		break;
	case 2:
		if (sprite->animation() == 3 && auxCounter >= 80) {
			sprite->changeAnimation(0);
			PlaySound(TEXT("media/slice.wav"), NULL, SND_FILENAME | SND_ASYNC);
			auxCounter = 0;
		}
		else if (sprite->animation() == 3) ++auxCounter;
		else if (sprite->animation() == 2 && !blocked) {
			sprite->changeAnimation(1);
			PlaySound(TEXT("media/slice.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
		else if (sprite->animation() == 0) {
			if (sprite->keyFrame() == sprite->numberKeyFrames(0)) {
				sprite->changeAnimation(2);
			}
		}
		else if (sprite->animation() == 1) {
			if (sprite->keyFrame() == sprite->numberKeyFrames(1)) {
				sprite->changeAnimation(3);
			}
			break;
		case 3:
			break;
		case 4:
			break;
		}
		break;
	}
	sprite->update(deltaTime);
}

void Activable::render() {
	sprite->render();
}

void Activable::activate() {
	active = true;
}

void Activable::deactivate() {
	active = false;
}

void Activable::block() {
	blocked = true;
}

void Activable::setPosition(const glm::vec2 &pos)
{
	this->pos = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + this->pos.x), float(tileMapDispl.y + this->pos.y)));
}

bool Activable::isActive() {
	if (type == 2) return sprite->animation() == 0;
	return active || sprite->animation() == 1;
}
