#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 45
#define JUMP_HEIGHT 96
#define FALL_STEP 4

#define SPRITESHEET_X 1.f/15.f
#define SPRITESHEET_Y 1.f/38.f

#define MIRRORED 19
#define NB_ANIMATIONS 62

#define RIGHT 0
#define LEFT 1

#define KEY_A 97
#define KEY_S 115
#define KEY_D 100


enum PlayerAnims
{
	DUCK_R, DUCK_L,
	LAND_R, LAND_L,
	GET_UP_R, GET_UP_L,
	SHOW_SWORD_R, SHOW_SWORD_L,
	SAVE_SWORD_R, SAVE_SWORD_L,
	JUMP_FORWARD_R, JUMP_FORWARD_L,
	DRIFT_R, DRIFT_L,
	STEP_R, STEP_L,
	DRAW_R, DRAW_L,
	STOP_R, STOP_L,
	FALL_R, FALL_L,
	DEFEND_R, DEFEND_L,
	TURN_R, TURN_L,
	DEADLY_FALL_R, DEADLY_FALL_L,
	DEAD_R, DEAD_L,
	SPEARS_DEATH_R, SPEARS_DEATH_L,
	SWORD_DEATH_R, SWORD_DEATH_L,
	SLICED_DEATH_R, SLICED_DEATH_L,
	CLIMB_R, CLIMB_L,
	STAIRS_R, STAIRS_L,
	ATTACK_R, ATTACK_L,
	DRINK_R, DRINK_L,
	JUMP_FAILED_R, JUMP_FAILED_L,
	STAND_R, STAND_L,
	MOVE_R, MOVE_L,
	RUN_R, RUN_L,
	JUMP_RUNNING_R, JUMP_RUNNING_L,
	STAND_SWORD_R, STAND_SWORD_L,
	MOVE_SWORD_R, MOVE_SWORD_L,
	JUMP_R, JUMP_L,
	SWING_R, SWING_L
};


void Player::createAnimation(int r_animation, int l_animation, int x, int y, int size, int speed)
{
	sprite->setAnimationSpeed(r_animation, speed);
	for (int i = x; i < x+size; ++i)
		sprite->addKeyframe(r_animation, glm::vec2(SPRITESHEET_X * i, SPRITESHEET_Y * y));
	sprite->setAnimationSpeed(l_animation, speed);
	for (int i = x; i < x+size; ++i)
		sprite->addKeyframe(l_animation, glm::vec2(SPRITESHEET_X * i , SPRITESHEET_Y * (MIRRORED+y)));
}



