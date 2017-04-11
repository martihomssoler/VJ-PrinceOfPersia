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


#define SCREEN_X 0
#define SCREEN_Y 0


#define INIT_PLAYER_X_TILES 1.5
#define INIT_PLAYER_Y_TILES 1

#define ENEMY_1 0
#define ENEMY_2 1

#define ENEMY 0
#define PRINCE 1

#define TILE_X 32
#define TILE_Y 64

#define MOVEMENT_300 150
#define ATTACK_300 295

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
	backMap = TileMap::createTileMap("levels/" + level + "Back.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map = TileMap::createTileMap("levels/" + level + "Ground.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	wallMap = TileMap::createTileMap("levels/" + level + "Wall.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileBackMap(backMap);
	player->setTileMap(map);
	player->setTileWallMap(wallMap);
	
	playerHealth = new HealthGUI();
	playerHealth->init(player->getPostion(), 3, texProgram, PRINCE);
	player->setHealthGUI(playerHealth);
	initEnemies("levels/" + level + "Enemies.txt");
	initActivables("levels/" + level + "Activables.txt");
	// vector d'events pendents per gestionar al update d'una escèna
	events = vector<int>(enemies.size() + 1, 0);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
	bShowEnemyLifebar = false;
}

void Scene::update(int deltaTime)
{
	
	currentTime += deltaTime;
	glm::ivec2 playerPos = player->getPostion();
	player->update(deltaTime,events[events.size()-1]);
		
	for (unsigned int i = 0; i < enemies.size(); ++i)
	{
		if (events[i] != -1 && events[events.size()-1] != -1) // l'enemic segueix viu i el personatge també
		{
			string action = "";
			int r = rand() % 300 + 1;
			glm::ivec2 enemyPos = enemies[i].getPosition();
			// Afegir la logica aplicada al enemics
			// ara per ara el codi dels enemics esta buid!!!
			// si events[i] és igual a (-1) significa que l'enemic ha mort
			if (r > MOVEMENT_300 && enemyPos.y + 12 >= playerPos.y && playerPos.y >= enemyPos.y - 12) // "mateixa" altura
			{
				if (enemyPos.x - TILE_X > playerPos.x && playerPos.x > enemyPos.x - 8 * TILE_X) // si el jugador esta entre 1 a 4 blocks de distància a l'ESQUERRA
				{
					action = "MOVE_LEFT";
					bShowEnemyLifebar = true;
				}
				else if (r > ATTACK_300 && playerPos.x >= enemyPos.x - TILE_X && enemyPos.x >= playerPos.x) // si el jugador esta entre 1 a 8 blocks de distància a l'ESQUERRA
				{
					action = "ATTACK_LEFT";
					bShowEnemyLifebar = true;
				}
				else if (playerPos.x < enemyPos.x + 8 * TILE_X && enemyPos.x + TILE_X < playerPos.x) // si el jugador esta entre 1 a 4 blocks de distància a la DRETA
				{
					action = "MOVE_RIGHT";
					bShowEnemyLifebar = true;
				}
				else if (r > ATTACK_300 && playerPos.x <= enemyPos.x + TILE_X && enemyPos.x <= playerPos.x) // si el jugador esta entre 1 a 8 blocks de distància a la DRETA
				{
					action = "ATTACK_RIGHT";
					bShowEnemyLifebar = true;
				}
				
			}
			else
			{
				action = "STAND";
			}

			enemies[i].update(deltaTime, action, events[i]);
			enemyLifebars[i]->update(deltaTime);
			enemyLifebars[i]->setPosition(enemies[i].getPosition());
		}
	}	
	
	eventHandler();
	/*for (unsigned int i = 0; i < forcePlates.size(); ++i){
		if (player->getPostion().x + 32 > forcePlates[i].x && player->getPostion().x + 32 < forcePlates[i].x + 64 && player->getPostion().y == forcePlates[i].y) {
			map->changeTile(forcePlates[i].x / TILE_X, forcePlates[i].y / TILE_Y, 1);
		}
		else if (player->getPostion().x + 32 <= forcePlates[i].x || player->getPostion().x + 32 >= forcePlates[i].x + 64) map->changeTile(forcePlates[i].x / TILE_X, forcePlates[i].y / TILE_Y, 4);
	}*/
	for (unsigned int i = 0; i < spikeAnimation.size(); ++i){
		if (player->getPostion().x + 32 > spikes[i].x && player->getPostion().x + 32 < spikes[i].x + 64 && player->getPostion().y <= spikes[i].y && spikes[i].y - player->getPostion().y <= TILE_Y*2) {
			spikeAnimation[i]->activate();
			if (player->getPostion().x + 32 > spikes[i].x + 16 && player->getPostion().x + 32 < spikes[i].x + 48 && player->getPostion().y == spikes[i].y && !player->isJumping()) {
				player->setPosition(spikes[i]);
				player->spikes();
			}
		}
		else if (player->getPostion().x + 32 <= spikes[i].x || player->getPostion().x + 32 >= spikes[i].x + 64) spikeAnimation[i]->deactivate();
		spikeAnimation[i]->update(deltaTime);
	}

	for (unsigned int i = 0; i < piercingTrapAnimation.size(); ++i){
		if (player->getPostion().y == piercingTraps[i].y) {
			if (piercingTrapAnimation[i]->isActive() && player->getPostion().x + 32 > piercingTraps[i].x && player->getPostion().x + 32 < piercingTraps[i].x + 64) {
				player->setPosition(glm::ivec2(piercingTraps[i].x -32, piercingTraps[i].y));
				player->slice();
				piercingTrapAnimation[i]->block();
			}
		}
		piercingTrapAnimation[i]->update(deltaTime);
	}

	for (unsigned int i = 0; i < fallingPlatesAnimation.size(); ++i){
		if (player->getPostion().x + 32 > fallingPlates[i].x && player->getPostion().x + 32 < fallingPlates[i].x + 64 && player->getPostion().y == fallingPlates[i].y) {
			map->changeTile(fallingPlates[i].x / TILE_X, fallingPlates[i].y / TILE_Y, 0);
			fallingPlatesAnimation[i]->activate();
		}
		fallingPlatesAnimation[i]->update(deltaTime);
	}
	playerHealth->update(deltaTime);
}

void Scene::eventHandler()
{
	glm::ivec2 playerPos = player->getPostion();
	int direction = player->getDirection();
	switch (events[events.size() - 1])
	{
		case 1:
			events[events.size() - 1] = 0;
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
			events[events.size() - 1] = 0;
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
			events[events.size() - 1] = 0;
			// E pressed MAYBE AT THE DOOR
			if ((playerPos.x >= door.x  && door.x + 64 >= playerPos.x) && (playerPos.y >= door.y - 5 && door.y + 5 >= playerPos.y))
			{
				player->enterDoor();				
			}
			break;
		case -1:
			// THE PLAYER IS DEAD
			break;
		default:
			break;
	}
	

	for (unsigned int i = 0; i < events.size() - 1; ++i) // des de 0 fins a events.size()-2 hi ha enemics, l'event del princep és el events.size()-1
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
			bShowEnemyLifebar = false;
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

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	player->render();

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
		if (bShowEnemyLifebar) enemyLifebars[i]->render();
	}

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f));
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	playerHealth->render();

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f));
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	gameOver->render();

	//glm::vec3 translation = glm::vec3(2 * TILE_X*INIT_PLAYER_X_TILES - player->getPostion().x, TILE_Y*INIT_PLAYER_Y_TILES - player->getPostion().y, 0);
	projection = glm::translate(glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f), getTranslationMap());
}

