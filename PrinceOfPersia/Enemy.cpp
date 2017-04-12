#include "Enemy.h"
#include <iostream>

#define FALL_STEP 4

#define SPRITESHEET_X 1.f/23.f
#define SPRITESHEET_Y 1.f/8.f

#define NB_ANIMATIONS 12

#define MIRRORED 2

enum PlayerAnims
{
	STAND_R, STAND_L,
	MOVE_R, MOVE_L,
	ATTACK_R, ATTACK_L,
	FALL_R, FALL_L,
	DIE_R, DIE_L,
	SPEARS_R, SPEARS_L
};

void Enemy::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram, int enemy_type, int direction)
{
	this->enemy_type = enemy_type;
	this->direction = direction;
	bAlive = true;
	bFalling = false;
	startY = posEnemy.y;
	spritesheet.loadFromFile("images/sprite-enemies.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(SPRITESHEET_X, SPRITESHEET_Y), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(NB_ANIMATIONS);

	createAnimation(STAND_R, STAND_L, 0, 0, 1, 10);
	createAnimation(MOVE_R, MOVE_L, 1, 0, 6, 10);
	createAnimation(ATTACK_R, ATTACK_L, 7, 0, 8, 10);
	createAnimation(FALL_R, FALL_L, 15, 0, 2, 10);
	createAnimation(DIE_R, DIE_L, 17, 0, 5, 10);
	createAnimation(SPEARS_R, SPEARS_L, 22, 0, 1, 10);

	tileMapDispl = tileMapPos;
	sprite->changeAnimation(STAND_L);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Enemy::update(int deltaTime, string action, int &events)
{
	if (bAlive)
	{
		sprite->update(deltaTime);

		// Falling Handler
		if (!map->collisionMoveDown(posEnemy, glm::ivec2(32, 64)))
		{
			if (!bFalling) startY = posEnemy.y;
			posEnemy.y += FALL_STEP;
			bFalling = true;
			if (sprite->animation() % 2 == 0) sprite->changeAnimation(FALL_R);
			else sprite->changeAnimation(FALL_L);
		}
		// The Enemy has collided with the floor
		else if (sprite->animation() == FALL_L || sprite->animation() == FALL_R)
		{
			bFalling = false;
			if (posEnemy.y - startY > 64 * 3) bAlive = lifebar->damage(3);
			else if (posEnemy.y - startY > 64) bAlive = lifebar->damage(1);
		}

		// Event Handler
		if (sprite->animation() == ATTACK_L || sprite->animation() == ATTACK_R)
		{
			if (sprite->keyFrame() == sprite->numberKeyFrames(ATTACK_L) || sprite->keyFrame() == sprite->numberKeyFrames(ATTACK_R))
			{
				events = 1;
			}
		}
		else if (sprite->animation() == DIE_L || sprite->animation() == DIE_R)
		{
			if (sprite->keyFrame() == sprite->numberKeyFrames(DIE_L) || sprite->keyFrame() == sprite->numberKeyFrames(DIE_R))
			{
				events = -1; // problema amb el events -1, de tant en tant no funciona correctament
			}
		}
		else
		{
			events = 0;
		}

		// Action Handler
		if (action == "MOVE_LEFT" && !wallMap->collisionMoveLeft(posEnemy, glm::ivec2(32, 64)))
		{
			if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation()))
			{
				direction = -1;
				sprite->changeAnimation(MOVE_L);
			}
			--posEnemy.x;
		}
		else if (action == "MOVE_RIGHT" && !wallMap->collisionMoveRight(posEnemy, glm::ivec2(32, 64)))
		{
			if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation()))
			{
				direction = 1;
				sprite->changeAnimation(MOVE_R);
			}
			++posEnemy.x;
		}
		else if (action == "ATTACK_LEFT" && !wallMap->collisionMoveLeft(posEnemy, glm::ivec2(32, 64)))
		{
			if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation()))
			{
				direction = -1;
				sprite->changeAnimation(ATTACK_L);
			}
		}
		else if (action == "ATTACK_RIGHT" && !wallMap->collisionMoveRight(posEnemy, glm::ivec2(32, 64)))
		{
			if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation()))
			{
				direction = 1;
				sprite->changeAnimation(ATTACK_R);
			}
		}
		else if (action == "STAND" && sprite->animation() != DIE_L && sprite->animation() != DIE_R)
		{
			if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation()))
			{
				if (direction == -1)
					sprite->changeAnimation(STAND_L);
				else
					sprite->changeAnimation(STAND_R);
			}
		}
		else if (action == "DEAD")
		{
			bAlive = false;
			if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation()))
			{
				if (direction == -1)
					sprite->changeAnimation(DIE_L);
				else
					sprite->changeAnimation(DIE_R);
			}
		}

		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
	}
	else
	{
		if (sprite->animation() == DIE_L && sprite->keyFrame() != sprite->numberKeyFrames(DIE_L))
			sprite->update(deltaTime);
		else if (sprite->animation() == DIE_R && sprite->keyFrame() != sprite->numberKeyFrames(DIE_R))
			sprite->update(deltaTime);
		else if (sprite->animation() == SPEARS_R && sprite->keyFrame() != sprite->numberKeyFrames(SPEARS_R))
			sprite->update(deltaTime);
		else if (sprite->animation() == SPEARS_L && sprite->keyFrame() != sprite->numberKeyFrames(SPEARS_L))
			sprite->update(deltaTime);
		else
			events = -1;
	}
}

