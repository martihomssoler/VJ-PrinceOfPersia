#pragma once
class HealthGUI
{
public:
	HealthGUI();
	~HealthGUI();

	void init(const glm::ivec2 &pos, int points, ShaderProgram &shaderProgram);
	bool damage(int amount);
	bool cure(int amount);

private:
	Texture spritesheet;
	vector<Sprite*> lifebar;
	int health;
};

