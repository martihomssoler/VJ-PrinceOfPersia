#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "Sprite.h"

using namespace std;


#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES_0 1.5
#define INIT_PLAYER_Y_TILES_0 5

#define INIT_PLAYER_X_TILES_1 1.5	// 1.5		// 13
#define INIT_PLAYER_Y_TILES_1 1		// 1		// 2

#define INIT_PLAYER_X_TILES_2 1.5
#define INIT_PLAYER_Y_TILES_2 6

#define ENEMY_1 0
#define ENEMY_2 1

#define ENEMY 0
#define PRINCE 1

#define TILE_X 32
#define TILE_Y 64

#define MOVEMENT_300 150
#define ATTACK_300 295

#define MAX_POWER_TIME 2000

Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}


void Scene::init(string level)
{
	initShaders();
	bShowFinalScreen = false;
	bShowInitScreen = false;
	if (level == "level0") bShowInitScreen = true;
	if (level == "level3")
	{
		level = "level0";
		bShowFinalScreen = true;

	}
	currentLevel = level;
	if (level == "level0" ) levelNB = 0;
	else if (level == "level1") levelNB = 1;
	else levelNB = 2;
	backMap = TileMap::createTileMap("levels/" + level + "Back.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map = TileMap::createTileMap("levels/" + level + "Ground.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	wallMap = TileMap::createTileMap("levels/" + level + "Wall.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	if (level == "level0") player->setPosition(glm::vec2(INIT_PLAYER_X_TILES_0 * map->getTileSize(), INIT_PLAYER_Y_TILES_0 * map->getTileSize()));
	else if (level == "level1") player->setPosition(glm::vec2(INIT_PLAYER_X_TILES_1 * map->getTileSize(), INIT_PLAYER_Y_TILES_1 * map->getTileSize()));
	else if (level == "level2") player->setPosition(glm::vec2(INIT_PLAYER_X_TILES_2 * map->getTileSize(), INIT_PLAYER_Y_TILES_2 * map->getTileSize()));
	player->setTileBackMap(backMap);
	player->setTileMap(map);
	player->setTileWallMap(wallMap);
	pjevent = 0;
	
	playerHealth = new HealthGUI();
	playerHealth->init(player->getPosition(), 3, texProgram, PRINCE);
	player->setHealthGUI(playerHealth);
	initEnemies("levels/" + level + "Enemies.txt");
	initActivables("levels/" + level + "Activables.txt");
	
	// vector d'events pendents per gestionar al update d'una esc�na
	events = vector<int>(enemies.size(), 0);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
	bShowEnemyLifebar = vector<bool> (enemies.size(),false);
	
	initScreen = new Image();
	initScreen->init("images/InitScreen.png", glm::ivec2(640, 480), texProgram);
	

	instructions = new Image();
	instructions->init("images/instructions.png", glm::ivec2(640, 480), texProgram);
	bShowInstructions = false;

	credits = new Image();
	credits->init("images/credits.png", glm::ivec2(640, 480), texProgram);
	bShowCredits = false;

	finalScreen = new Image();
	finalScreen->init("images/YouWin.png", glm::ivec2(640, 480), texProgram);

	gameOver = new Image();
	gameOver->init("images/GameOver.png", glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), texProgram);
	bShowGameOver = false;
	loseTime = 0;
	poweredTime = 0;

	
}

void pierdeTiempo(int secs) {
	for (int k = 0; k < secs; ++k) {
		for (int i = 0; i < 100000000; ++i);
		for (int i = 0; i < 100000000; ++i);
		for (int i = 0; i < 100000000; ++i);
		for (int i = 0; i < 100000000; ++i);
		for (int i = 0; i < 100000000; ++i);
		for (int i = 0; i < 10000000; ++i);
	}
}

void Scene::update(int deltaTime)
{
	
	currentTime += deltaTime;
	glm::ivec2 playerPos = player->getPosition();
	player->update(deltaTime,pjevent);
	if (player->isPowered() && poweredTime >= MAX_POWER_TIME)
	{
		player->powerDown();
		poweredTime = 0;
	}
	else if (player->isPowered()) ++poweredTime;
	if (bShowInitScreen) {
		pierdeTiempo(3);
		bShowInitScreen = false;
	}
	if (Game::instance().getKey(105)) 
	{
		bShowInstructions = !bShowInstructions;
		pierdeTiempo(1);
	}
	if (Game::instance().getKey(99)) {
		bShowCredits = !bShowCredits;
		pierdeTiempo(1);
	}
	if (bShowFinalScreen)
	{
		if (Game::instance().getKey(114)) {
			bShowFinalScreen = false;
		}
	}
	for (unsigned int i = 0; i < events.size(); ++i)
	{
		 if (events[i] != -1 && pjevent != -1) // l'enemic segueix viu i el personatge tamb�
		{
			string action = "STAND";
			int r = rand() % 300 + 1;
			glm::ivec2 enemyPos = enemies[i].getPosition();
			// Afegir la logica aplicada al enemics
			// ara per ara el codi dels enemics esta buid!!!
			// si events[i] �s igual a (-1) significa que l'enemic ha mort
			if (r > MOVEMENT_300 && enemyPos.y + 12 >= playerPos.y && playerPos.y >= enemyPos.y - 12) // "mateixa" altura
			{
				if (enemyPos.x - TILE_X > playerPos.x && playerPos.x > enemyPos.x - 8 * TILE_X) // si el jugador esta entre 1 a 4 blocks de dist�ncia a l'ESQUERRA
				{
					action = "MOVE_LEFT";
					bShowEnemyLifebar[i] = true;
				}
				else if (r > ATTACK_300 && playerPos.x >= enemyPos.x - TILE_X && enemyPos.x >= playerPos.x) // si el jugador esta entre 1 a 8 blocks de dist�ncia a l'ESQUERRA
				{
					action = "ATTACK_LEFT";
					bShowEnemyLifebar[i] = true;
					
				}
				else if (playerPos.x < enemyPos.x + 8 * TILE_X && enemyPos.x + TILE_X < playerPos.x) // si el jugador esta entre 1 a 4 blocks de dist�ncia a la DRETA
				{
					action = "MOVE_RIGHT";
					bShowEnemyLifebar[i] = true;
				}
				else if (r > ATTACK_300 && playerPos.x <= enemyPos.x + TILE_X && enemyPos.x <= playerPos.x) // si el jugador esta entre 1 a 8 blocks de dist�ncia a la DRETA
				{
					action = "ATTACK_RIGHT";
					bShowEnemyLifebar[i] = true;
				}
				
			}

			//cout << "enemy nb: " << i << " tries to: " << action << endl;

			enemies[i].update(deltaTime, action, events[i]);
			enemyLifebars[i]->update(deltaTime);
		}
	}	
	
	eventHandler();
	// PLAYER - SPIKE TRAP LOGIC
	for (unsigned int i = 0; i < spikeAnimation.size(); ++i){
		bool active = false;
		if (player->getPosition().x + 32 > spikes[i].x && player->getPosition().x + 32 < spikes[i].x + 64 && player->getPosition().y <= spikes[i].y && spikes[i].y - player->getPosition().y <= TILE_Y*2) 
		{
			active = true;
			if (player->getPosition().x + 32 > spikes[i].x + 16 && player->getPosition().x + 32 < spikes[i].x + 48 && player->getPosition().y == spikes[i].y && !player->isJumping()) 
			{
				if (!player->isPowered())
				{
					player->setPosition(spikes[i]);
					player->spikes();
					spikeAnimation[i]->block();
				}
			}
		}
		//else if (player->getPosition().x + 32 <= spikes[i].x || player->getPosition().x + 32 >= spikes[i].x + 64) spikeAnimation[i]->deactivate();

		for (unsigned int j = 0; j < enemies.size(); ++j)
		{
			if (enemies[j].getPosition().x + 32 > spikes[i].x && enemies[j].getPosition().x + 32 < spikes[i].x + 64 && enemies[j].getPosition().y <= spikes[i].y && spikes[i].y - enemies[j].getPosition().y <= TILE_Y * 2)
			{
				active = true;
				if (enemies[j].getPosition().x + 32 > spikes[i].x + 16 && enemies[j].getPosition().x + 32 < spikes[i].x + 48 && enemies[j].getPosition().y == spikes[i].y)
				{
					enemies[j].setPosition(spikes[i]);
					enemies[j].spikes();
					spikeAnimation[i]->block();
				}
			}
		}

		if (active)
			spikeAnimation[i]->activate();
		else
			spikeAnimation[i]->deactivate();

		spikeAnimation[i]->update(deltaTime);
	}
	// OTHERS
	for (unsigned int i = 0; i < piercingTrapAnimation.size(); ++i){
		if (player->getPosition().y == piercingTraps[i].y) {
			if (piercingTrapAnimation[i]->isActive() && player->getPosition().x + 32 >= piercingTraps[i].x && player->getPosition().x + 32 <= piercingTraps[i].x + 32) {
				if (!player->isPowered())
				{
					player->setPosition(glm::ivec2(piercingTraps[i].x - 32, piercingTraps[i].y));
					player->slice();
				}
			}
		}
		piercingTrapAnimation[i]->update(deltaTime);
	}

	for (unsigned int i = 0; i < fallingPlatesAnimation.size(); ++i){
		if (player->getPosition().x + 32 > fallingPlates[i].x && player->getPosition().x + 32 < fallingPlates[i].x + 64 && player->getPosition().y == fallingPlates[i].y) {
			map->changeTile(fallingPlates[i].x / TILE_X, fallingPlates[i].y / TILE_Y, 0);
			fallingPlatesAnimation[i]->activate();
		}
		fallingPlatesAnimation[i]->update(deltaTime);
	}
	playerHealth->update(deltaTime);
}

void Scene::eventHandler()
{
	glm::ivec2 playerPos = player->getPosition();
	int direction = player->getDirection();
	switch (pjevent)
	{
		case 1:
			pjevent = 0;
			// THE PRINCE CAN POSSIBLY HIT AN ENEMY
			for (unsigned int i = 0; i < enemies.size(); ++i)
			{
				if (events[i] != -1) // l'enemic segueix viu
				{
					glm::ivec2 enemyPos = enemies[i].getPosition();
					if (direction == -1) // LEFT
					{
						if (playerPos.x - (2 * TILE_X / 3) <= enemyPos.x && enemyPos.x <= playerPos.x + (2 * TILE_X / 3))
						{
							enemies[i].hit();
							events[i] = 0;							
						}
					}
					else // RIGHT
					{
						if (playerPos.x - (2 * TILE_X / 3) <= enemyPos.x && enemyPos.x <= playerPos.x + (2 * TILE_X / 3))
						{
							enemies[i].hit();
							events[i] = 0;
						}
					}
				}
			}
			break;
		case 2:
			pjevent = 0;
			// THE PRINCE CAN POSSIBLY PARRIED AN ENEMY BLOW
			for (unsigned int i = 0; i < enemies.size(); ++i)
			{
				if (events[i] != -1) // l'enemic segueix viu
				{
					glm::ivec2 enemyPos = enemies[i].getPosition();
					if (direction == -1) // LEFT
					{
						if (playerPos.x - TILE_X <= enemyPos.x && enemyPos.x <= playerPos.x)
						{
							events[i] = 0;
						}
					}
					else // RIGHT
					{
						if (playerPos.x <= enemyPos.x && enemyPos.x <= playerPos.x + TILE_X)
						{
							events[i] = 0;
						}
					}
				}
			}
			break;
		case 3:
			pjevent = 0;
			// E pressed MAYBE AT THE DOOR
			if ((playerPos.x >= door2.x - 32  && door2.x + 16 >= playerPos.x) && (playerPos.y == door2.y))
			{
				player->setPosition(door2);
				player->enterDoor("level2");				
			}
			else if ((playerPos.x >= door1.x - 32  && door1.x + 16 >= playerPos.x) && (playerPos.y == door1.y))
			{
				player->setPosition(door1);
				player->enterDoor("level1");
			}
			else if ((playerPos.x >= door0.x - 32  && door0.x + 16 >= playerPos.x) && (playerPos.y == door0.y))
			{
				player->setPosition(door0);
				player->enterDoor("level0");
			}
			else if ((playerPos.x >= windoor.x - 32 && windoor.x + 16 >= playerPos.x) && (playerPos.y == windoor.y))
			{
				player->setPosition(windoor);
				player->enterDoor("level3");
			}
			else {
				for (int i = 0; i < powerPotion.size(); ++i) {
					if (powerPotionAnimation[i]->isActive() && playerPos.x >= powerPotion[i].x - 32 && playerPos.x <= powerPotion[i].x && playerPos.y == powerPotion[i].y)
					{
						player->powerUp();
						powerPotionAnimation[i]->deactivate();
					}
				}
				for (int i = 0; i < healPotion.size(); ++i) {
					if (healPotionAnimation[i]->isActive() && playerPos.x >= healPotion[i].x - 32 && playerPos.x <= healPotion[i].x && playerPos.y == healPotion[i].y)
					{
						player->cure();
						healPotionAnimation[i]->deactivate();
					}
				}
			}
			break;
		case -1:

			if (loseTime >= 50)
			{
				if (!bShowGameOver)
					PlaySound(TEXT("media/GAMEOVER.wav"), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
				bShowGameOver = true;
				loseTime = 0;
			}
			else ++loseTime;
			if (Game::instance().getKey(114)) //R 
			{
				Game::instance().init(currentLevel);
				PlaySound(NULL, 0, 0); // To stop playing the Game-Over sound
			}
			break;
		default:
			break;
	}
	

	for (unsigned int i = 0; i < events.size(); ++i) // des de 0 fins a events.size()-2 hi ha enemics, l'event del princep �s el events.size()-1
	{
		glm::ivec2 enemyPos = enemies[i].getPosition();
		int direction = enemies[i].getDirection();
		switch (events[i])
		{
		case 1:
			events[i] = 0;
			// THE PRINCE CAN POSSIBLY HIT AN ENEMY
			if (direction == -1) // LEFT
			{
				if (enemyPos.x - TILE_X <= playerPos.x && playerPos.x <= enemyPos.x)
				{
					player->hit();					
				}

			}
			else // RIGHT
			{
				if (enemyPos.x <= playerPos.x && playerPos.x <= enemyPos.x + TILE_X)
				{
					player->hit();
				}
			}
			break;
		case -1:
			bShowEnemyLifebar[i] = false;
			break;
		default:
			break;
		}
	}
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	backMap->render();

	for (unsigned int i = 0; i < fallingPlatesAnimation.size(); ++i)
	{
		texProgram.use();
		texProgram.setUniformMatrix4f("projection", projection);
		texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		modelview = glm::mat4(1.0f);
		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
		if (fallingPlatesAnimation[i]->isActive()) fallingPlatesAnimation[i]->render();

	}

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	player->render();

	for (unsigned int i = 0; i < piercingTrapAnimation.size(); ++i)
	{
		texProgram.use();
		texProgram.setUniformMatrix4f("projection", projection);
		texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		modelview = glm::mat4(1.0f);
		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
		//if (piercingTrapAnimation[i]->isActive()) 
		piercingTrapAnimation[i]->render();
	}

	for (unsigned int i = 0; enemies.size() > i; ++i)
	{
		texProgram.use();
		texProgram.setUniformMatrix4f("projection", projection);
		texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		modelview = glm::mat4(1.0f);
		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
		enemies[i].render();
	}

	for (unsigned int i = 0; i < spikeAnimation.size(); ++i)
	{
		texProgram.use();
		texProgram.setUniformMatrix4f("projection", projection);
		texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		modelview = glm::mat4(1.0f);
		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
		if (spikeAnimation[i]->isActive()) spikeAnimation[i]->render();

	}
	for (unsigned int i = 0; i < powerPotionAnimation.size(); ++i)
	{
		texProgram.use();
		texProgram.setUniformMatrix4f("projection", projection);
		texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		modelview = glm::mat4(1.0f);
		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
		if (powerPotionAnimation[i]->isActive()) powerPotionAnimation[i]->render();

	}

	for (unsigned int i = 0; i < healPotionAnimation.size(); ++i)
	{
		texProgram.use();
		texProgram.setUniformMatrix4f("projection", projection);
		texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		modelview = glm::mat4(1.0f);
		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
		if (healPotionAnimation[i]->isActive()) healPotionAnimation[i]->render();

	}
		
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	wallMap->render();
	

	for (unsigned int i = 0; enemies.size() > i; ++i)
	{
		
		texProgram.use();
		texProgram.setUniformMatrix4f("projection", glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f));
		texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		modelview = glm::mat4(1.0f);
		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
		if (bShowEnemyLifebar[i]) enemyLifebars[i]->render();
	}

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f));
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	playerHealth->render();
		
	if (bShowGameOver)
	{
		gameOver->render();
	}

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f));
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	if (bShowGameOver) gameOver->render();

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f));
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	if (bShowInitScreen) initScreen->render();

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f));
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	if (bShowInstructions) instructions->render();

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f));
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	if (bShowCredits) credits->render();

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f));
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	if (bShowFinalScreen) finalScreen->render();

	//glm::vec3 translation = glm::vec3(2 * TILE_X*INIT_PLAYER_X_TILES - player->getPostion().x, TILE_Y*INIT_PLAYER_Y_TILES - player->getPostion().y, 0);
	projection = glm::translate(glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f), getTranslationMap());
}

