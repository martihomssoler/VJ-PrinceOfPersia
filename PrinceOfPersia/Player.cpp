#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

#define SPRITESHEET_X 1.f/15.f
#define SPRITESHEET_Y 1.f/38.f

#define MIRRORED 19
#define NB_ANIMATIONS 60

#define RIGHT 0
#define LEFT 1



enum PlayerAnims
{
	DUCK_R, DUCK_L,
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
	UP_R, UP_L,
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
	createAnimation(GET_UP_R, GET_UP_L, 3, 0, 9, 10);
	createAnimation(SHOW_SWORD_R, SHOW_SWORD_L, 0, 2, 9, 10);
	createAnimation(SAVE_SWORD_R, SAVE_SWORD_L, 3, 2, 7, 10);
	createAnimation(JUMP_FORWARD_R, JUMP_FORWARD_L, 0, 3, 15, 10);
	createAnimation(DRIFT_L, DRIFT_R, 0, 4, 11, 10);
	createAnimation(STEP_R, STEP_L, 0, 6, 8, 10);
	createAnimation(DRAW_R, DRAW_L, 0, 7, 6, 10);
	createAnimation(STOP_R, STOP_L, 0, 8, 6, 10);
	createAnimation(FALL_R, FALL_L, 0, 9, 4, 10);
	createAnimation(DEFEND_R, DEFEND_L, 1, 10, 4, 10);
	createAnimation(TURN_R, TURN_L, 0, 11, 6, 10);
	createAnimation(DEADLY_FALL_R, DEADLY_FALL_L, 0, 13, 2, 10); 
	createAnimation(DEAD_R, DEAD_L, 1, 13, 1, 10);
	createAnimation(SPEARS_DEATH_R, SPEARS_DEATH_L, 2, 13, 3, 10);
	createAnimation(SWORD_DEATH_R, SWORD_DEATH_L, 5, 13, 5, 10);
	createAnimation(SLICED_DEATH_R, SLICED_DEATH_L, 10, 13, 1, 10);
	createAnimation(UP_R, UP_L, 0, 16, 15, 10);
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

	int anim = sprite->animation();

