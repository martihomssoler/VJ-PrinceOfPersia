#pragma once
class HealthGUI
{
public:
	HealthGUI();
	~HealthGUI();

	void init(const glm::ivec2 &pos, int points, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	bool damage(int amount);
	void cure();

private:
	Texture spritesheet;
	vector<Sprite*> lifebar;
	vector<bool> lifebar_values;
	int health;
	glm::ivec2 tileMapDispl, posLifebar;
};

