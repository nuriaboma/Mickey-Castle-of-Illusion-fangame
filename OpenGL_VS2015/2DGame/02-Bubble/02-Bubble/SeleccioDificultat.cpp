#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "SeleccioDificultat.h"
#include "Game.h"
//#include <irrKlang.h>

#define SCREEN_X 0
#define SCREEN_Y 0


SeleccioDificultat::SeleccioDificultat()
{
	map = NULL;
}

SeleccioDificultat::~SeleccioDificultat()
{
	if (map != NULL)
		delete map;

}

void SeleccioDificultat::init()
{
	initShaders();

	map = TileMap::createTileMap("levels/selecciodif.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	//TEXT:
	Texture* practiceTexture = new Texture();
	practiceTexture->loadFromFile("images/text/practice.png", TEXTURE_PIXEL_FORMAT_RGBA);
	practiceText = Sprite::createSprite(glm::vec2(306, 64), glm::vec2(1, 1), practiceTexture, &texProgram);
	practiceText->setPosition(glm::vec2(396.5, 170));

	Texture* normalTexture = new Texture();
	normalTexture->loadFromFile("images/text/normal.png", TEXTURE_PIXEL_FORMAT_RGBA);
	normalText = Sprite::createSprite(glm::vec2(264, 64), glm::vec2(1, 1), normalTexture, &texProgram);
	normalText->setPosition(glm::vec2(396.5, 300));

	Texture* arrowTexture = new Texture();
	arrowTexture->loadFromFile("images/text/fletxa.png", TEXTURE_PIXEL_FORMAT_RGBA);
	arrowText = Sprite::createSprite(glm::vec2(51, 68), glm::vec2(1, 1), arrowTexture, &texProgram);
	arrowText->setPosition(glm::vec2(320, 170));

	arrowText2 = Sprite::createSprite(glm::vec2(51, 68), glm::vec2(1, 1), arrowTexture, &texProgram);
	arrowText2->setPosition(glm::vec2(320, 300));
	selectedOption = 0;

}

void SeleccioDificultat::update(int deltaTime)
{

	practiceText->update(deltaTime);
	normalText->update(deltaTime);
	arrowText->update(deltaTime);
	arrowText2->update(deltaTime);

}

void SeleccioDificultat::render()
{
	glm::mat4 modelview = glm::mat4(1.0f);
	texProgram.use();

	glm::mat4 projection = glm::ortho(0.0f, mapWidth, mapHeight, 0.0f);
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniformMatrix4f("modelview", modelview);

	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	normalText->render();
	practiceText->render();
	if (selectedOption == 0) {
		arrowText->render();
	}

	else if (selectedOption == 1){
		arrowText2->render();
	}

}

void SeleccioDificultat::moveSelectionUp() {
	selectedOption = (selectedOption + 1) % 2;  // Mou amunt cíclicament
}

void SeleccioDificultat::moveSelectionDown() {
	selectedOption = (selectedOption + 1) % 2;  // Mou avall cíclicament
}

int SeleccioDificultat::getSelectedOption() const {
	return selectedOption;
}


void SeleccioDificultat::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		std::cout << "Error en el Shader de Vertex" << std::endl;
		std::cout << "" << vShader.log() << std::endl << std::endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		std::cout << "Error en el Shader de Fragment" << std::endl;
		std::cout << "" << fShader.log() << std::endl << std::endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		std::cout << "Error en l'Enllaç del Shader" << std::endl;
		std::cout << "" << texProgram.log() << std::endl << std::endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}
