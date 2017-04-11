#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 45
#define JUMP_HEIGHT 96
#define FALL_STEP 4

#define SPRITESHEET_X 1.f/30.f
#define SPRITESHEET_Y 1.f/38.f

#define MIRRORED 19
#define NB_ANIMATIONS 62*2
#define POWERED 62

#define RIGHT 0
#define LEFT 1

#define KEY_A 97
#define KEY_S 115
#define KEY_D 100
#define KEY_E 101
#define KEY_J 106


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
	sprite->setAnimationSpeed(r_animation + POWERED, speed);
	sprite->setAnimationSpeed(l_animation, speed);
	sprite->setAnimationSpeed(l_animation + POWERED, speed);
	for (int i = x; i < x + size; ++i)
	{
		sprite->addKeyframe(r_animation, glm::vec2(SPRITESHEET_X * i, SPRITESHEET_Y * y));
		sprite->addKeyframe(r_animation + POWERED, glm::vec2(SPRITESHEET_X * (i + 15), SPRITESHEET_Y * y));
		sprite->addKeyframe(l_animation, glm::vec2(SPRITESHEET_X * i, SPRITESHEET_Y * (MIRRORED + y)));
		sprite->addKeyframe(l_animation + POWERED, glm::vec2(SPRITESHEET_X * (i + 15), SPRITESHEET_Y * (MIRRORED + y)));
	}		
}



void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	pick_potion = false;
	pick_sword = false;
	bFalling = false;
	bPowered = 0;
	spritesheet.loadFromFile("images/onelinesprites.png", TEXTURE_PIXEL_FORMAT_RGBA);
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
	createAnimation(JUMP_FAILED_R, JUMP_FAILED_L, 10, 14, 2, 10);
	createAnimation(STAND_R, STAND_L, 0, 0, 1, 10);
	createAnimation(MOVE_R, MOVE_L, 0, 1, 7, 8);
	createAnimation(RUN_R, RUN_L, 7, 1, 8, 10);
	createAnimation(JUMP_RUNNING_R, JUMP_RUNNING_L, 0, 5, 10, 10);
	createAnimation(STAND_SWORD_R, STAND_SWORD_L, 0, 10, 1, 10);
	createAnimation(MOVE_SWORD_R, MOVE_SWORD_L, 1, 10, 3, 10);
	createAnimation(JUMP_R, JUMP_L, 0, 14, 10, 10);
	createAnimation(SWING_R, SWING_L, 0, 15, 13, 10);
		
	sprite->changeAnimation(STAND_R + POWERED*bPowered);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	startY = posPlayer.y;
}

