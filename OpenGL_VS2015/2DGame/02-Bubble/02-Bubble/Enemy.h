#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE

#include <glm/glm.hpp>
#include "Sprite.h"
#include "TileMap.h"
#include <iostream>

class Enemy
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setPosition(const glm::vec2 &pos);
	void setTileMap(TileMap *tileMap);
	void detectPlayer(const glm::vec2 &playerPosition);
	glm::vec2 getSize();
	glm::vec2 getPosition();

private:
	bool isFalling;
	bool isAscending;
	bool isWaiting; 
	bool isAbove;
	glm::ivec2 tileMapDispl, posEnemy;
	int startY;
	glm::vec2 fallVelocity;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	int waitTime; 
	int waitTimer; // Temporitzador per al temps d'espera
	int chainLength; // Longitud de la cadena


};

#endif