glm::vec3 Scene::getTranslationMap()
{
	int left = player->getPosition().x - SCREEN_WIDTH / 2;
	int up = player->getPosition().y - SCREEN_HEIGHT / 2;

	if (left < 0)
	{
		left = 0;
	}
	if (map->getWidth() < left + SCREEN_WIDTH)
	{
		left = map->getWidth() - SCREEN_WIDTH;
	}

	if (up < 0)
	{
		up = 0;
	}
	if (map->getHeight() < up + SCREEN_HEIGHT)
	{
		up = map->getHeight() - SCREEN_HEIGHT;
	}

	return glm::vec3(-left, -up, 0);
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Scene::initEnemies(const string & enemiesFile)
{
	ifstream fin;
	string line;
	stringstream sstream;
	int numEnemies, maxEnemies, mapSizex, mapSizey;

	fin.open(enemiesFile.c_str());
	if (!fin.is_open())
		return;
	getline(fin, line);
	if (line.compare(0, 7, "ENEMIES") != 0)
		return;
	getline(fin, line);
	sstream.str(line);
	sstream >> numEnemies;
	getline(fin, line);
	sstream.str(line);
	sstream >> maxEnemies;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSizex >> mapSizey;

	int k = 0;
	enemies = vector<Enemy>(numEnemies);
	enemyLifebars = vector<HealthGUI*>(numEnemies);

	for (int j = 0; j<mapSizey; j++)
	{
		getline(fin, line);
		std::stringstream   linestream(line);
		std::string         value;

		for (int i = 0; i<mapSizex; i++)
		{
			getline(linestream, value, ',');
			int aux = atoi(value.c_str()) + 1;
			if (aux != 0 && k < numEnemies)
			{
				// Comprovar que el calcul de type i position es el correcte!
				// si aux > maxEnemies �s cert, l'enemic estar� mirant cap a l'esquerra (-1)
				// altrement si �s fal�, l'enemic estar� mirant cap a la dreta (1)
				enemies[k].init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, aux % maxEnemies, 1 - 2 * (aux > maxEnemies));
				enemies[k].setPosition(glm::vec2(i * TILE_X, j * TILE_Y));
				enemies[k].setTileBackMap(backMap);
				enemies[k].setTileMap(map);
				enemies[k].setTileWallMap(wallMap);
				enemyLifebars[k] = new HealthGUI();
				enemyLifebars[k]->init(glm::vec2(i * TILE_X, j * TILE_Y), 3, texProgram, ENEMY);
				enemies[k].setHealthGUI(enemyLifebars[k]);
				++k;
			}
		}
	}
	fin.close();
}

