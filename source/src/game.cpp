#include <iostream>

#include "game.h"
#include "graphics.h"
#include "input.h"

#include <algorithm>

namespace {
	const int FPS = 50;
	const int MAX_FRAME_TIME = 5 * 1000 / FPS;
}

Game::Game() : _gameState(GameState::PLAYER) {
	SDL_Init(SDL_INIT_EVERYTHING);
	this->gameLoop();
}

Game::~Game() {
	
}

void Game::gameLoop() {
	Graphics graphics;
	Input input;
	SDL_Event event;
	
	this->_level = Level("Map 1", Vector2(100,100), graphics);
	this->_player = Player(graphics, this->_level.getPlayerSpawnpoint());
	
	int LAST_UPDATE_TIME = SDL_GetTicks();
	//Start Game Loop
	while(true) {
    if (input.beginNewFrame(&event) == InputResult::SHUTDOWN) return;
    processInput(input);
		
		const int CURR_TIME_MS = SDL_GetTicks();
		int ELAP_TIME_MS = CURR_TIME_MS - LAST_UPDATE_TIME;
		this->update(std::min(ELAP_TIME_MS, MAX_FRAME_TIME));
		LAST_UPDATE_TIME = CURR_TIME_MS;
		
		//std::cout << "playing";
		
		this->draw(graphics);
	}
}

void Game::draw(Graphics &graphics) {
	graphics.clear();
	this->_level.draw(graphics);
	this->_player.draw(graphics);
	graphics.flip();
}

void Game::update(float elapsedTime) {
	this->_player.update(elapsedTime);
	this->_level.update(elapsedTime);
	

#ifdef COLLISION
	std::vector<Rectangle> others;
	if((others = this->_level.checkTileCollisions(this->_player.getBoundingBox())).size() > 0) {
		//Player collided with atleast one tile
		this->_player.handleTileCollisions(others);
	}

	//Check slopes
	std::vector<Slope> otherSlopes;
	if ((otherSlopes = this->_level.checkSlopeCollisions(this->_player.getBoundingBox())).size() > 0) {
		this->_player.handleSlopeCollisions(otherSlopes);
	}
#endif
}

void Game::processInput(Input& input)
{
  if (_gameState == GameState::PLAYER)
  {
    _player.HandleInput(input);
  }
}

