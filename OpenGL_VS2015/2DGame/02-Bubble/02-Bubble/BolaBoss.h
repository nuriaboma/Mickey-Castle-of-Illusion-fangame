#ifndef _BOLABOSS_INCLUDE
#define _BOLABOSS_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include <cstdlib>
#include <ctime> 

class BolaBoss
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const glm::vec2& direction, float speed);
	void update(int deltaTime);
	void render() const;

	void setStartPosition(const glm::ivec2& pos);
	void setTileMap(TileMap* tileMap);
	glm::ivec2 getSize() const;
	glm::ivec2 getPosition() const;

private:
	bool isThrown;
	glm::ivec2 tileMapDispl;
	glm::ivec2 posBola;
	glm::ivec2 startPos;
	glm::vec2 velocity;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
};

#endif
