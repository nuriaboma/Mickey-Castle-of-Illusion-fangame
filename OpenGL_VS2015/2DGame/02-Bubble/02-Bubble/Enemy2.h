#ifndef _ENEMY2_INCLUDE
#define _ENEMY2_INCLUDE

#include <glm/glm.hpp>
#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"

class Enemy2
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setPosition(const glm::vec2 &pos);
	void setTileMap(TileMap *tileMap);
	glm::vec2 getSize();
	glm::vec2 getPosition();
	void Killed();
	bool isAlive();

private:
	glm::ivec2 tileMapDispl, posEnemy;
	int startY;
	glm::vec2 fallVelocity;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	bool killedbyplayer;
	bool dead;

};

#endif
