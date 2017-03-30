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
	void render();

private:
	void initShaders();
	void initEnemies(const string &enemiesFile);

	TileMap *backMap;
	TileMap *map;
	TileMap *wallMap;
	Player *player;
<<<<<<< HEAD
	HealthGUI *playerHealth;
=======
	vector<Enemy> enemies;
>>>>>>> 1b3f2a62b5ace474965e6fd5ee7db936310fba16
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

};


#endif // _SCENE_INCLUDE

