#ifndef _INSTRUCTIONS_INCLUDE
#define _INSTRUCTIONS_INCLUDE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Sprite.h"

class Instructions
{

public:
	Instructions();
	~Instructions();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();


private:
	ShaderProgram texProgram;
	glm::mat4 projection;
	TileMap* map;
	float mapHeight = 704;
	float mapWidth = 1024;
	Sprite* jumpText;
	Sprite* crouchText;
	Sprite* leftText;
	Sprite* rightText;
	Sprite* pickupText;
	Sprite* throwText;
	Sprite* basedOnTextText;
	Sprite* HText;
	Sprite* GText;
	Sprite* exit;


};


#endif