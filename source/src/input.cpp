#include <stdio.h>

#include "input.h"

/*  Input Class
 - Keeps track of keyboard state
 */

InputResult Input::beginNewFrame(SDL_Event* event) {
	this->_pressedKeys.clear();
	this->_releasedKeys.clear();

  //get new key states
  if (SDL_PollEvent(event)) {
    if (event->type == SDL_KEYDOWN) {
      if (event->key.repeat == 0) {
        keyDownEvent(*event);
      }
    }
    else if (event->type == SDL_KEYUP) {
      keyUpEvent(*event);
    }
    else if (event->type == SDL_QUIT) {
      return InputResult::SHUTDOWN;
    }
  }

  if (wasKeyPressed(SDL_SCANCODE_ESCAPE))
    return InputResult::SHUTDOWN;
  return InputResult::NONE;
}

void Input::keyDownEvent(const SDL_Event& event) {
	this->_pressedKeys[event.key.keysym.scancode] = true;
	this->_heldKeys[event.key.keysym.scancode] = true;
}

void Input::keyUpEvent(const SDL_Event& event) {
	this->_releasedKeys[event.key.keysym.scancode] = true;
	this->_heldKeys[event.key.keysym.scancode] = false;
}

bool Input::wasKeyPressed(SDL_Scancode key) {
	return this->_pressedKeys[key];
}

bool Input::wasKeyReleased(SDL_Scancode key) {
	return this->_releasedKeys[key];
}

bool Input::isKeyHeld(SDL_Scancode key) {
	return this->_heldKeys[key];
}
