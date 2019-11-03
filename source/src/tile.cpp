#include "graphics.h"
#include "tile.h"
#include "utils.h"

#define ISO_DRAW_TEST

Tile::Tile() {}

Tile::Tile(SDL_Texture* tileset, Vector2 size, Vector2 tilesetPosition, Vector2 position) :
	_tileset(tileset),
	_size(size),
	_tilesetPosition(tilesetPosition),
	_position(Vector2(position.x * globals::SPRITE_SCALE, position.y * globals::SPRITE_SCALE))
{}

void Tile::update(int elapsedTime) {}

void Tile::draw(Graphics &graphics) {

#ifdef ISO_DRAW_TEST
  auto pos = Utils::Conver2DToIso(this->_position.x, this->_position.y);
	SDL_Rect destRect = { pos.X(), pos.Y(), static_cast<int>(this->_size.x * globals::SPRITE_SCALE), static_cast<int>(this->_size.y * globals::SPRITE_SCALE) };
	SDL_Rect sourceRect = { this->_tilesetPosition.x, this->_tilesetPosition.y, this->_size.x, this->_size.y };
#else
  SDL_Rect destRect = { this->_position.x, this->_position.y, static_cast<int>(this->_size.x * globals::SPRITE_SCALE), static_cast<int>(this->_size.y * globals::SPRITE_SCALE) };
  SDL_Rect sourceRect = { this->_tilesetPosition.x, this->_tilesetPosition.y, this->_size.x, this->_size.y };
#endif
	
	graphics.blitSurface(this->_tileset, &sourceRect, &destRect);
}