void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	spritesheet.loadFromFile("images/sprite-atlas.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(SPRITESHEET_X, SPRITESHEET_Y), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(NB_ANIMATIONS);

	createAnimation(DUCK_R, DUCK_L, 0, 0, 4, 10);
	createAnimation(LAND_R, LAND_L, 1, 0, 3, 10);
	createAnimation(GET_UP_R, GET_UP_L, 3, 0, 9, 10);
	createAnimation(SHOW_SWORD_R, SHOW_SWORD_L, 0, 2, 9, 10);
	createAnimation(SAVE_SWORD_R, SAVE_SWORD_L, 3, 2, 7, 10);
	createAnimation(JUMP_FORWARD_R, JUMP_FORWARD_L, 0, 3, 15, 10);
	createAnimation(DRIFT_L, DRIFT_R, 0, 4, 11, 10);
	createAnimation(STEP_R, STEP_L, 0, 6, 8, 10);
	createAnimation(DRAW_R, DRAW_L, 0, 7, 6, 10);
	createAnimation(STOP_R, STOP_L, 0, 8, 6, 10);
	createAnimation(FALL_R, FALL_L, 0, 9, 3, 10);
	createAnimation(DEFEND_R, DEFEND_L, 1, 10, 4, 10);
	createAnimation(TURN_R, TURN_L, 0, 11, 6, 10);
	createAnimation(DEADLY_FALL_R, DEADLY_FALL_L, 0, 13, 2, 10); 
	createAnimation(DEAD_R, DEAD_L, 1, 13, 1, 10);
	createAnimation(SPEARS_DEATH_R, SPEARS_DEATH_L, 2, 13, 3, 10);
	createAnimation(SWORD_DEATH_R, SWORD_DEATH_L, 5, 13, 5, 10);
	createAnimation(SLICED_DEATH_R, SLICED_DEATH_L, 10, 13, 1, 10);
	createAnimation(CLIMB_R, CLIMB_L, 0, 16, 12, 10);
	createAnimation(STAIRS_R, STAIRS_L, 0, 17, 15, 10);
	createAnimation(ATTACK_R, ATTACK_L, 2, 18, 5, 10);
	createAnimation(DRINK_R, DRINK_L, 0, 12, 15, 10);
	createAnimation(JUMP_FAILED_R, JUMP_FAILED_L, 10, 14, 15, 10);
	createAnimation(STAND_R, STAND_L, 0, 0, 1, 10);
	createAnimation(MOVE_R, MOVE_L, 0, 1, 7, 8);
	createAnimation(RUN_R, RUN_L, 7, 1, 8, 10);
	createAnimation(JUMP_RUNNING_R, JUMP_RUNNING_L, 0, 5, 10, 10);
	createAnimation(STAND_SWORD_R, STAND_SWORD_L, 0, 10, 1, 10);
	createAnimation(MOVE_SWORD_R, MOVE_SWORD_L, 1, 10, 3, 10);
	createAnimation(JUMP_R, JUMP_L, 0, 14, 10, 10);
	createAnimation(SWING_R, SWING_L, 0, 15, 13, 10);
		
	sprite->changeAnimation(STAND_R);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);

	if (sprite->animation() == DUCK_R || sprite->animation() == DUCK_L) { //DUCK
		if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation()))  {
			if (sprite->animation()== DUCK_R) {
				if (pick_potion) sprite->changeAnimation(DRINK_R);
				else if (pick_sword) sprite->changeAnimation(SHOW_SWORD_R);
				else sprite->changeAnimation(GET_UP_R);
			}
			else {
				if (pick_potion) sprite->changeAnimation(DRINK_L);
				else if (pick_sword) sprite->changeAnimation(SHOW_SWORD_L);
				else sprite->changeAnimation(GET_UP_L);
			}
		}
	}
	else if (sprite->animation() == LAND_R || sprite->animation() == LAND_L) {
		if (sprite->keyFrame() == sprite->numberKeyFrames(LAND_R)) {
			if (sprite->animation() == LAND_R) sprite->changeAnimation(GET_UP_R);
			else sprite->changeAnimation(GET_UP_L);
		}
	}
	else if (sprite->animation() == GET_UP_R || sprite->animation() == GET_UP_L) { //GET_UP
		if (sprite->keyFrame() == sprite->numberKeyFrames(GET_UP_R)) {
			if (sprite->animation() == GET_UP_R) sprite->changeAnimation(STAND_R);
			else sprite->changeAnimation(STAND_L);
		}
	}
	else if (sprite->animation() == SHOW_SWORD_R || sprite->animation() == SHOW_SWORD_L) { //SHOW_SWORD

	}
	else if (sprite->animation() == SAVE_SWORD_R || sprite->animation() == SAVE_SWORD_L) { //SAVE_SWORD
		if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation())) {
			if (sprite->animation()== SAVE_SWORD_R) sprite->changeAnimation(STAND_R);
			else sprite->changeAnimation(STAND_L);
		}
	}
	else if (sprite->animation() == JUMP_FORWARD_R || sprite->animation() == JUMP_FORWARD_L) { //JUMP_FORWARD

	}
	else if (sprite->animation() == DRIFT_R || sprite->animation() == DRIFT_L) { //DRIFT
		if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation())) {
			if (sprite->animation()== DRIFT_R) sprite->changeAnimation(RUN_R);
			else sprite->changeAnimation(RUN_L);
		}

	}
	else if (sprite->animation() == STEP_R || sprite->animation() == STEP_L) { //STEP

	}
	else if (sprite->animation() == DRAW_R || sprite->animation() == DRAW_L) { //DRAW
		if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation())) {
			if (sprite->animation()== DRAW_R) sprite->changeAnimation(STAND_SWORD_R);
			else sprite->changeAnimation(STAND_SWORD_L);
		}

	}
	else if (sprite->animation() == STOP_R || sprite->animation() == STOP_L) { //STOP
		if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation())) {
			if (sprite->animation()== STOP_R) {
				sprite->changeAnimation(STAND_R);
				++posPlayer.x;
			}
			else {
				sprite->changeAnimation(STAND_L);
				--posPlayer.x;
			}
		}
		else {
			if (sprite->animation() == STOP_R) {
				++posPlayer.x;
			}
			else {
				--posPlayer.x;
			}
		}

	}
	else if (sprite->animation() == FALL_R || sprite->animation() == FALL_L) { //FALL
		if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 64), &posPlayer.y)) {
			if (sprite->animation() == FALL_R) sprite->changeAnimation(LAND_R);
			else sprite->changeAnimation(LAND_L);
		}
	}
	else if (sprite->animation() == DEFEND_R || sprite->animation() == DEFEND_L) { //DEFEND
		if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation())) {
			if (sprite->animation()== DEFEND_R) sprite->changeAnimation(STAND_SWORD_R);
			else sprite->changeAnimation(STAND_SWORD_L);
		}

	}
	else if (sprite->animation() == TURN_R || sprite->animation() == TURN_L) { //TURN
		if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation())) {
			if (sprite->animation()== TURN_R) sprite->changeAnimation(STAND_R);
			else sprite->changeAnimation(STAND_L);
		}

	}
	else if (sprite->animation() == DEADLY_FALL_R || sprite->animation() == DEADLY_FALL_L) { //DEADLY_FALL

	}
	else if (sprite->animation() == DEAD_R || sprite->animation() == DEAD_L) { //DEAD

	}
	else if (sprite->animation() == SPEARS_DEATH_R || sprite->animation() == SPEARS_DEATH_L) { //SPEARS_DEATH

	}
	else if (sprite->animation() == SWORD_DEATH_R || sprite->animation() == SWORD_DEATH_L) { //SWORD_DEATH
		if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation())) {
			if (sprite->animation()== SWORD_DEATH_R) sprite->changeAnimation(DEAD_R);
			else sprite->changeAnimation(DEAD_L);
		}

	}
	else if (sprite->animation() == SLICED_DEATH_R || sprite->animation() == SLICED_DEATH_L) { //SLICED_DEAD

	}
	else if (sprite->animation() == CLIMB_R || sprite->animation() == CLIMB_L) { //CLIMB
		if (sprite->keyFrame() == sprite->numberKeyFrames(CLIMB_R)) {
			if (sprite->animation() == CLIMB_R) sprite->changeAnimation(STAND_R);
			else sprite->changeAnimation(STAND_L);
		}
		else if (sprite->keyFrame() >= 1 && sprite->keyFrame() < 4) --posPlayer.y;
		else if (sprite->keyFrame() >= 4 && sprite->keyFrame() < 9) {
			if (sprite->animation() == CLIMB_R) ++posPlayer.x;
			else --posPlayer.x;
		}

	}
	else if (sprite->animation() == STAIRS_R || sprite->animation() == STAIRS_L) { //STAIRS

	}
	else if (sprite->animation() == ATTACK_R || sprite->animation() == ATTACK_L) { //ATTACK
		if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation())) {
			if (sprite->animation()== ATTACK_R) {
				sprite->changeAnimation(STAND_SWORD_R);
				if (!lifebar->damage(1)) sprite->changeAnimation(SWORD_DEATH_R);
			}
			else sprite->changeAnimation(STAND_SWORD_L);
		}
		else {
			if (sprite->animation()== ATTACK_R) ++posPlayer.x;
			else --posPlayer.x;
		}

	}
	else if (sprite->animation() == DRINK_R || sprite->animation() == DRINK_L) { //DRINK

	}
	else if (sprite->animation() == JUMP_FAILED_R || sprite->animation() == JUMP_FAILED_L) { //JUMP_FAILED

	}
	else if (sprite->animation() == STAND_R || sprite->animation() == STAND_L) { //STAND
		if (sprite->animation()== STAND_R) {
			if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) sprite->changeAnimation(MOVE_R);
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) sprite->changeAnimation(TURN_L);
			else if (Game::instance().getKey(KEY_S)) {
				sprite->changeAnimation(DRAW_R);
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
				if (wallMap->collisionMoveRight(posPlayer, glm::ivec2(32, 64))) sprite->changeAnimation(CLIMB_R);
				else sprite->changeAnimation(JUMP_R);
			}
		}
		else {
			if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) sprite->changeAnimation(MOVE_L);
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) sprite->changeAnimation(TURN_R);
			else if (Game::instance().getKey(KEY_S)) {
				sprite->changeAnimation(DRAW_L);
			}
		}

	}
	else if (sprite->animation() == MOVE_R || sprite->animation() == MOVE_L) { //MOVE
		if (sprite->keyFrame() < sprite->numberKeyFrames(MOVE_R)) {
			if (sprite->animation()== MOVE_R) {
				if (wallMap->collisionMoveRight(posPlayer, glm::ivec2(32, 64))) sprite->changeAnimation(STAND_R);
				else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) ) {
					sprite->changeAnimation(RUN_R);
				}
			}
			else {
				if (wallMap->collisionMoveLeft(posPlayer, glm::ivec2(32, 64))) sprite->changeAnimation(STAND_L);
				else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) sprite->changeAnimation(RUN_L);
			}
		}
		else if (sprite->keyFrame() == sprite->numberKeyFrames(MOVE_R)) {
			if (sprite->animation()== MOVE_R) sprite->changeAnimation(STOP_R);
			else sprite->changeAnimation(STOP_L);
		}

	}
	else if (sprite->animation() == RUN_R || sprite->animation() == RUN_L) { //RUN
		bool hold = true;
		if (sprite->animation() == RUN_R) {
			if (wallMap->collisionMoveRight(posPlayer, glm::ivec2(32, 64))) sprite->changeAnimation(STAND_R);
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) sprite->changeAnimation(DRIFT_L);
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) ++posPlayer.x;
			else {
				++posPlayer.x;
				hold = false;
			}
		}
		else {
			if (wallMap->collisionMoveLeft(posPlayer, glm::ivec2(32, 64))) sprite->changeAnimation(STAND_L);
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) sprite->changeAnimation(DRIFT_R);
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) --posPlayer.x;
			else {
				--posPlayer.x;
				hold = false;
			}
		}
		if (sprite->keyFrame() == sprite->numberKeyFrames(RUN_R) && !hold) {
			if (sprite->animation()== RUN_R) {
				sprite->changeAnimation(STOP_R);
				++posPlayer.x;
			}
			else {
				sprite->changeAnimation(STOP_L);
				--posPlayer.x;
			}
		}

	}
	else if (sprite->animation() == DUCK_R || sprite->animation() == DUCK_L) { //JUMP_RUNNING

	}
	else if (sprite->animation() == STAND_SWORD_R || sprite->animation() == STAND_SWORD_L) { //STAND_SWORD
		if (sprite->animation() == STAND_SWORD_R) {
			if (Game::instance().getKey(KEY_S)) sprite->changeAnimation(SAVE_SWORD_R);
			else if (Game::instance().getKey(KEY_D)) sprite->changeAnimation(DEFEND_R);
			else if (Game::instance().getKey(KEY_A)) sprite->changeAnimation(ATTACK_R);
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) || Game::instance().getSpecialKey(GLUT_KEY_LEFT)){
				sprite->changeAnimation(MOVE_SWORD_R);
				if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) orientation = RIGHT;
				else orientation = LEFT;
			}
		}
		else {
			if (Game::instance().getKey(KEY_S)) sprite->changeAnimation(SAVE_SWORD_L);
			else if (Game::instance().getKey(KEY_D)) sprite->changeAnimation(DEFEND_L);
			else if (Game::instance().getKey(KEY_A)) sprite->changeAnimation(ATTACK_L);
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) || Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
				sprite->changeAnimation(MOVE_SWORD_L);
				if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) orientation = RIGHT;
				else orientation = LEFT;
			}
				
		}

	}
	else if (sprite->animation() == MOVE_SWORD_R || sprite->animation() == MOVE_SWORD_L) { //MOVE_SWORD
		if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation())) {
			if (sprite->animation()== MOVE_SWORD_R) sprite->changeAnimation(STAND_SWORD_R);
			else sprite->changeAnimation(STAND_SWORD_L);
		}
		else {
			if (orientation == RIGHT) ++posPlayer.x;
			else if (orientation == LEFT) --posPlayer.x;
		}

	}
	else if (sprite->animation() == JUMP_R || sprite->animation() == JUMP_L) { //JUMP
		if (sprite->keyFrame() == sprite->numberKeyFrames(JUMP_R)) {
			if (sprite->animation() == JUMP_R) {
				if (wallMap->collisionMoveRight(posPlayer, glm::ivec2(32, 64))) {
					sprite->changeAnimation(CLIMB_R);
					posPlayer.y -= 32;
				}
			}
			if (sprite->animation() == JUMP_L) {
				if (wallMap->collisionMoveLeft(posPlayer, glm::ivec2(32, 64))) {
					sprite->changeAnimation(CLIMB_L);
					posPlayer.y -= 32;
				}
				else sprite->changeAnimation(JUMP_FAILED_L);
			}
		}
		else if (sprite->keyFrame() > 6) --posPlayer.y;

	}
	else if (sprite->animation() == SWING_R || sprite->animation() == SWING_L) { //SWING

	}

	/*if (bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle == 225)
		{
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{
			--posPlayer.y;//= int(startY - 68 * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle > 90)
				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 64), &posPlayer.y);
		}
	}
	else
	{*/
		if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 64), &posPlayer.y))
		{
			if (Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
				if (sprite->animation() % 2 == 0) sprite->changeAnimation(JUMP_R);
				else sprite->changeAnimation(JUMP_L);
			}
		}
		else {
			posPlayer.y += FALL_STEP;
			if (sprite->animation()%2 == 0) sprite->changeAnimation(FALL_R);
			else sprite->changeAnimation(FALL_L);
		}
	//}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setTileBackMap(TileMap *tileMap)
{
	backMap = tileMap;
}
void Player::setTileWallMap(TileMap *tileMap)
{
	wallMap = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::setHealthGUI(HealthGUI *lifebar) 
{
	this->lifebar = lifebar;
}


bool Player::damage(int amount) {
	return lifebar->damage(amount);
}

glm::ivec2 Player::getPostion()
{
	return posPlayer;
}

void Player::cure() {
	lifebar->cure();
}
