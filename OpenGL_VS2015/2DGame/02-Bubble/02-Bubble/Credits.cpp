#include <iostream>
#include "Credits.h"
#include "Game.h"

#define SCREEN_X 0
#define SCREEN_Y 0

Credits::Credits() {
	map = NULL;
}

Credits::~Credits() {
	if (map != NULL)
		delete map;
}


void Credits::init() {

	initShaders();
	map = TileMap::createTileMap("levels/intro.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	Texture* projectBy = new Texture();
	projectBy->loadFromFile("images/text/projectby.png", TEXTURE_PIXEL_FORMAT_RGBA);
	projectByText = Sprite::createSprite(glm::vec2(910, 124), glm::vec2(1, 1), projectBy, &texProgram);
	projectByText->setPosition(glm::vec2(50, 180));

	Texture* basedOnTexture = new Texture();
	basedOnTexture->loadFromFile("images/text/basedon.png", TEXTURE_PIXEL_FORMAT_RGBA);
	basedOnText = Sprite::createSprite(glm::vec2(892, 49), glm::vec2(1, 1), basedOnTexture, &texProgram);
	basedOnText->setPosition(glm::vec2(59, 380));

	Texture* exitTexture = new Texture();
	exitTexture->loadFromFile("images/text/exit.png", TEXTURE_PIXEL_FORMAT_RGBA);
	exit = Sprite::createSprite(glm::vec2(75, 35), glm::vec2(1, 1), exitTexture, &texProgram);
	exit->setPosition(glm::vec2(474.5, 530));

}


void Credits::update(int deltaTime) {
	projectByText->update(deltaTime);
	basedOnText->update(deltaTime);
	exit->update(deltaTime);
}

void Credits::render() {

	glm::mat4 modelview = glm::mat4(1.0f);
	texProgram.use();

	glm::mat4 projection = glm::ortho(0.0f, mapWidth, mapHeight, 0.0f);
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniformMatrix4f("modelview", modelview);

	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	projectByText->render();
	basedOnText->render();
	
	texProgram.setUniform4f("color", 1.0f, 1.0f, 0.0f, 1.0f);
	exit->render();

}


void Credits::initShaders()
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
