#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init(string level);
	void update(int deltaTime);
	void eventHandler();
	void render();

private:
	void initShaders();
	void initEnemies(const string &enemiesFile);

	TileMap *backMap;
	TileMap *map;
	TileMap *wallMap;
	Player *player;

	HealthGUI *playerHealth;
	vector<Enemy> enemies;
	vector<HealthGUI> enemyLifebars;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	// idea per gestionar tots els events i accions que passen en una mateixa escena
	vector<int> events;

};


#endif // _SCENE_INCLUDE