glm::vec3 Scene::getTranslationMap()
{
	int left = player->getPostion().x - SCREEN_WIDTH / 2;
	int up = player->getPostion().y - SCREEN_HEIGHT / 2;

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

	cout << left << " , " << up << endl;

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
				// si aux > maxEnemies és cert, l'enemic estarà mirant cap a l'esquerra (-1)
				// altrement si és falç, l'enemic estarà mirant cap a la dreta (1)
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

	potion.clear();
	piercingTraps.clear();
	piercingTrapAnimation.clear();
	spikes.clear();
	spikeAnimation.clear();
	fallingPlates.clear();
	fallingPlatesAnimation.clear();
	barredDoors.clear();
	
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
				case 1: //POTION
					potion.push_back(glm::ivec2(i * TILE_X, j * TILE_Y));
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
				case 4: // FORCE PLATE
					forcePlates.push_back(glm::ivec2(i * TILE_X, j * TILE_Y));
					break;
				case 5: // FALLING PLATE
					fallingPlates.push_back(glm::ivec2(i * TILE_X, j * TILE_Y));
					fallingPlatesAnimation.push_back(new Activable());
					fallingPlatesAnimation.back()->init(fallingPlates.back(), texProgram, 1);
					break;
				case 8: // BARRED DOOR
					barredDoors.push_back(glm::ivec2(i * TILE_X, j * TILE_Y));
					break;
				case 9: // OUTDOOR STAIRS
					door = glm::ivec2(i * TILE_X , j * TILE_Y);
					break;
				default:
					break;
			}
		}
	}
	fin.close();
}



