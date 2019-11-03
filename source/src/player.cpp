#include "graphics.h"
#include "player.h"
#include "utils.h"
#include "input.h"

namespace player_constants {
	const float WALK_SPEED = 0.2f;
	const float JUMP_SPEED = 0.7f;
	
	const char* CHAR_IMG = "./content/sprites/MyChar.png";
	const float GRAVITY = 0.002f;
	const float GRAVITY_CAP = 0.8f;
}

Player::Player() {}

Player::Player(Graphics &graphics, Vector2 spawnPoint) :
	AnimatedSprite(graphics, Utils::CorrectPathForSystem(player_constants::CHAR_IMG), 0, 0, 16, 16, spawnPoint.x, spawnPoint.y, 100),
	_dx(0),
	_dy(0),
	_facing(RIGHT),
	_grounded(false)
{
	graphics.loadImage(Utils::CorrectPathForSystem(player_constants::CHAR_IMG));
	this->setupAnimations();
	this->playAnimation("RunRight");
}

void Player::setupAnimations() {
	this->addAnimation(1, 0, 0, "IdleLeft", 16, 16, Vector2(0,0));
	this->addAnimation(1, 0, 16, "IdleRight", 16, 16, Vector2(0,0));
	this->addAnimation(3, 0, 0, "RunLeft", 16, 16, Vector2(0,0));
	this->addAnimation(3, 0, 16, "RunRight", 16, 16, Vector2(0,0));
}

void Player::animationDone(std::string currentAnimation) {}

const float Player::getX() const {
	return this->_x;
}

const float Player::getY() const {
	return this->_y;
}

void Player::moveLeft() {
	this->_dx = -player_constants::WALK_SPEED;
	this->playAnimation("RunLeft");
	this->_facing = LEFT;
}

void Player::moveRight() {
	this->_dx = player_constants::WALK_SPEED;
	this->playAnimation("RunRight");
	this->_facing = RIGHT;
}

void Player::moveUp() {
  this->_dy = -player_constants::WALK_SPEED;
  this->playAnimation(this->_facing == RIGHT ? "RunRight" : "RunLeft");
}

void Player::moveDown() {
  this->_dy = player_constants::WALK_SPEED;
  this->playAnimation(this->_facing == RIGHT ? "RunRight" : "RunLeft");
}

void Player::stopMoving() {
	this->_dx = 0.0f;
  _dy = 0.0f;
	this->playAnimation(this->_facing == RIGHT ? "IdleRight" : "IdleLeft");
}

void Player::jump() {
	
	if(this->_grounded) {
		this->_dy = 0;
		this->_dy -= player_constants::JUMP_SPEED;
		this->_grounded = false;
	}
}

/* void handleTileCollisions
 * Handle all player collisions with tiles
 */
void Player::handleTileCollisions(std::vector<Rectangle> &others) {
	//Figure out what side the collisions is happening on
	for(int i = 0; i < others.size(); i++) {
		sides::Side collisionSide = Sprite::getCollisionSide(others.at(i));
		if(collisionSide != sides::NONE) {
			switch(collisionSide) {
				case sides::TOP:
					this->_dy = 0;
					this->_y = others.at(i).getBottom() + 1;
					if(this->_grounded) {
						this->stopMoving();
						this->_dx = 0;
						this->_x -= this->_facing == RIGHT ? 0.5f : -0.5f;
					}
					break;
					
				case sides::BOTTOM:
					this->_y = others.at(i).getTop() - this->_boundingBox.getHeight() - 1;
					this->_dy = 0;
					this->_grounded = true;
					break;
				
				case sides::LEFT:
					this->_x = others.at(i).getRight() + 1;
					break;
				
				case sides::RIGHT:
					this->_x = others.at(i).getLeft() - this->_boundingBox.getWidth() - 1;
					break;
					
				case sides::NONE:
					break;
			}
		}
	}
}

void Player::handleSlopeCollisions(std::vector<Slope> &others) {
	for(int i = 0; i < others.size(); i++) {
		//Calculate where on the slope the player's bottom center is touching
		//and use y=mx+b to figure out the y position to place him at
		//First calculate "b" (slope intercept) using one of the points (b=y-mx)
		int b = ((others.at(i).getP1().y) - (others.at(i).getSlope() * fabs(others.at(i).getP1().x)));
		
		//Get player's center x
		int centerX = this->_boundingBox.getCenterX();
		
		//Pass centerX into the y=mx+b to find y
		int newY = (others.at(i).getSlope() * centerX) + b - 8; // 8 is a temp fix
		
		//Re-position the player to the correct "y"
		if(this->_grounded) {
			this->_y = newY - this->_boundingBox.getHeight();
			this->_grounded = true;
		}
	}
}

void Player::update(float elapsedTime) {
#ifdef GRAVITY
  //Apply gravity
  if (this->_dy <= player_constants::GRAVITY_CAP) {
    this->_dy += player_constants::GRAVITY * elapsedTime;
  }
#endif
	
	//Move by dx
	this->_x += this->_dx * elapsedTime; // Move bby a certain amount based off your frame rate
	//Move by dy
	this->_y += this->_dy *elapsedTime;
	
	AnimatedSprite::update(elapsedTime);
}

void Player::draw(Graphics &graphics) {
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}

void Player::HandleInput(Input& input)
{
  if (input.isKeyHeld(SDL_SCANCODE_LEFT)) {
    moveLeft();
  }
  else if (input.isKeyHeld(SDL_SCANCODE_RIGHT)) {
    moveRight();
  }

  if (input.isKeyHeld(SDL_SCANCODE_UP)) {
    moveUp();
  }
  else if (input.isKeyHeld(SDL_SCANCODE_DOWN)) {
    moveDown();
  }

  if (input.wasKeyPressed(SDL_SCANCODE_Z) == true) {
    jump();
  }

  if (!input.isKeyHeld(SDL_SCANCODE_LEFT) && !input.isKeyHeld(SDL_SCANCODE_RIGHT)
    && !input.isKeyHeld(SDL_SCANCODE_UP) && !input.isKeyHeld(SDL_SCANCODE_DOWN)) {
    stopMoving();
  }
}
