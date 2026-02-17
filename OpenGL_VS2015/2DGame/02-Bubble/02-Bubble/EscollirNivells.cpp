#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "EscollirNivells.h"
#include "Game.h"
#include "HUD.h"
//#include <irrKlang.h>

#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 3 
#define INIT_PLAYER_Y_TILES 8

EscollirNivell::EscollirNivell()
{
	map = NULL;
	player = NULL;
	hud = NULL;
}

EscollirNivell::~EscollirNivell()
{
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
	if (hud != NULL)
		delete hud;
}

void EscollirNivell::init()
{
	initShaders();

	map = TileMap::createTileMap("levels/escollirnivell.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);


	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);


	hud = new HUD();
	hud->init(texProgram);

	currentTime = 0.0f;

	cameraPosition = player->getPosition();
	totalTime = 700.f;

}

void EscollirNivell::update(int deltaTime)
{
	currentTime += deltaTime;


	player->update(deltaTime);

	float mapHeight = 448;
	float mapWidth = 736;

	hud->update(player->getVides(), player->getTries(), player->getScore(), cameraPosition, totalTime);
	cameraPosition.x = player->getPosition().x;
	cameraPosition.y = 315; 


	float minX = CAMERA_OFFSET_X;
	float maxX = mapWidth - CAMERA_OFFSET_X;
	float minY = CAMERA_OFFSET_Y;
	float maxY = mapHeight - CAMERA_OFFSET_Y;

	// Limitar la càmera dins del mapa
	cameraPosition.x = std::max(minX, std::min(cameraPosition.x, maxX));
	cameraPosition.y = std::max(minY, std::min(cameraPosition.y, maxY));

	glm::vec2 posPlayer = player->getPosHitBox();
	glm::vec2 sizePlayer = player->getSizeHitBox();

	entraNivell();
}

void EscollirNivell::render()
{
	glm::mat4 modelview;

	texProgram.use();


	projection = glm::ortho(cameraPosition.x - CAMERA_OFFSET_X, cameraPosition.x + CAMERA_OFFSET_X,
		cameraPosition.y + CAMERA_OFFSET_Y, cameraPosition.y - CAMERA_OFFSET_Y);


	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	player->render();
	hud->render();

}

int EscollirNivell::entraNivell()
{
	if (player->entraPorta()) {
		entra = 1;
		//std::cout << "El jugador ha entrat a una porta!" << std::endl;
	}
	return entra;
		// Pots reinicialitzar `entraporta` a `false` des de la classe `Player`
		// si necessites fer-ho després d'entrar a una porta. 
}

void EscollirNivell::initShaders()
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

