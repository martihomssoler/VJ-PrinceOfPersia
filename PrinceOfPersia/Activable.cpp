#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Activable.h"
#include "Game.h"


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
	spritesheet.loadFromFile("images/spike-trap.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spikes = Sprite::createSprite(glm::vec2(64, 64), glm::vec2(1.f/5.f, 1.f/1.f), &spritesheet, &shaderProgram);
	spikes->setNumberAnimations(4);
	spikes->setAnimationSpeed(0, 8);
	spikes->addKeyframe(0, glm::vec2(0.2f * 0, 0.f));
	spikes->addKeyframe(0, glm::vec2(0.2f * 1, 0.f));
	spikes->addKeyframe(0, glm::vec2(0.2f * 2, 0.f));
	spikes->addKeyframe(0, glm::vec2(0.2f * 3, 0.f));
	spikes->addKeyframe(0, glm::vec2(0.2f * 4, 0.f));
	spikes->setAnimationSpeed(1, 8);
	spikes->addKeyframe(1, glm::vec2(0.2f * 4, 0.f));
	spikes->addKeyframe(1, glm::vec2(0.2f * 3, 0.f));
	spikes->addKeyframe(1, glm::vec2(0.2f * 2, 0.f));
	spikes->addKeyframe(1, glm::vec2(0.2f * 1, 0.f));
	spikes->addKeyframe(1, glm::vec2(0.2f * 0, 0.f));
	spikes->setAnimationSpeed(2, 8);
	spikes->addKeyframe(2, glm::vec2(0.2f * 4, 0.0f));
	spikes->setAnimationSpeed(3, 8);
	spikes->addKeyframe(3, glm::vec2(0.2f * 0, 0.0f));
	spikes->changeAnimation(3);
	spikes->setPosition(glm::vec2(float(tileMapDispl.x), float(tileMapDispl.y)));
}

void Activable::update(int deltaTime) {
	if (active && spikes->animation() == 3) spikes->changeAnimation(0);
	else if (!active && spikes->animation() == 2) spikes->changeAnimation(1);
	if (spikes->animation() == 0) {
		if (spikes->keyFrame() == spikes->numberKeyFrames(0)) {
			spikes->changeAnimation(2);
		}
	}
	if (spikes->animation() == 1) {
		if (spikes->keyFrame() == spikes->numberKeyFrames(1)) {
			spikes->changeAnimation(3);
		}
	}
	spikes->update(deltaTime);
}

void Activable::render() {
	spikes->render();
}

void Activable::activate() {
	active = true;
}

void Activable::deactivate() {
	active = false;
}

void Activable::setPosition(const glm::vec2 &pos)
{
	this->pos = pos;
	spikes->setPosition(glm::vec2(float(tileMapDispl.x + this->pos.x), float(tileMapDispl.y + this->pos.y)));
}

bool Activable::isActive() {
	return active || spikes->animation() == 1;
}