	/*switch (anim % 30){
	case 0: //DUCK
		if (sprite->keyFrame() == sprite->numberKeyFrames(anim))  {
			if (anim == DUCK_R) {
				if (pick_potion) sprite->changeAnimation(DRINK_R);
				else if (pick_sword) sprite->changeAnimation(SHOW_SWORD_R);
				else sprite->changeAnimation(GET_UP_R);
			}
			else {
				if (pick_potion) sprite->changeAnimation(DRINK_L);
				else if (pick_sword) sprite->changeAnimation(SHOW_SWORD_L);
				else sprite->changeAnimation(GET_UP_R);
			}
		}
	case 1: //GET_UP
		if (sprite->keyFrame() == sprite->numberKeyFrames(anim)) {}
	case 2: //SHOW_SWORD
		if (sprite->keyFrame() == sprite->numberKeyFrames(anim)) {}
	case 3: //SAVE_SWORD
		if (sprite->keyFrame() == sprite->numberKeyFrames(anim)) {}
	case 4: //JUMP_FORWARD
		if (sprite->keyFrame() == sprite->numberKeyFrames(anim)) {}
	case 5: //DRIFT
		if (sprite->keyFrame() == sprite->numberKeyFrames(anim)) {}
	case 6: //STEP
		if (sprite->keyFrame() == sprite->numberKeyFrames(anim)) {}
	case 7: //DRAW
		if (sprite->keyFrame() == sprite->numberKeyFrames(anim)) {}
	case 8: //STOP
		if (sprite->keyFrame() == sprite->numberKeyFrames(anim)) {
			if (anim == STOP_R) {
				sprite->changeAnimation(STAND_R);
				++posPlayer.x;
			}
			else {
				sprite->changeAnimation(STAND_L);
				--posPlayer.x;
			}
		}
	case 9: //FALL
		if (sprite->keyFrame() == sprite->numberKeyFrames(anim)) {}
	case 10: //DEFEND
		if (sprite->keyFrame() == sprite->numberKeyFrames(anim)) {}
	case 11: //TURN
		if (sprite->keyFrame() == sprite->numberKeyFrames(anim)) {}
	case 12: //DEADLY_FALL
		if (sprite->keyFrame() == sprite->numberKeyFrames(anim)) {}
	case 13: //DEAD
		if (sprite->keyFrame() == sprite->numberKeyFrames(anim)) {}
	case 14: //SPEARS_DEATH
		if (sprite->keyFrame() == sprite->numberKeyFrames(anim)) {}
	case 15: //SWORD_DEATH
		if (sprite->keyFrame() == sprite->numberKeyFrames(anim)) {}
	case 16: //SLICED_DEAD
		if (sprite->keyFrame() == sprite->numberKeyFrames(anim)) {}
	case 17: //UP
		if (sprite->keyFrame() == sprite->numberKeyFrames(anim)) {}
	case 18: //STAIRS
		if (sprite->keyFrame() == sprite->numberKeyFrames(anim)) {}
	case 19: //ATTACK
		if (sprite->keyFrame() == sprite->numberKeyFrames(anim)) {}
	case 20: //DRINK
		if(sprite->keyFrame() == sprite->numberKeyFrames(anim)) {}
	case 21: //JUMP_FAILED
		if (sprite->keyFrame() == sprite->numberKeyFrames(anim)) {}
	case 22: //STAND
		if (sprite->keyFrame() == sprite->numberKeyFrames(anim)) {}
	case 23: //MOVE
		if (sprite->keyFrame() == sprite->numberKeyFrames(anim)) {}
	case 24: //RUN
		if (sprite->keyFrame() == sprite->numberKeyFrames(anim)) {}
	case 25: //JUMP_RUNNING
		if (sprite->keyFrame() == sprite->numberKeyFrames(anim)) {}
	case 26: //STAND_SWORD
		if (sprite->keyFrame() == sprite->numberKeyFrames(anim)) {}
	case 27: //MOVE_SWORD
		if (sprite->keyFrame() == sprite->numberKeyFrames(anim)) {}
	case 28: //JUMP
		if (sprite->keyFrame() == sprite->numberKeyFrames(anim)) {}
	case 29: //SWING
		if (sprite->keyFrame() == sprite->numberKeyFrames(anim)) {}
	default:
		break;

	}*/
	if (sprite->animation() == STOP_L) {
		if (sprite->keyFrame() == sprite->numberKeyFrames(STOP_L)) sprite->changeAnimation(STAND_L);
		--posPlayer.x;
	}
	else if (sprite->animation() == STOP_R) {
		if (sprite->keyFrame() == sprite->numberKeyFrames(STOP_R)) sprite->changeAnimation(STAND_R);
		++posPlayer.x;
	}
	else if (sprite->animation() == TURN_R) {
		if (sprite->keyFrame() == sprite->numberKeyFrames(TURN_R)) sprite->changeAnimation(STAND_R);
	}
	else if (sprite->animation() == TURN_L) {
		if (sprite->keyFrame() == sprite->numberKeyFrames(TURN_L)) sprite->changeAnimation(STAND_L);
	}
	else if (sprite->animation() == DRIFT_R) {
		if (sprite->keyFrame() == sprite->numberKeyFrames(DRIFT_R)) sprite->changeAnimation(RUN_R);
	}
	else if (sprite->animation() == DRIFT_L) {
		if (sprite->keyFrame() == sprite->numberKeyFrames(DRIFT_L)) sprite->changeAnimation(RUN_L);
	}
	else if (sprite->animation() == DRAW_R) {
		if (sprite->keyFrame() == sprite->numberKeyFrames(DRAW_R)) sprite->changeAnimation(STAND_SWORD_R);
	}
	else if (sprite->animation() == DRAW_L) {
		if (sprite->keyFrame() == sprite->numberKeyFrames(DRAW_L)) sprite->changeAnimation(STAND_SWORD_L);
	}
	else if (sprite->animation() == MOVE_SWORD_R) {
		if (sprite->keyFrame() == sprite->numberKeyFrames(MOVE_SWORD_R)) sprite->changeAnimation(STAND_SWORD_R);
		else if (orientation == RIGHT) ++posPlayer.x;
		else if (orientation == LEFT) --posPlayer.x;
	}
	else if (sprite->animation() == DEFEND_R) {
		if (sprite->keyFrame() == sprite->numberKeyFrames(DEFEND_R)) sprite->changeAnimation(STAND_SWORD_R);
	}
	else if (sprite->animation() == ATTACK_R) {
		if (sprite->keyFrame() == sprite->numberKeyFrames(ATTACK_R)) {
			sprite->changeAnimation(STAND_SWORD_R);
			if (!lifebar->damage(1)) sprite->changeAnimation(SWORD_DEATH_R);
		}
		else ++posPlayer.x;
	}
	else if (sprite->animation() == SWORD_DEATH_R) {
		if (sprite->keyFrame() == sprite->numberKeyFrames(SWORD_DEATH_R)) sprite->changeAnimation(DEAD_R);
	}
	else if (sprite->animation() == SWORD_DEATH_L) {
		if (sprite->keyFrame() == sprite->numberKeyFrames(SWORD_DEATH_L)) sprite->changeAnimation(DEAD_L);
	}
	else if (sprite->animation() == SAVE_SWORD_R) {
		if (sprite->keyFrame() == sprite->numberKeyFrames(SAVE_SWORD_R)) sprite->changeAnimation(STAND_R);
	}