void Enemy::render()
{
	sprite->render();
}

void Enemy::setTileMap(TileMap * tileMap)
{
	map = tileMap;
}

void Enemy::setTileBackMap(TileMap * tileMap)
{
	backMap = tileMap;
}

void Enemy::setTileWallMap(TileMap * tileMap)
{
	wallMap = tileMap;
}

void Enemy::setPosition(const glm::vec2 & pos)
{
	posEnemy = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Enemy::spikes()
{
	bAlive = lifebar->damage(10);
	if (!bAlive) {
		if (sprite->animation() % 2 == 0) sprite->changeAnimation(SPEARS_R);
		else sprite->changeAnimation(SPEARS_L);
	}
}

void Enemy::setHealthGUI(HealthGUI * lifebar)
{
	this->lifebar = lifebar;
}

bool Enemy::damage(int amount)
{
	return lifebar->damage(1);
}

glm::ivec2 Enemy::getPosition()
{
	return posEnemy;
}

int Enemy::getDirection()
{
	return direction;
}

void Enemy::createAnimation(int r_animation, int l_animation, int x, int y, int size, int speed)
{
	sprite->setAnimationSpeed(r_animation, speed);
	for (int i = x; i < x + size; ++i)
		sprite->addKeyframe(r_animation, glm::vec2(SPRITESHEET_X * i, SPRITESHEET_Y * (y + enemy_type)));
	sprite->setAnimationSpeed(l_animation, speed);
	for (int i = x; i < x + size; ++i)
		sprite->addKeyframe(l_animation, glm::vec2(SPRITESHEET_X * i, SPRITESHEET_Y * (MIRRORED + y + enemy_type)));
}

int Enemy::swordHit()
{
	if (sprite->animation() == ATTACK_R && sprite->keyFrame() == sprite->numberKeyFrames(ATTACK_R))
		return 1;
	else if (sprite->animation() == ATTACK_L && sprite->keyFrame() == sprite->numberKeyFrames(ATTACK_L))
		return -1;
	else
		return 0;
}

void Enemy::hit()
{
	// the enemy has been hit
	bAlive = this->damage(1);
	if (!bAlive) // it is dead
	{

		// ha mort -> render animació de mort
		if (direction == -1)
		{
			sprite->changeAnimation(DIE_L);
		}
		else 
		{
			sprite->changeAnimation(DIE_R);
		}
	}
	else
	{
		// l'han impactat -> render animació de STAND
		if (direction == -1)
		{
			sprite->changeAnimation(STAND_L);
		}
		else
		{
			sprite->changeAnimation(STAND_R);
		}
	}
}
