#include <iostream>
#include "Instructions.h"
#include "Game.h"

#define SCREEN_X 0
#define SCREEN_Y 0

Instructions::Instructions() {
	map = NULL;
}

Instructions::~Instructions() {
	if (map != NULL)
		delete map;
}


void Instructions::init() {

	initShaders();
	map = TileMap::createTileMap("levels/intro.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	Texture* jumpTexture = new Texture();
	jumpTexture->loadFromFile("images/text/jump.png", TEXTURE_PIXEL_FORMAT_RGBA);
	jumpText = Sprite::createSprite(glm::vec2(231, 50), glm::vec2(1, 1), jumpTexture, &texProgram);
	//jumpText->setPosition(glm::vec2(396.5, 95));
	jumpText->setPosition(glm::vec2(380.5, 80));

	Texture* crouchTexture = new Texture();
	crouchTexture->loadFromFile("images/text/crouch.png", TEXTURE_PIXEL_FORMAT_RGBA);
	crouchText = Sprite::createSprite(glm::vec2(256, 43), glm::vec2(1, 1), crouchTexture, &texProgram);
	//crouchText->setPosition(glm::vec2(384, 165));
	crouchText->setPosition(glm::vec2(396.5, 135));

	Texture* leftTexture = new Texture();
	leftTexture->loadFromFile("images/text/left.png", TEXTURE_PIXEL_FORMAT_RGBA);
	leftText = Sprite::createSprite(glm::vec2(185, 43), glm::vec2(1, 1), leftTexture, &texProgram);
	//leftText->setPosition(glm::vec2(419.5, 233));
	leftText->setPosition(glm::vec2(396.5, 193));

	Texture* rightTexture = new Texture();
	rightTexture->loadFromFile("images/text/right.png", TEXTURE_PIXEL_FORMAT_RGBA);
	rightText = Sprite::createSprite(glm::vec2(221, 54), glm::vec2(1, 1), rightTexture, &texProgram);
	//rightText->setPosition(glm::vec2(401.5, 301));
	rightText->setPosition(glm::vec2(396.5, 250));

	Texture* pickupTexture = new Texture();
	pickupTexture->loadFromFile("images/text/pickup.png", TEXTURE_PIXEL_FORMAT_RGBA);
	pickupText = Sprite::createSprite(glm::vec2(266, 54), glm::vec2(1, 1), pickupTexture, &texProgram);
	//pickupText->setPosition(glm::vec2(379, 380));
	pickupText->setPosition(glm::vec2(396.5, 310));

	Texture* throwTexture = new Texture();
	throwTexture->loadFromFile("images/text/throw.png", TEXTURE_PIXEL_FORMAT_RGBA);
	throwText = Sprite::createSprite(glm::vec2(245, 43), glm::vec2(1, 1), throwTexture, &texProgram);
	//throwText->setPosition(glm::vec2(389.5, 454));
	throwText->setPosition(glm::vec2(403.5, 364));

	Texture* HTexture = new Texture();
	HTexture->loadFromFile("images/text/H.png", TEXTURE_PIXEL_FORMAT_RGBA);
	HText = Sprite::createSprite(glm::vec2(329, 43), glm::vec2(1, 1), HTexture, &texProgram);
	//throwText->setPosition(glm::vec2(389.5, 454));
	HText->setPosition(glm::vec2(403.5, 424));

	Texture* GTexture = new Texture();
	GTexture->loadFromFile("images/text/G.png", TEXTURE_PIXEL_FORMAT_RGBA);
	GText = Sprite::createSprite(glm::vec2(324, 43), glm::vec2(1, 1), GTexture, &texProgram);
	//throwText->setPosition(glm::vec2(389.5, 454));
	GText->setPosition(glm::vec2(403.5, 480));

	Texture* exitTexture = new Texture();
	exitTexture->loadFromFile("images/text/exit.png", TEXTURE_PIXEL_FORMAT_RGBA);
	exit = Sprite::createSprite(glm::vec2(75, 35), glm::vec2(1, 1), exitTexture, &texProgram);
	exit->setPosition(glm::vec2(474.5, 575));

}


void Instructions::update(int deltaTime) {
	jumpText->update(deltaTime);
	crouchText->update(deltaTime);
	leftText->update(deltaTime);
	rightText->update(deltaTime);
	pickupText->update(deltaTime);
	throwText->update(deltaTime);
	GText->update(deltaTime);
	HText->update(deltaTime);
	exit->update(deltaTime);
}

void Instructions::render() {

	glm::mat4 modelview = glm::mat4(1.0f);
	texProgram.use();

	glm::mat4 projection = glm::ortho(0.0f, mapWidth, mapHeight, 0.0f);
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniformMatrix4f("modelview", modelview);

	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	jumpText->render();
	crouchText->render();
	leftText->render();
	rightText->render();
	pickupText->render();
	throwText->render();
	GText->render();
	HText->render();

	texProgram.setUniform4f("color", 1.0f, 1.0f, 0.0f, 1.0f);
	exit->render();

}


void Instructions::initShaders()
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
