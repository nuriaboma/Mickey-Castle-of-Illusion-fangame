#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Sprite.h"

enum MenuOption { PLAY, INSTRUCTIONS, CREDITS };

class Menu
{

public:
	Menu();
	~Menu();

	void init();
	void update(int deltaTime);
	void render();
	void moveSelectionUp();
	void moveSelectionDown();
	int getSelectedOption() const;

private:
	void initShaders();


private:
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	TileMap *map;
	float mapHeight = 704;
	float mapWidth = 1024;
	int selectedOption;
	Sprite *playOption;
	Sprite *instructionsOption;
	Sprite *creditsOption;

};


#endif