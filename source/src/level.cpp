//
//  level.cpp
//  cavestory_demo
//
//  Created by Christian Jarani on 10/23/19.
//  Copyright © 2019 majula. All rights reserved.
//

#include <stdio.h>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>

#include "graphics.h"
#include "level.h"
#include <../external/tinyxml2.h>
#include "utils.h"

using namespace tinyxml2;

Level::Level() {}

Level::Level(std::string mapName, Vector2 spawnPoint, Graphics &graphics) :
	_mapName(mapName),
	_spawnPoint(spawnPoint),
	_size(Vector2(0,0))
{
	this->loadMap(mapName, graphics);
}

Level::~Level() {

}

void Level::loadMap(std::string mapName, Graphics &graphics) {
	//Parse the .tmx file
	XMLDocument doc;
	std::stringstream ss;
	ss << "./content/maps/" << mapName << ".tmx"; // Pass in Map 1, we get /content/maps/Map 1.tmx
	
	printf("%s\n",ss.str().c_str());
	
  auto levelFilePath = Utils::CorrectPathForSystem(ss.str().c_str());
	doc.LoadFile(levelFilePath.c_str());
	
	XMLElement* mapNode = doc.FirstChildElement("map"); // first child element within .tmx
	
	//Get width and height of the whole map and store it in _size
	int width, height;
	mapNode->QueryIntAttribute("width", &width);
	mapNode->QueryIntAttribute("height", &height);
	this->_size = Vector2(width,height);
	
	//Get the width and height of tiles and store in _tileSize
	int tileWidth, tileHeight;
	mapNode->QueryIntAttribute("tilewidth", &tileWidth);
	mapNode->QueryIntAttribute("tileheight", &tileHeight);
	this->_tileSize = Vector2(tileWidth,tileHeight);
	
	// Tileset element in map file only contains GID and reference to actual tileset file
	XMLElement* pTileset = mapNode->FirstChildElement("tileset");
	if(pTileset != NULL) {
		while(pTileset) {
			int firstgid;
			pTileset->QueryIntAttribute("firstgid", &firstgid);	// Get GID from Map n.tmx tileset element
			
			//EXTRA STEP NOT FROM TUTORIAL: Get tileset file, which contains all tileset attributes
			XMLDocument ts;
			const char* tilesetPath;
			pTileset->QueryStringAttribute("source", &tilesetPath);
      auto tilesetPathStr = Utils::CorrectPathForSystem(tilesetPath);

			ts.LoadFile(tilesetPathStr.c_str());
		
			//Get image source for tileset file
			XMLElement* tilesetNode = ts.FirstChildElement("tileset");
			const char* source = tilesetNode->FirstChildElement("image")->Attribute("source");
     

      
			std::stringstream ss;
			ss << "./content/tilesets/" << source;
			
      std::string tlsLocation = Utils::CorrectPathForSystem(ss.str().c_str());

			SDL_Texture* tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(tlsLocation));
			this->_tilesets.push_back(Tileset(tex, firstgid));
			
			pTileset = pTileset->NextSiblingElement("tileset");
		}
	}
	
	//Loading the layer elements
	XMLElement* pLayer = mapNode->FirstChildElement("layer");
	if(pLayer != NULL) {
		while(pLayer){
			
			//Loading the data elements
			XMLElement* pData = pLayer->FirstChildElement("data");
			while(pData != NULL) {
				while(pData) {
					
					//Loading the tile elements
					XMLElement* pTile = pData->FirstChildElement("tile");
					if(pTile != NULL) {
						int tileCounter = 0;
						while(pTile) {
							//Build individual tile here
							//If gid is 0, no tile should be drawn. Continue loop
							if(pTile->IntAttribute("gid") == 0) {
								tileCounter++;
								if(pTile->NextSiblingElement("tile")){
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else {
									break;
								}
							}
							
							//get the tileset for this specific gid
							int gid = pTile->IntAttribute("gid");
							Tileset tls;
							for(int i = 0; i < this->_tilesets.size(); i++) {
								if(this->_tilesets[i].FirstGid <= gid) {
									//This is the tileset we want
									tls = this->_tilesets.at(i);
									break;
								}
							}
							
							if(tls.FirstGid == -1) {
								//No tileset was found for this gid
								tileCounter++;
								if(pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("title");
									continue;
								}
								else {
									break;
								}
							}
							
							//Get the position of the tile in the level
							int xx = 0, yy = 0;
							xx = tileCounter % width;
							xx *= tileWidth;
							yy += tileHeight * (tileCounter / width);
							Vector2 finalTilePosition = Vector2(xx, yy);
							
							//Calculate the position of the tile in the tileset
							int tilesetWidth, tilesetHeight;
							SDL_QueryTexture(tls.Texture, NULL, NULL, &tilesetWidth, &tilesetHeight);
							
							int tsxx = gid % (tilesetWidth / tileWidth) - 1;
							tsxx *= tileWidth;
							
							int tsyy = 0;
							int amt = (gid / (tilesetWidth / tileWidth));
							tsyy = tileHeight * amt;
							Vector2 finalTilesetPosition = Vector2(tsxx, tsyy);
							
							//Build the actual tile and add it to the level's tile list
							Tile tile(tls.Texture, Vector2(tileHeight, tileHeight), finalTilesetPosition, finalTilePosition);
							this->_tileList.push_back(tile);
							tileCounter++;
							
							pTile = pTile->NextSiblingElement("tile");
						}
					}
					
					pData = pData->NextSiblingElement("data");
				}
			}
			
			pLayer = pLayer->NextSiblingElement("layer");
		}
	}
	
	//Parse out the collisions
	XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup");
	if(pObjectGroup != NULL) {
		while(pObjectGroup) {
			const char* name = pObjectGroup->Attribute("name");
			std::stringstream ss;
			ss << name;
			
			std::cout << ss.str() << "\n";
			
			//Collisions Object Group
			if(ss.str() == "collisions") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if(pObject != NULL) {
					while(pObject) {
						float x, y, width, height;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height");
						this->_collisionRects.push_back(Rectangle(
							  std::ceil(x) * globals::SPRITE_SCALE,
							  std::ceil(y) * globals::SPRITE_SCALE,
							  std::ceil(width) * globals::SPRITE_SCALE,
							  std::ceil(height) * globals::SPRITE_SCALE
						));
						
						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			
			//Slopes Object Group
			else if(ss.str() == "slopes") {
				//std::cout << "enter slopes cond.";
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if(pObjectGroup != NULL) {
					while(pObject) {
						std::vector<Vector2> points;
						Vector2 p1;
						p1 = Vector2(std::ceil(pObject->FloatAttribute("x")), std::ceil(pObject->FloatAttribute("y")));
						
						XMLElement* pPolyline = pObject->FirstChildElement("polyline");
						if(pPolyline != NULL) {
							std::vector<std::string> pairs;
							const char* pointString = pPolyline->Attribute("points");
							std::stringstream ss;
							ss << pointString;
							Utils::split(ss.str(), pairs, ' ');
							
							//Now we have each of the pairs, loop thru the list of pairs,
							// split them into Vector2's, and then store them in our points vector
							for(int i = 0; i < pairs.size(); i++) {
								std::vector<std::string> ps;
								Utils::split(pairs.at(i), ps, ',');
								points.push_back(Vector2(std::stoi(ps.at(0)), std::stoi(ps.at(1))));
							}
						}
						
						for(int i = 0; i < points.size(); i += 2) {
							// Create vectors with points retrieve from polyline object
							this->_slopes.push_back(Slope(
									Vector2((p1.x + points.at(i < 2 ? i : i-1).x) * globals::SPRITE_SCALE,
											(p1.y + points.at(i < 2 ? i : i-1).y) * globals::SPRITE_SCALE),
								    Vector2((p1.x + points.at(i < 2 ? i+1 : i).x) * globals::SPRITE_SCALE,
										    (p1.y + points.at(i < 2 ? i+1 : i).y) * globals::SPRITE_SCALE)
						    ));
						}
//						std::cout << "polyline pts: " << points.size() << "\n";
//						std::cout << "vectors: " << _slopes.size() << "\n";
						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			
			//Spawnpoints Object Group
			else if(ss.str() == "spawnpoints") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if(pObjectGroup != NULL) {
					while(pObject) {
						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");
						const char* name = pObject->Attribute("name");
						std::stringstream ss;
						ss << name;
						if(ss.str() == "player") {
							this->_spawnPoint = Vector2(std::ceil(x) * globals::SPRITE_SCALE, std::ceil(y) * globals::SPRITE_SCALE);
						}
						
						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			
			pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
		}
	}
}

void Level::update(int elapsedTime) {}

void Level::draw(Graphics &graphics) {
	for(int i = 0; i < this->_tileList.size(); i++) {
		this->_tileList.at(i).draw(graphics);
	}
}

std::vector<Rectangle> Level::checkTileCollisions(const Rectangle &other) {
	std::vector<Rectangle> others;
	for(int i = 0; i < this->_collisionRects.size(); i++) {
		if(this->_collisionRects.at(i).collidesWith(other)) {
			others.push_back(this->_collisionRects.at(i));
		}
	}
	return others;
}

std::vector<Slope> Level::checkSlopeCollisions(const Rectangle &other) {
	std::vector<Slope> others;
	for(int i = 0; i < this->_slopes.size(); i++) {
		if(this->_slopes.at(i).collidesWith(other)) {
			others.push_back(this->_slopes.at(i));
		}
	}
	return others;
}

const Vector2 Level::getPlayerSpawnpoint() const {
	return this->_spawnPoint;
}