void Scene::initActivables(const string & activablesFile)
{
	ifstream fin;
	string line;
	stringstream sstream;
	int mapSizex, mapSizey;

	fin.open(activablesFile.c_str());
	if (!fin.is_open())
		return;
	getline(fin, line);
	if (line.compare(0, 10, "ACTIVABLES") != 0)
		return;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSizex >> mapSizey;

	powerPotion.clear();
	powerPotionAnimation.clear();
	healPotion.clear();
	healPotionAnimation.clear();
	piercingTraps.clear();
	piercingTrapAnimation.clear();
	spikes.clear();
	spikeAnimation.clear();
	fallingPlates.clear();
	fallingPlatesAnimation.clear();
	
	for (int j = 0; j < mapSizey; j++)
	{
		getline(fin, line);
		std::stringstream   linestream(line);
		std::string         value;

		for (int i = 0; i < mapSizex; i++)
		{
			getline(linestream, value, ',');
			int aux = atoi(value.c_str()) + 1;
			switch (aux)
			{
				case 1: // POWER POTION
					powerPotion.push_back(glm::ivec2(i * TILE_X, j * TILE_Y));
					powerPotionAnimation.push_back(new Activable());
					powerPotionAnimation.back()->init(powerPotion.back(), texProgram, 4);
					powerPotionAnimation.back()->activate();
					break;
				case 2: // PIERCING TRAP
					piercingTraps.push_back(glm::ivec2(i * TILE_X, j * TILE_Y));
					piercingTrapAnimation.push_back(new Activable());
					piercingTrapAnimation.back()->init(piercingTraps.back(), texProgram, 2);
					break;
				case 3: //SPIKES
					spikes.push_back(glm::ivec2(i * TILE_X, j * TILE_Y));
					spikeAnimation.push_back(new Activable());
					spikeAnimation.back()->init(spikes.back(), texProgram, 0);
					break;
				case 4: // HEAL POTION
					healPotion.push_back(glm::ivec2(i * TILE_X, j * TILE_Y));
					healPotionAnimation.push_back(new Activable());
					healPotionAnimation.back()->init(healPotion.back(), texProgram, 3);
					healPotionAnimation.back()->activate();
					break;
				case 5: // FALLING PLATE
					fallingPlates.push_back(glm::ivec2(i * TILE_X, j * TILE_Y));
					fallingPlatesAnimation.push_back(new Activable());
					fallingPlatesAnimation.back()->init(fallingPlates.back(), texProgram, 1);
					break;
				case 8: // BARRED DOOR
					door0 = glm::ivec2(i * TILE_X, j * TILE_Y);
					break;
				case 9: // OUTDOOR STAIRS
					door1 = glm::ivec2(i * TILE_X , j * TILE_Y);
					break;
				case 10:
					door2 = glm::ivec2(i * TILE_X, j * TILE_Y);
					break;
				case 11:
					windoor = glm::ivec2(i * TILE_X, j * TILE_Y);
				default:
					break;
			}
		}
	}
	fin.close();
}



