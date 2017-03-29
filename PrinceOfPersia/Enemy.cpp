#include "Enemy.h"

void Enemy::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
}

void Enemy::update(int deltaTime)
{
}

void Enemy::render()
{
}

void Enemy::setTileMap(TileMap * tileMap)
{
}

void Enemy::setTileBackMap(TileMap * tileMap)
{
}

void Enemy::setTileWallMap(TileMap * tileMap)
{
}

void Enemy::setPosition(const glm::vec2 & pos)
{
}

void Enemy::createAnimation(int r_animation, int l_animation, int x, int y, int size, int speed) {
	sprite->setAnimationSpeed(r_animation, speed);
	for (int i = x; i < x + size; ++i)
		sprite->addKeyframe(r_animation, glm::vec2(SPRITESHEET_X * i, SPRITESHEET_Y * y));
	sprite->setAnimationSpeed(l_animation, speed);
	for (int i = x; i < x + size; ++i)
		sprite->addKeyframe(l_animation, glm::vec2(SPRITESHEET_X * i, SPRITESHEET_Y * (MIRRORED + y)));
}
