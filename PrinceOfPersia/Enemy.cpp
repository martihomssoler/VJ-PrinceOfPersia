#include "Enemy.h"

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
	spritesheet.loadFromFile("images/sprite-enemies.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(SPRITESHEET_X, SPRITESHEET_Y), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(NB_ANIMATIONS);

	createAnimation(STAND_R, STAND_L, 0, 0, 1, 10);
	createAnimation(MOVE_R, MOVE_L, 0, 1, 6, 10);
	createAnimation(ATTACK_R, ATTACK_L, 0, 7, 8, 10);
	createAnimation(FALL_R, FALL_L, 0, 15, 2, 10);
	createAnimation(DIE_R, DIE_L, 0, 17, 5, 10);
	createAnimation(SPEARS_R, SPEARS_L, 0, 22, 1, 10);

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Enemy::update(int deltaTime, string action)
{
	sprite->update(deltaTime);

	if (action == "MOVE_LEFT")
	{
		if (sprite->animation() != MOVE_L) 
		{
			sprite->changeAnimation(MOVE_L);
			--posEnemy.x;
		}
	}
	else if (action == "MOVE_RIGHT")
	{
	}
	else if (action == "ATTACK_LEFT")
	{
	}
	else if (action == "ATTACK_RIGHT")
	{
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
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

// set the enemy animation to attacking
void Enemy::attack()
{
	
}
// set the enemy animation to walking
void Enemy::walk()
{
	
}
// set the enemy animation to dying
void Enemy::die()
{
	
}

// direction: -1 left | 1 right
void Enemy::setDirection(int direction)
{
	if (this->direction != direction)
	{
		// change sprite sheet direction
		this->direction = direction;
	}
}

glm::ivec2 Enemy::getPosition()
{
	return posEnemy;
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
