#ifndef _SELECCIODIFICULTAT_INCLUDE
#define _SELECCIODIFICULTAT_INCLUDE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"

enum DificultatOption { PRACTICE, NORMAL };

class SeleccioDificultat
{

public:
	SeleccioDificultat();
	~SeleccioDificultat();

	void init();
	void update(int deltaTime);
	void render();
	void moveSelectionUp();
	void moveSelectionDown();
	int getSelectedOption() const;

private:
	void initShaders();


private:
	TileMap* map;
	glm::mat4 projection;
	ShaderProgram texProgram;
	int selectedOption;
	Sprite* practiceText;
	Sprite* normalText;
	Sprite* arrowText;
	Sprite* arrowText2;
	float mapHeight = 704;
	float mapWidth = 1024;

};

#endif