void Player::update(int deltaTime, int &events)
{
	sprite->update(deltaTime);
	//FALLING HANDLER
	if (!bJumping) {
		if (!map->collisionMoveDown(posPlayer, glm::ivec2(32, 64)))
		{
			if (!bFalling) startY = posPlayer.y;
			posPlayer.y += FALL_STEP;
			bFalling = true;
			if (sprite->animation() % 2 == 0) sprite->changeAnimation(FALL_R + POWERED*bPowered);
			else sprite->changeAnimation(FALL_L + POWERED*bPowered);
		}
	}
	//DUCK
	if (sprite->animation() == DUCK_R + POWERED*bPowered || sprite->animation() == DUCK_L + POWERED*bPowered) {
		if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation()))  {
			if (sprite->animation() % 2 == 0) {
				if (pick_potion) {
					sprite->changeAnimation(DRINK_R + POWERED*bPowered);
					pick_potion = false;
				}
				else sprite->changeAnimation(GET_UP_R + POWERED*bPowered);
			}
			else {
				if (pick_potion) {
					sprite->changeAnimation(DRINK_L + POWERED*bPowered);
					pick_potion = false;
				}
				else sprite->changeAnimation(GET_UP_L + POWERED*bPowered);
			}
		}
	}
	//LAND
	else if (sprite->animation() == LAND_R + POWERED*bPowered || sprite->animation() == LAND_L + POWERED*bPowered) {
		bJumping = false;
		if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation())) {
			if (sprite->animation() % 2 == 0) sprite->changeAnimation(GET_UP_R + POWERED*bPowered);
			else sprite->changeAnimation(GET_UP_L + POWERED*bPowered);
		}
	}
	//GET_UP
	else if (sprite->animation() == GET_UP_R + POWERED*bPowered || sprite->animation() == GET_UP_L + POWERED*bPowered) {
		if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation())) {
			if (sprite->animation() % 2 == 0) {
				if (pick_sword) sprite->changeAnimation(SHOW_SWORD_R + POWERED*bPowered);
				else sprite->changeAnimation(STAND_R + POWERED*bPowered);
			}
			else {
				if (pick_sword) sprite->changeAnimation(SHOW_SWORD_L + POWERED*bPowered);
				else sprite->changeAnimation(STAND_L + POWERED*bPowered);
			}
		}
	}
	//SHOW_SWORD
	else if (sprite->animation() == SHOW_SWORD_R + POWERED*bPowered || sprite->animation() == SHOW_SWORD_L + POWERED*bPowered) {
		if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation())) {
			if (sprite->animation() % 2 == 0) sprite->changeAnimation(STAND_R + POWERED*bPowered);
			else sprite->changeAnimation(STAND_L + POWERED*bPowered);
		}

	}
	//SAVE_SWORD
	else if (sprite->animation() == SAVE_SWORD_R + POWERED*bPowered || sprite->animation() == SAVE_SWORD_L + POWERED*bPowered) {
		if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation())) {
			if (sprite->animation() % 2 == 0) sprite->changeAnimation(STAND_R + POWERED*bPowered);
			else sprite->changeAnimation(STAND_L + POWERED*bPowered);
		}
	}
	//JUMP_FORWARD
	else if (sprite->animation() == JUMP_FORWARD_R + POWERED*bPowered || sprite->animation() == JUMP_FORWARD_L + POWERED*bPowered) { 
		if (wallMap->collisionMoveLeft(posPlayer, glm::ivec2(32, 64))) 
		{
			if (sprite->animation() % 2 == 1) sprite->changeAnimation(STAND_L + POWERED*bPowered);
			bJumping = false;
		}
		else if (wallMap->collisionMoveRight(posPlayer, glm::ivec2(32, 64)))
		{
			if (sprite->animation() % 2 == 0)sprite->changeAnimation(STAND_R + POWERED*bPowered);
			bJumping = false;
		}
		else if (sprite->keyFrame() == sprite->numberKeyFrames(JUMP_FORWARD_R)) {
			bJumping = false;
			if (sprite->animation() % 2 == 0) sprite->changeAnimation(STAND_R + POWERED*bPowered);
			else sprite->changeAnimation(STAND_L + POWERED*bPowered);
		}
		else if (sprite->keyFrame() >=5 && sprite->keyFrame() <=12){
			if (sprite->animation() == JUMP_FORWARD_R + POWERED*bPowered) ++posPlayer.x;
			else --posPlayer.x;
		}
	}
	//DRIFT
	else if (sprite->animation() == DRIFT_R + POWERED*bPowered || sprite->animation() == DRIFT_L + POWERED*bPowered) { 
		if (wallMap->collisionMoveLeft(posPlayer, glm::ivec2(32, 64)))
		{
			if (sprite->animation() % 2 == 1) sprite->changeAnimation(STAND_L + POWERED*bPowered);
		}
		else if (wallMap->collisionMoveRight(posPlayer, glm::ivec2(32, 64)))
		{
			if (sprite->animation() % 2 == 0)sprite->changeAnimation(STAND_R + POWERED*bPowered);
		}
		if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation())) {
			if (sprite->animation() % 2 == 0) sprite->changeAnimation(RUN_R + POWERED*bPowered);
			else sprite->changeAnimation(RUN_L + POWERED*bPowered);
		}

	}
	//STEP
	else if (sprite->animation() == STEP_R + POWERED*bPowered || sprite->animation() == STEP_L + POWERED*bPowered) {
		/*if (wallMap->collisionMoveLeft(posPlayer, glm::ivec2(32, 64)))
		{
			if (sprite->animation() % 2 == 1) sprite->changeAnimation(STAND_L + POWERED*bPowered);
		}
		else if (wallMap->collisionMoveRight(posPlayer, glm::ivec2(32, 64)))
		{
			if (sprite->animation() % 2 == 0)sprite->changeAnimation(STAND_R + POWERED*bPowered);
		}*/
		if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation())) {
			if (sprite->animation() % 2 == 0) sprite->changeAnimation(STAND_R + POWERED*bPowered);
			else sprite->changeAnimation(STAND_L + POWERED*bPowered);
		}
		else if (sprite->keyFrame() >= 2 && sprite->keyFrame() <= 5) {
			if (sprite->animation() % 2 == 0 && !wallMap->collisionMoveRight(posPlayer, glm::ivec2(32, 64))) ++posPlayer.x;
			else if (!wallMap->collisionMoveLeft(posPlayer, glm::ivec2(32, 64)))--posPlayer.x;
		}

	}
	else if (sprite->animation() == DRAW_R + POWERED*bPowered || sprite->animation() == DRAW_L + POWERED*bPowered) { //DRAW
		if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation())) {
			if (sprite->animation() % 2 == 0) sprite->changeAnimation(STAND_SWORD_R + POWERED*bPowered);
			else sprite->changeAnimation(STAND_SWORD_L + POWERED*bPowered);
		}

	}
	else if (sprite->animation() == STOP_R + POWERED*bPowered || sprite->animation() == STOP_L + POWERED*bPowered) { //STOP
		/*if (wallMap->collisionMoveLeft(posPlayer, glm::ivec2(32, 64)))
		{
			if (sprite->animation() % 2 == 1) sprite->changeAnimation(STAND_L + POWERED*bPowered);
		}
		else if (wallMap->collisionMoveRight(posPlayer, glm::ivec2(32, 64)))
		{
			if (sprite->animation() % 2 == 0)sprite->changeAnimation(STAND_R + POWERED*bPowered);
		}*/
		if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation())) {
			if (sprite->animation() % 2 == 0) 
			{
				sprite->changeAnimation(STAND_R + POWERED*bPowered);
				//++posPlayer.x;
			}
			else {
				sprite->changeAnimation(STAND_L + POWERED*bPowered);
				//--posPlayer.x;
			}
		}
		else {
			if (sprite->animation() % 2 == 0 && !wallMap->collisionMoveRight(posPlayer, glm::ivec2(32, 64))) {
				++posPlayer.x;
			}
			else if (!wallMap->collisionMoveLeft(posPlayer, glm::ivec2(32, 64))){
				--posPlayer.x;
			}
		}

	}
	else if (sprite->animation() == FALL_R + POWERED*bPowered || sprite->animation() == FALL_L + POWERED*bPowered) { //FALL
		if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 64))) {
			bFalling = false;
			if (posPlayer.y - startY > 64 * 3) bAlive = lifebar->damage(3);
			else if (posPlayer.y - startY > 64) bAlive = lifebar->damage(1);
			if (sprite->animation() % 2 == 0) {
				if (!bAlive) {
					sprite->changeAnimation(DEADLY_FALL_R + POWERED*bPowered);
				}
				else
				{
					PlaySound(TEXT("media/land-fall.wav"), NULL, SND_FILENAME | SND_ASYNC);
					sprite->changeAnimation(LAND_R + POWERED*bPowered);
				}
			}
			else {
				if (!bAlive) {
					sprite->changeAnimation(DEADLY_FALL_L + POWERED*bPowered);
				}
				else
				{
					PlaySound(TEXT("media/land-fall.wav"), NULL, SND_FILENAME | SND_ASYNC);
					sprite->changeAnimation(LAND_L + POWERED*bPowered);
				}
			}

		}
		events = 4; //IS FALLING
	}
	else if (sprite->animation() == DEFEND_R + POWERED*bPowered || sprite->animation() == DEFEND_L + POWERED*bPowered) { //DEFEND
		if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation())) {
			if (sprite->animation() % 2 == 0)
			{
				sprite->changeAnimation(STAND_SWORD_R + POWERED*bPowered);
				// DEFEND
				events = 2;
			}
			else
			{
				sprite->changeAnimation(STAND_SWORD_L + POWERED*bPowered);
				// DEFEND
				events = 2;
			}
		}

	}
	else if (sprite->animation() == TURN_R + POWERED*bPowered || sprite->animation() == TURN_L + POWERED*bPowered) { //TURN
		if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation())) {
			if (sprite->animation() % 2 == 0) sprite->changeAnimation(STAND_R + POWERED*bPowered);
			else sprite->changeAnimation(STAND_L + POWERED*bPowered);
		}

	}
	else if (sprite->animation() == DEADLY_FALL_R + POWERED*bPowered || sprite->animation() == DEADLY_FALL_L + POWERED*bPowered) { //DEADLY_FALL
		if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation())) {
	        PlaySound(TEXT("media/fall-impact.wav"), NULL, SND_FILENAME | SND_ASYNC);
			if (sprite->animation() % 2 == 0) sprite->changeAnimation(DEAD_R + POWERED*bPowered);
			else sprite->changeAnimation(DEAD_L + POWERED*bPowered);
		}

	}
	else if (sprite->animation() == DEAD_R + POWERED*bPowered || sprite->animation() == DEAD_L + POWERED*bPowered) { //DEAD

	}
	else if (sprite->animation() == SPEARS_DEATH_R + POWERED*bPowered || sprite->animation() == SPEARS_DEATH_L + POWERED*bPowered) { //SPEARS_DEATH

	}
	else if (sprite->animation() == SWORD_DEATH_R + POWERED*bPowered || sprite->animation() == SWORD_DEATH_L + POWERED*bPowered) { //SWORD_DEATH
		if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation())) {
			if (sprite->animation() % 2 == 0)
			{
				sprite->changeAnimation(DEAD_R + POWERED*bPowered);
				// DEAD
				bAlive = false;
			}
			else
			{
				sprite->changeAnimation(DEAD_L + POWERED*bPowered);
				// DEAD
				bAlive = false;
			}
		}

	}
	else if (sprite->animation() == SLICED_DEATH_R + POWERED*bPowered || sprite->animation() == SLICED_DEATH_L + POWERED*bPowered) { //SLICED_DEAD

	}
	else if (sprite->animation() == CLIMB_R + POWERED*bPowered || sprite->animation() == CLIMB_L + POWERED*bPowered) { //CLIMB
		if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation())) {
			if (sprite->animation() % 2 == 0) sprite->changeAnimation(STAND_R + POWERED*bPowered);
			else sprite->changeAnimation(STAND_L + POWERED*bPowered);
			startY = posPlayer.y;
			bJumping = false;
		}
		else if (sprite->keyFrame() >= 0 && sprite->keyFrame() <= 3)
			posPlayer.y = startY - 32;
		else posPlayer.y = startY - 64;
		if (sprite->keyFrame() > 4 ) {
			if (sprite->animation() % 2 == 0 && !wallMap->collisionMoveRight(posPlayer, glm::ivec2(32, 64))) posPlayer.x += 1;
			else if (!wallMap->collisionMoveLeft(posPlayer, glm::ivec2(32, 64)))posPlayer.x -= 1;
			
		}


	}
	else if (sprite->animation() == STAIRS_R + POWERED*bPowered || sprite->animation() == STAIRS_L + POWERED*bPowered) { //STAIRS
		if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation())){
			Game::instance().init("level02"); 
		}

	}
	else if (sprite->animation() == ATTACK_R + POWERED*bPowered || sprite->animation() == ATTACK_L + POWERED*bPowered) { //ATTACK
		if (wallMap->collisionMoveLeft(posPlayer, glm::ivec2(32, 64)))
		{
			if (sprite->animation() % 2 == 1) sprite->changeAnimation(STAND_SWORD_L + POWERED*bPowered);
		}
		else if (wallMap->collisionMoveRight(posPlayer, glm::ivec2(32, 64)))
		{
			if (sprite->animation() % 2 == 0)sprite->changeAnimation(STAND_SWORD_R + POWERED*bPowered);
		}
		if (sprite->keyFrame() == sprite->numberKeyFrames(ATTACK_R + POWERED*bPowered)) {
			if (sprite->animation() % 2 == 0) {
				sprite->changeAnimation(STAND_SWORD_R + POWERED*bPowered);
				// COMBAT
				events = 1;
			}
			else
			{
				sprite->changeAnimation(STAND_SWORD_L + POWERED*bPowered);
				// COMBAT
				events = 1;
			}
		}
		else {
			if (sprite->animation() % 2 == 0) ++posPlayer.x;
			else --posPlayer.x;
		}

	}
	else if (sprite->animation() == DRINK_R + POWERED*bPowered || sprite->animation() == DRINK_L + POWERED*bPowered) { //DRINK
		if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation())) {
			if (sprite->animation() % 2 == 0) sprite->changeAnimation(STAND_R + POWERED*bPowered);
			else sprite->changeAnimation(STAND_R + POWERED*bPowered);
		}

	}
	else if (sprite->animation() == JUMP_FAILED_R + POWERED*bPowered || sprite->animation() == JUMP_FAILED_L + POWERED*bPowered) { //JUMP_FAILED
		if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation())) {
			if (sprite->animation() % 2 == 0) sprite->changeAnimation(STAND_R + POWERED*bPowered);
			else sprite->changeAnimation(STAND_L + POWERED*bPowered);
			bJumping = false;
		}
	}
	else if (sprite->animation() == STAND_R + POWERED*bPowered || sprite->animation() == STAND_L + POWERED*bPowered) { //STAND
		if (sprite->animation() % 2 == 0) {
			if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
			{
				sprite->changeAnimation(MOVE_R + POWERED*bPowered);
				orientation = RIGHT;
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
			{
				sprite->changeAnimation(TURN_L + POWERED*bPowered);
				orientation = LEFT;
			}
			else if (Game::instance().getKey(KEY_S)) {
				sprite->changeAnimation(DRAW_R + POWERED*bPowered);
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
				sprite->changeAnimation(JUMP_R + POWERED*bPowered);
				bJumping = true;
				startY = posPlayer.y;
			}
			else if (Game::instance().getKey(KEY_J)) {
				PlaySound(TEXT("media/short-jump.wav"), NULL, SND_FILENAME | SND_ASYNC);
				sprite->changeAnimation(JUMP_FORWARD_R + POWERED*bPowered);
				bJumping = true;
				startY = posPlayer.y;
			}
			else if (Game::instance().getKey(KEY_D)) {
				sprite->changeAnimation(STEP_R + POWERED*bPowered);
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
			{
				sprite->changeAnimation(DUCK_R + POWERED*bPowered);
			}
			else if (Game::instance().getKey(KEY_E))
			{
				events = 3;
			}
		}
		else {
			if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
			{
				sprite->changeAnimation(MOVE_L + POWERED*bPowered);
				orientation = LEFT;
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
			{
				sprite->changeAnimation(TURN_R + POWERED*bPowered);
				orientation = RIGHT;
			}
			else if (Game::instance().getKey(KEY_S)) {
				sprite->changeAnimation(DRAW_L + POWERED*bPowered);
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
				sprite->changeAnimation(JUMP_L + POWERED*bPowered);
				bJumping = true;
				startY = posPlayer.y;
			}
			else if (Game::instance().getKey(KEY_J)) {
				PlaySound(TEXT("media/short-jump.wav"), NULL, SND_FILENAME | SND_ASYNC);
				sprite->changeAnimation(JUMP_FORWARD_L + POWERED*bPowered);
				bJumping = true;
				startY = posPlayer.y;
			}
			else if (Game::instance().getKey(KEY_A)) {
				sprite->changeAnimation(STEP_L + POWERED*bPowered);
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) sprite->changeAnimation(DUCK_L + POWERED*bPowered);
		}

	}
	else if (sprite->animation() == MOVE_R + POWERED*bPowered || sprite->animation() == MOVE_L + POWERED*bPowered) { //MOVE
		if (sprite->keyFrame() < sprite->numberKeyFrames(sprite->animation())) {
			if (sprite->animation() % 2 == 0) {
				if (wallMap->collisionMoveRight(posPlayer, glm::ivec2(32, 64))) sprite->changeAnimation(STAND_R + POWERED*bPowered);
				else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) ) {
					if (sprite->keyFrame() > 3) sprite->changeAnimation(RUN_R + POWERED*bPowered);
				}
				else if (sprite->keyFrame() >= 3) ++posPlayer.x;
			}
			else {
				if (wallMap->collisionMoveLeft(posPlayer, glm::ivec2(32, 64))) sprite->changeAnimation(STAND_L + POWERED*bPowered);
				else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
					if (sprite->keyFrame() > 3) sprite->changeAnimation(RUN_L + POWERED*bPowered);
				}
				else if (sprite->keyFrame() >= 3) --posPlayer.x;
			}
		}
		else if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation())) {
			if (sprite->animation() % 2 == 0) sprite->changeAnimation(STOP_R + POWERED*bPowered);
			else sprite->changeAnimation(STOP_L + POWERED*bPowered);
		}
	}
	else if (sprite->animation() == RUN_R + POWERED*bPowered || sprite->animation() == RUN_L + POWERED*bPowered) { //RUN
		bool hold = true;
		if (sprite->animation() % 2 == 0) {
			if (wallMap->collisionMoveRight(posPlayer, glm::ivec2(32, 64))) sprite->changeAnimation(STAND_R + POWERED*bPowered);
			else if (Game::instance().getKey(KEY_J)) {
				PlaySound(TEXT("media/long-jump.wav"), NULL, SND_FILENAME | SND_ASYNC);
				sprite->changeAnimation(JUMP_RUNNING_R + POWERED*bPowered);
				bJumping = true;
				startY = posPlayer.y;
				hold = false;
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) sprite->changeAnimation(DRIFT_L + POWERED*bPowered);
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) ++posPlayer.x;
			else {
				++posPlayer.x;
				hold = false;
			}
		}
		else {
			if (wallMap->collisionMoveLeft(posPlayer, glm::ivec2(32, 64))) sprite->changeAnimation(STAND_L + POWERED*bPowered);
			else if (Game::instance().getKey(KEY_J)) {
				PlaySound(TEXT("media/long-jump.wav"), NULL, SND_FILENAME | SND_ASYNC);
				sprite->changeAnimation(JUMP_RUNNING_L + POWERED*bPowered);
				bJumping = true;
				startY = posPlayer.y;
				hold = false;
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) sprite->changeAnimation(DRIFT_R + POWERED*bPowered);
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) --posPlayer.x;
			else {
				--posPlayer.x;
				hold = false;
			}
		}
		if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation()) && !hold) {
			if (sprite->animation() == RUN_R + POWERED*bPowered) {
				sprite->changeAnimation(STOP_R + POWERED*bPowered);
				++posPlayer.x;
			}
			else {
				sprite->changeAnimation(STOP_L + POWERED*bPowered);
				--posPlayer.x;
			}
		}
	}
	else if (sprite->animation() == JUMP_RUNNING_R + POWERED*bPowered || sprite->animation() == JUMP_RUNNING_L + POWERED*bPowered) { //JUMP_RUNNING
		if (wallMap->collisionMoveLeft(posPlayer, glm::ivec2(32, 64)))
		{
			if (sprite->animation() % 2 == 1) sprite->changeAnimation(STAND_L + POWERED*bPowered);
			bJumping = false;
		}
		else if (wallMap->collisionMoveRight(posPlayer, glm::ivec2(32, 64)))
		{
			if (sprite->animation() % 2 == 0)sprite->changeAnimation(STAND_R + POWERED*bPowered);
			bJumping = false;
		}
		if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation())) {
			if (sprite->animation() % 2 == 0) {
				if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 64))) {
					if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) sprite->changeAnimation(RUN_R + POWERED*bPowered);
					else sprite->changeAnimation(STOP_R + POWERED*bPowered);
				}	
			}
			else {
				if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 64))) {
					if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) sprite->changeAnimation(RUN_L + POWERED*bPowered);
					else sprite->changeAnimation(STOP_L + POWERED*bPowered);
				}
			}		
		}
		else {
			if (sprite->animation() % 2 == 0) {
				if (sprite->keyFrame() < 3) ++posPlayer.x;
				else posPlayer.x += 2;
				
			}
			else {
				if (sprite->keyFrame() < 3) --posPlayer.x;
				else posPlayer.x -= 2;
			}
			if (sprite->keyFrame() == 8) bJumping = false;
		}
	}
	else if (sprite->animation() == STAND_SWORD_R + POWERED*bPowered || sprite->animation() == STAND_SWORD_L + POWERED*bPowered) { //STAND_SWORD
		if (sprite->animation() % 2 == 0) {
			if (Game::instance().getKey(KEY_S)) sprite->changeAnimation(SAVE_SWORD_R + POWERED*bPowered);
			else if (Game::instance().getKey(KEY_D)) sprite->changeAnimation(DEFEND_R + POWERED*bPowered);
			else if (Game::instance().getKey(KEY_A))
			{
				sprite->changeAnimation(ATTACK_R + POWERED*bPowered);
				PlaySound(TEXT("media/sword-slice.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) || Game::instance().getSpecialKey(GLUT_KEY_LEFT))
			{
				sprite->changeAnimation(MOVE_SWORD_R + POWERED*bPowered);
				if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) orientation = RIGHT;
				else orientation = LEFT;
			}
		}
		else {
			if (Game::instance().getKey(KEY_S)) sprite->changeAnimation(SAVE_SWORD_L + POWERED*bPowered);
			else if (Game::instance().getKey(KEY_D)) sprite->changeAnimation(DEFEND_L + POWERED*bPowered);
			else if (Game::instance().getKey(KEY_A))
			{
				sprite->changeAnimation(ATTACK_L + POWERED*bPowered);
				PlaySound(TEXT("media/sword-slice.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) || Game::instance().getSpecialKey(GLUT_KEY_LEFT)) 
			{
				sprite->changeAnimation(MOVE_SWORD_L + POWERED*bPowered);
				if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) orientation = RIGHT;
				else orientation = LEFT;
			}
				
		}

	}
	else if (sprite->animation() == MOVE_SWORD_R + POWERED*bPowered || sprite->animation() == MOVE_SWORD_L + POWERED*bPowered) { //MOVE_SWORD
		if (wallMap->collisionMoveLeft(posPlayer, glm::ivec2(32, 64)) && orientation == LEFT) 
		{
			sprite->changeAnimation(STAND_SWORD_R + POWERED*bPowered);
		}
		else if (wallMap->collisionMoveRight(posPlayer, glm::ivec2(32, 64)) && orientation == RIGHT)
		{	
			sprite->changeAnimation(STAND_SWORD_L + POWERED*bPowered);
		}
		else if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation())) {
			if (sprite->animation() % 2 == 0) sprite->changeAnimation(STAND_SWORD_R + POWERED*bPowered);
			else sprite->changeAnimation(STAND_SWORD_L + POWERED*bPowered);
		}
		else {
			if (orientation == RIGHT) ++posPlayer.x;
			else if (orientation == LEFT) --posPlayer.x;
		}

	}
	//JUMP
	else if (sprite->animation() == JUMP_R + POWERED*bPowered || sprite->animation() == JUMP_L + POWERED*bPowered) {
		if (sprite->keyFrame() == sprite->numberKeyFrames(sprite->animation())) {
			if (sprite->animation() % 2 == 0) {
				if (map->collisionMoveUp(glm::ivec2(posPlayer.x-16, posPlayer.y), glm::ivec2(32, 64), &startY)) {
					sprite->changeAnimation(JUMP_FAILED_R + POWERED*bPowered);
				}
				else if (map->collisionClimbRight(glm::ivec2(posPlayer.x, startY), glm::ivec2(32, 64))) {
					PlaySound(TEXT("media/climb.wav"), NULL, SND_FILENAME | SND_ASYNC);
					sprite->changeAnimation(CLIMB_R + POWERED*bPowered);
					posPlayer.x += 4;
				}
				else sprite->changeAnimation(JUMP_FAILED_R + POWERED*bPowered);
			}
			else if (sprite->animation() % 2 == 1) {
				if (map->collisionMoveUp(glm::ivec2(posPlayer.x+16, posPlayer.y), glm::ivec2(32, 64), &startY)) {
					sprite->changeAnimation(JUMP_FAILED_L + POWERED*bPowered);
				}
				else if (map->collisionClimbLeft(glm::ivec2(posPlayer.x, startY), glm::ivec2(32, 64))) {
					PlaySound(TEXT("media/climb.wav"), NULL, SND_FILENAME | SND_ASYNC);
					sprite->changeAnimation(CLIMB_L + POWERED*bPowered);
					posPlayer.x -= 2;
				}
				else sprite->changeAnimation(JUMP_FAILED_L + POWERED*bPowered);
			}
		}
	}
	else if (sprite->animation() == SWING_R + POWERED*bPowered || sprite->animation() == SWING_L + POWERED*bPowered) { //SWING
		if (!Game::instance().getKey(KEY_A)) {
			if (sprite->animation() % 2 == 0) sprite->changeAnimation(JUMP_FAILED_R + POWERED*bPowered);
			else sprite->changeAnimation(JUMP_FAILED_L + POWERED*bPowered);
		}

	}
	if (!bAlive) events = -1;
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

int Player::getDirection()
{
	if (orientation == LEFT)
		return -1;
	else
		return 1;
}

int Player::swordHit()
{
	if (sprite->animation() == ATTACK_R + POWERED*bPowered && sprite->keyFrame() == sprite->numberKeyFrames(ATTACK_R + POWERED*bPowered))
		return 1;
	else if (sprite->animation() == ATTACK_L + POWERED*bPowered && sprite->keyFrame() == sprite->numberKeyFrames(ATTACK_L + POWERED*bPowered))
		return -1;
	else 
		return 0;
}

void Player::hit()
{
	// the enemy has been hit
	bool b = this->damage(1);
	if (!b)
	{
		// ha mort -> render animació de mort
		if (orientation == LEFT)
		{
			sprite->changeAnimation(SWORD_DEATH_L + POWERED*bPowered);
		}
		else
		{
			sprite->changeAnimation(SWORD_DEATH_R + POWERED*bPowered);
		}
	}
	else
	{
		// l'han impactat -> render animació de STAND
		if (orientation == LEFT)
		{
			sprite->changeAnimation(STAND_SWORD_R + POWERED*bPowered);
		}
		else
		{
			sprite->changeAnimation(STAND_SWORD_L + POWERED*bPowered);
		}
	}
}

void Player::cure() {
	pick_potion = true;
	lifebar->cure();
}

void Player::enterDoor() {
	sprite->changeAnimation(STAIRS_R + POWERED*bPowered);
}

void Player::spikes() {
	bAlive = lifebar->damage(3);
	if (!bAlive) {	
		if (sprite->animation() % 2 == 0) sprite->changeAnimation(SPEARS_DEATH_R + POWERED*bPowered);
		else sprite->changeAnimation(SPEARS_DEATH_L + POWERED*bPowered);
	}	
}

void Player::slice() {
	bAlive = lifebar->damage(3);
	if (!bAlive) {
		if (sprite->animation() % 2 == 0) sprite->changeAnimation(SLICED_DEATH_R + POWERED*bPowered);
		else sprite->changeAnimation(SLICED_DEATH_L + POWERED*bPowered);
	}
}

bool Player::isJumping(){
	return bJumping;
}

void Player::powerUp() {
	pick_potion = true;
	bPowered = 1;
}

void Player::powerDown() {
	bPowered = 0;
}
