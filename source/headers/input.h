#ifndef input_h
#define input_h

#include <map>
#include <SDL2/SDL.h>

enum class InputResult
{
  SHUTDOWN,
  NONE
};

class Input {
public:
	/*
	 * void beginNewFrame
	 * Called at beginning of each new frame to reset processed key presses
	 */
  InputResult beginNewFrame(SDL_Event* event);
	
	/*
	 * void keyUpEvent
	 * Called when a key has been released
	 */
	void keyUpEvent(const SDL_Event& event);
	
	/*
	 * void keyDownEvent
	 * Called when a key has been pressed
	 */
	void keyDownEvent(const SDL_Event& event);
	
	/*
	 * void wasKeyPressed
	 * Check if certain key was pressed during current frame
	 */
	bool wasKeyPressed(SDL_Scancode key);
	
	/*
	 * void wasKeyReleased
	 * Check if certain key was released during current frame
	 */
	bool wasKeyReleased(SDL_Scancode key);
	
	/*
	 * void isKeyHeld
	 * Check if certain key was held during current frame
	 */
	bool isKeyHeld(SDL_Scancode key);
	
private:
	std::map<SDL_Scancode,bool> _heldKeys;
	std::map<SDL_Scancode,bool> _pressedKeys;
	std::map<SDL_Scancode,bool> _releasedKeys;
};

#endif /* input_h */
