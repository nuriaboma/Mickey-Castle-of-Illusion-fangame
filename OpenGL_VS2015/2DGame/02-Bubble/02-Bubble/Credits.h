#ifndef _CREDITS_INCLUDE
#define _CREDITS_INCLUDE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Sprite.h"

class Credits
{

public:
	Credits();
	~Credits();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();


private:
	ShaderProgram texProgram;
	glm::mat4 projection;
	TileMap *map;
	float mapHeight = 704;
	float mapWidth = 1024;
	Sprite* projectByText;
	Sprite* basedOnText;
	Sprite *exit;


};


#endif