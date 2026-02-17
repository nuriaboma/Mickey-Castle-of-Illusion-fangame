#ifndef _BOSS_INCLUDE
#define _BOSS_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include <cstdlib>
#include <ctime> 

class Boss
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render() const;

	void setPosition(const glm::ivec2& pos);
	void setTileMap(TileMap* tileMap);
	glm::ivec2 getSize() const;
	glm::ivec2 getPosition() const;
	void restarVida();
	int getVides();

private:
	bool isThrown;
	glm::ivec2 tileMapDispl;
	glm::ivec2 posBoss;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	int vides;
	int cooldownTime;
	int lastHitTime;
};

#endif
