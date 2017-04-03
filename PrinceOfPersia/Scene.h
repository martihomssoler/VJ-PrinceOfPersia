#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "Activable.h"


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
	void initActivables(const string &activablesFile);

	TileMap *backMap;
	TileMap *map;
	TileMap *wallMap;
	Player *player;

	HealthGUI* playerHealth;
	vector<Enemy> enemies;
	
	vector<HealthGUI*> enemyLifebars;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	bool bShowEnemyLifebar;

	// idea per gestionar tots els events i accions que passen en una mateixa escena
	vector<int> events;

	// traps and activables
	vector<glm::ivec2> potion;
	vector<glm::ivec2> piercingTraps;
	vector<glm::ivec2> fallingPlates;
	vector<glm::ivec2> forcePlates;
	vector<glm::ivec2> barredDoors;
	vector<glm::ivec2> spikes;
	vector<Activable*> spikeAnimation;
	Texture spikeSpritesheet;
	glm::ivec2 door;
};


#endif // _SCENE_INCLUDE

