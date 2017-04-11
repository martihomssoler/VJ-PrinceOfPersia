#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

void Game::init(string level)
{
	bPlay = true;
	shift_key = false;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	scene.init(level);
}

bool Game::update(int deltaTime)
{
	scene.update(deltaTime);
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render();
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	if (key == 49) // 1 code
		init("level1");
	if (key == 50) // 2 code
		init("level02");
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::shiftKeyPressed() 
{
	shift_key = true;
}
void Game::shiftKeyReleased()
{
	shift_key = false;
}
void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

bool Game::getShiftKey() const
{
	return shift_key;
}





