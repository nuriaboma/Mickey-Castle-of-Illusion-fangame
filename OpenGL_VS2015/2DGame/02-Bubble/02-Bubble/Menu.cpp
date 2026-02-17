#include <iostream>
#include "Menu.h"
#include "Game.h"

#define SCREEN_X 0
#define SCREEN_Y 0

Menu::Menu() {
	map = NULL;
}

Menu::~Menu() {
	if (map != NULL)
		delete map;
}


void Menu::init() {

	initShaders();
	map = TileMap::createTileMap("levels/intro.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	Texture* playTexture = new Texture();
	playTexture->loadFromFile("images/text/play.png", TEXTURE_PIXEL_FORMAT_RGBA);
	playOption = Sprite::createSprite(glm::vec2(200, 50), glm::vec2(1, 1), playTexture, &texProgram);
	playOption->setPosition(glm::vec2(400, 150));

	Texture* instructionsTexture = new Texture();
	instructionsTexture->loadFromFile("images/text/instru.png", TEXTURE_PIXEL_FORMAT_RGBA);
	instructionsOption = Sprite::createSprite(glm::vec2(611, 50), glm::vec2(1, 1), instructionsTexture, &texProgram);
	instructionsOption->setPosition(glm::vec2(195, 300));

	Texture* creditsTexture = new Texture();
	creditsTexture->loadFromFile("images/text/credits.png", TEXTURE_PIXEL_FORMAT_RGBA);
	creditsOption = Sprite::createSprite(glm::vec2(340, 50), glm::vec2(1, 1), creditsTexture, &texProgram);
	creditsOption->setPosition(glm::vec2(330, 450));

	selectedOption = 0;  // Comença seleccionant "Play"

}


void Menu::update(int deltaTime) {
	//currentTime += deltaTime;
	playOption->update(deltaTime);
	instructionsOption->update(deltaTime);
	creditsOption->update(deltaTime);
}

void Menu::render() {

	glm::mat4 modelview = glm::mat4(1.0f);
	texProgram.use();

	// Adjust the projection to the screen size
	glm::mat4 projection = glm::ortho(0.0f, mapWidth, mapHeight, 0.0f);
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniformMatrix4f("modelview", modelview);

	// Bind the texture and VAO, then draw the quad
	//texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	//modelview = glm::mat4(1.0f);
	//texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();


	if (selectedOption == 0) {
		texProgram.setUniform4f("color", 1.0f, 1.0f, 0.0f, 1.0f);  // Groc per "Play"
		playOption->render();
	} else {texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f); 
		playOption->render();
	}

	if (selectedOption == 1) {
		texProgram.setUniform4f("color", 1.0f, 1.0f, 0.0f, 1.0f);  // Groc per "Instructions"
		instructionsOption->render();
	} else { texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f); 
		instructionsOption->render();
	}
	if (selectedOption == 2) {
		texProgram.setUniform4f("color", 1.0f, 1.0f, 0.0f, 1.0f);  // Groc per "Credits"
		creditsOption->render();
	} else {texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f); 
		creditsOption->render();
	}

}

void Menu::moveSelectionUp() {
	selectedOption = (selectedOption + 2) % 3;  // Mou amunt cíclicament
}

void Menu::moveSelectionDown() {
	selectedOption = (selectedOption + 1) % 3;  // Mou avall cíclicament
}

int Menu::getSelectedOption() const {
	return selectedOption;
}

void Menu::initShaders()
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