	else if (sprite->animation() == MOVE_SWORD_L) {
		if (sprite->keyFrame() == sprite->numberKeyFrames(MOVE_SWORD_L)) sprite->changeAnimation(STAND_SWORD_L);
		else if (orientation == RIGHT) ++posPlayer.x;
		else if (orientation == LEFT) --posPlayer.x;
	}
	else if (sprite->animation() == DEFEND_L) {
		if (sprite->keyFrame() == sprite->numberKeyFrames(DEFEND_L)) sprite->changeAnimation(STAND_SWORD_L);
	}
	else if (sprite->animation() == ATTACK_L) {
		if (sprite->keyFrame() == sprite->numberKeyFrames(ATTACK_L)) sprite->changeAnimation(STAND_SWORD_L);
		else --posPlayer.x;
	}
	else if (sprite->animation() == SAVE_SWORD_L) {
		if (sprite->keyFrame() == sprite->numberKeyFrames(SAVE_SWORD_L)) sprite->changeAnimation(STAND_L);
	}
	else if (Game::instance().getKey(115)) {
		if (sprite->animation() == STAND_SWORD_R) sprite->changeAnimation(SAVE_SWORD_R);
		else if (sprite->animation() == STAND_R) sprite->changeAnimation(DRAW_R);
		else if (sprite->animation() == STAND_SWORD_L) sprite->changeAnimation(SAVE_SWORD_L);
		else if (sprite->animation() == STAND_L) sprite->changeAnimation(DRAW_L);
	}
	else if (Game::instance().getKey(100)) {
		if (sprite->animation() == STAND_SWORD_R) sprite->changeAnimation(DEFEND_R);
		else if (sprite->animation() == STAND_SWORD_L) sprite->changeAnimation(DEFEND_L);

	}
	else if (Game::instance().getKey(97)) {
		if (sprite->animation() == STAND_SWORD_R) sprite->changeAnimation(ATTACK_R);
		else if (sprite->animation() == STAND_SWORD_L) sprite->changeAnimation(ATTACK_L);

	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && !wallMap->collisionMoveLeft(posPlayer, glm::ivec2(32, 64))) {
		if (sprite->animation() == STAND_R) sprite->changeAnimation(TURN_L);
		else if (sprite->animation() == STAND_L) sprite->changeAnimation(MOVE_L);
		else if (sprite->animation() == MOVE_L) sprite->changeAnimation(RUN_L);
		else if (sprite->animation() == RUN_L) --posPlayer.x;
		else if (sprite->animation() == RUN_R || sprite->animation() == MOVE_R) {
			sprite->changeAnimation(DRIFT_L);
			++posPlayer.x;
		}
		else if (sprite->animation() == STAND_SWORD_R) {
			sprite->changeAnimation(MOVE_SWORD_R);
			orientation = LEFT;
		}
		else if (sprite->animation() == STAND_SWORD_L) {
			sprite->changeAnimation(MOVE_SWORD_L);
			orientation = LEFT;
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && !wallMap->collisionMoveRight(posPlayer, glm::ivec2(32, 64))) {
		if (sprite->animation() == STAND_L) sprite->changeAnimation(TURN_R);
		else if (sprite->animation() == STAND_R) sprite->changeAnimation(MOVE_R);
		else if (sprite->animation() == MOVE_R) sprite->changeAnimation(RUN_R);
		else if (sprite->animation() == RUN_R) ++posPlayer.x;
		else if (sprite->animation() == RUN_L || sprite->animation() == MOVE_L) {
			sprite->changeAnimation(DRIFT_R);
			--posPlayer.x;
		}
		else if (sprite->animation() == STAND_SWORD_R) {
			sprite->changeAnimation(MOVE_SWORD_R);
			orientation = RIGHT;
		}
		else if (sprite->animation() == STAND_SWORD_L) {
			sprite->changeAnimation(MOVE_SWORD_L);
			orientation = RIGHT;
		}
	}
	else if (sprite->animation() == MOVE_R && !wallMap->collisionMoveRight(posPlayer, glm::ivec2(32, 64))) {
		if (sprite->keyFrame() == sprite->numberKeyFrames(MOVE_R)) sprite->changeAnimation(RUN_R);
	}
	else if (sprite->animation() == RUN_R && !wallMap->collisionMoveRight(posPlayer, glm::ivec2(32, 64))) {
		if (sprite->keyFrame() == sprite->numberKeyFrames(RUN_R)) sprite->changeAnimation(STOP_R);
		++posPlayer.x;
	}
	else if (sprite->animation() == MOVE_L && !wallMap->collisionMoveLeft(posPlayer, glm::ivec2(32, 64))) {
		if (sprite->keyFrame() == sprite->numberKeyFrames(MOVE_L)) sprite->changeAnimation(RUN_L);
	}
	else if (sprite->animation() == RUN_L && !wallMap->collisionMoveLeft(posPlayer, glm::ivec2(32, 64))) {
		if (sprite->keyFrame() == sprite->numberKeyFrames(RUN_L)) sprite->changeAnimation(STOP_L);
		--posPlayer.x;
	}

	if (bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{
			posPlayer.y = int(startY - 68 * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle > 90)
				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 64), &posPlayer.y);
		}
	}
	else
	{
		if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 64), &posPlayer.y))
		{
			if (Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
		else
			posPlayer.y += FALL_STEP;
	}

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

<<<<<<< HEAD
void Player::damage() {
	lifebar->damage(1);
=======
void Player::damage(int amount) 
{
	lifebar->damage(amount);
}

glm::ivec2 Player::getPostion()
{
	return posPlayer;
>>>>>>> 1b3f2a62b5ace474965e6fd5ee7db936310fba16
}

void Player::cure() {
	lifebar->cure();
}
