#ifndef _ROCK_INCLUDE
#define _ROCK_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include <cstdlib>
#include <ctime> 

class Rock
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, const int objectType);
	void update(int deltaTime);
	void render() const;

	void setPosition(const glm::ivec2 &pos);
	void setTileMap(TileMap *tileMap);
	glm::ivec2 getSize() const;
	glm::ivec2 getPosition() const;

	bool isPickedUp() const;
	void pickUp();
	void throwRock(const glm::vec2 &direction, float speed);
	void updatePlayerPosition(glm::vec2 position, int faceDir);
	bool isDeleted();
	bool thrown();
	int getType();
	void destroyChest();

private:
	bool deleteRock;
	bool pickedUp;
	bool isThrown;
	int playerFacingDirection;
	int type; //0 dest / 1 noDest/ 2 chest / 3cake /4 coin //5 gema
	glm::vec2 posPlayer;
	glm::ivec2 tileMapDispl;
	glm::ivec2 posRock; 
	glm::vec2 velocity;
	glm::vec2 falling;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
};

#endif
