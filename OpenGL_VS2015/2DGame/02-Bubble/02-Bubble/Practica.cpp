#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Practica.h"
#include "Game.h"
#include "Audio.h"


#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 8 //8 pract / 52 map1 /4
#define INIT_PLAYER_Y_TILES 22 //22 pract / 43 map1 /8 //39


Practica::Practica()
{
	map = NULL;
	player = NULL;
	enemy = NULL;
	enemy2 = NULL;
	enemy21 = NULL;
	for (Rock* rock : rocks) {
		rock = NULL;
	}
	hud = NULL;
	if (!soundEngine) {
		std::cerr << "Error inicialitzant irrKlang!" << std::endl;
	}
}

Practica::~Practica()
{
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
	if (enemy != NULL)
		delete enemy;
	if (enemy2 != NULL)
		delete enemy2;
	if (enemy21 != NULL)
		delete enemy21;
	for (Rock* rock : rocks) {
		delete rock;
	}
	if (hud != NULL)
		delete hud;
	if (soundEngine) {
		soundEngine->drop();
	}
}

void Practica::init()
{
	initShaders();
	if (soundEngine) {
		soundEngine->play2D("media/musica/woods.ogg", true);
	}
	gkeypressed = false;
	gematrobada = false;
	

	
	map = TileMap::createTileMap("levels/practice.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);


	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	enemy = new Enemy();
	enemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	enemy->setPosition(glm::vec2(43 * map->getTileSize(), 38 * map->getTileSize())); // Posició inicial del primer enemic
	enemy->setTileMap(map);
	enemies.push_back(enemy);

	enemy2 = new Enemy();
	enemy2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	enemy2->setPosition(glm::vec2(28 * map->getTileSize(), 18 * map->getTileSize())); // Posició inicial del segon enemic
	enemy2->setTileMap(map);
	enemies.push_back(enemy2);

	enemy21 = new Enemy2();
	enemy21->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	enemy21->setPosition(glm::vec2(15 * map->getTileSize(), 24 * map->getTileSize())); // Posició inicial del segon enemic
	enemy21->setTileMap(map);
	enemies2.push_back(enemy21);

	const std::vector<objectInfo>& objects = map->getObjects();
	for (const objectInfo& obj : objects) {
		addRock(glm::ivec2(obj.position.x, obj.position.y), obj.type);
	}


	//projection = glm::ortho(0.f + 680, float(SCREEN_WIDTH) + 680, float(SCREEN_HEIGHT) + 730, 0.f + 730);
	currentTime = 0.0f;

	cameraPosition = player->getPosition();
	hud = new HUD();
	hud->init(texProgram);

	currentTime = 0.0f;

	cameraPosition = player->getPosition();
	totalTime = 700.f;

}

void Practica::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	enemy->detectPlayer(player->getPosition());
	enemy->update(deltaTime);
	enemy2->detectPlayer(player->getPosition());
	enemy2->update(deltaTime);
	enemy21->update(deltaTime);


	hud->update(player->getVides(), player->getTries(), player->getScore(), cameraPosition, totalTime - (currentTime / 1000));

	float mapHeight = 896;
	float mapWidth = 1344;

	cameraPosition.x = player->getPosition().x;
	int yPosPlayer = player->getPosition().y;
	std::cout << yPosPlayer << std::endl;

	if (yPosPlayer > 442)
		cameraPosition.y = 740;
	else
		cameraPosition.y = 300; 

	float minX = CAMERA_OFFSET_X;
	float maxX = mapWidth - CAMERA_OFFSET_X;
	float minY = CAMERA_OFFSET_Y;
	float maxY = mapHeight - CAMERA_OFFSET_Y;

	// Limitar la càmera dins del mapa
	cameraPosition.x = std::max(minX, std::min(cameraPosition.x, maxX));
	cameraPosition.y = std::max(minY, std::min(cameraPosition.y, maxY));

	rocks.erase(
		std::remove_if(rocks.begin(), rocks.end(), [](Rock* rock) {
			if (rock->isDeleted()) {
				delete rock;
				return true; // Se elimina del vector
			}
			return false;
			}),
		rocks.end()
	);

	for (Rock* rock : rocks) {
		rock->update(deltaTime);
	}

	glm::vec2 posPlayer = player->getPosHitBox();
	glm::vec2 sizePlayer = player->getSizeHitBox();
	glm::vec2 posEnemy = enemy->getPosition();
	glm::vec2 sizeEnemy = enemy->getSize();

	detectPlayerEnemyCollision();
	int facingDirectionPlayer = player->getFacingDirection();
	glm::vec2 posiblePosRock;
	if (facingDirectionPlayer == 0) // left
	{
		posiblePosRock = posPlayer + glm::vec2(-1, sizePlayer.y / 2 + 1);
	}
	else // right
	{
		posiblePosRock = posPlayer + glm::vec2(sizePlayer.x + 4, sizePlayer.y / 2 + 1);
	}


	rockReadyToPick(posiblePosRock);
	if (Game::instance().getKey(GLFW_KEY_G) && !gkeypressed) {
		gkeypressed = true;
	}
	else if (Game::instance().getKey(GLFW_KEY_G) && gkeypressed) {
		gkeypressed = false;
	}
	if (player->inlake()) {
		playerLostAllLives();
	}


}

void Practica::render()
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


	for (Rock* rock : rocks) {
		rock->render();
	}
	player->render();
	enemy->render();
	enemy2->render();
	enemy21->render();
	hud->render();


}

void Practica::initShaders()
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


void Practica::addRock(const glm::ivec2& position, int type) {
	Rock* newRock = new Rock();
	newRock->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, type);
	newRock->setPosition(position);
	newRock->setTileMap(map);
	rocks.push_back(newRock);
}

void Practica::detectPlayerEnemyCollision() {
	glm::vec2 posPlayer = player->getPosHitBox();
	glm::vec2 sizePlayer = player->getSizeHitBox();

	for (Enemy* e : enemies) { //ENEMIC QUE CAU
		glm::vec2 posEnemy = e->getPosition();
		glm::vec2 sizeEnemy = e->getSize();

		if (posPlayer.x + sizePlayer.x >= posEnemy.x && posPlayer.x <= posEnemy.x + sizeEnemy.x &&
			posPlayer.y + sizePlayer.y >= posEnemy.y && posPlayer.y <= posEnemy.y + sizeEnemy.y)
		{
			if (!gkeypressed) {
				playerLostLive(posEnemy.x, sizeEnemy.x);
			}
		}
	}

	for (Enemy2* e : enemies2) { //ENEMIC SOLDAT
		glm::vec2 posEnemy = e->getPosition();
		glm::vec2 sizeEnemy = e->getSize();

		if (e->isAlive() && posPlayer.x + sizePlayer.x >= posEnemy.x && posPlayer.x <= posEnemy.x + sizeEnemy.x &&
			posPlayer.y + sizePlayer.y >= posEnemy.y && posPlayer.y <= posEnemy.y + sizeEnemy.y) {
			float botPlayer = posPlayer.y + sizePlayer.y;
			float topEnemy = posEnemy.y;

			if (botPlayer <= topEnemy + 5 && player->estaCaienDeCul()) {
				player->enemyKilled();
				e->Killed();
				player->sumarPunts(10);
			}
			else
			{

				if (!gkeypressed) {
					playerLostLive(posEnemy.x, sizeEnemy.x);
				}
			}
		}
	}

	Rock* rockToDelete = NULL;
	for (Rock* rock : rocks) {
		glm::vec2 rockPos = rock->getPosition();
		glm::vec2 rockSize = rock->getSize();

		
		if (rock->getType() != 3 && rock->getType() != 4 && rock->getType() != 5)
		{

			float bottomPlayer = posPlayer.y + sizePlayer.y;
			float topRock = rockPos.y;
			if (abs(bottomPlayer - topRock) <= 4 && posPlayer.x + sizePlayer.x > rockPos.x && posPlayer.x < rockPos.x + rockSize.x)
			{
				if (player->estaCaienDeCul()) {
					player->destroyRock();
					if (rock->getType() == 2)
					{
						rock->destroyChest();
						map->deleteRock(rockPos);
					}
					else
					{
						rockToDelete = rock;
						map->deleteRock(rockPos);
					}
					break;
				}
			}
		}
		else
		{
			if (posPlayer.x + sizePlayer.x >= rockPos.x && posPlayer.x <= rockPos.x + rockSize.x &&
				posPlayer.y + sizePlayer.y >= rockPos.y && posPlayer.y <= rockPos.y + rockSize.y)
			{
				rockToDelete = rock;
				if (rock->getType() == 3) //CAKE
				{
					player->sumarPunts(10);
					player->sumarVida();
					soundEngine->play2D("media/sounds/pickItem.ogg", false);
				}
				else if (rock->getType() == 4) //COIN
				{
					player->sumarPunts(30);
					soundEngine->play2D("media/sounds/pickCoin.ogg", false);
				}
				else if (rock->getType() == 5) //GEMA
				{
					player->sumarPunts(1000);
					gematrobada = true;
					//EL JUGADOR HA GUANYAT LA PARTIDA
				}
			}
		}
	}

	if (rockToDelete != NULL) {
		auto it = std::find(rocks.begin(), rocks.end(), rockToDelete);
		if (it != rocks.end()) {
			rocks.erase(it);
			delete rockToDelete;
		}
	}
}

void Practica::rockReadyToPick(const glm::ivec2& position)
{
	for (Rock* rock : rocks) {
		if (rock->getType() != 3 && rock->getType() != 4 && rock->getType() != 5)
		{
			glm::vec2 rockPos = rock->getPosition();
			glm::vec2 rockSize = rock->getSize();

			if (position.x >= rockPos.x && position.x <= rockPos.x + rockSize.x &&
				position.y >= rockPos.y && position.y <= rockPos.y + rockSize.y) {
				player->setPosiblePickRock(rock);
				posiblePickingRock = rockPos;
				return;
			}
		}
	}
	/*
	if (!possible) {
	posiblePickingRock = glm::vec2(-1, -1);
	}
	*/
	player->setPosiblePickRock(NULL);
}

bool Practica::gematr() {
	return gematrobada;
}


void Practica::playerLostLive(float enemypos, float enemysize)
{
	player->Hit();
	player->restarVida(enemypos, enemysize);
	if (player->getVides() == 0)
		playerLostAllLives();
}

void Practica::playerLostAllLives()
{
	if (player->getTries() == 0)
	{
		//HA PERDUT
		playerLostGame();
	}
	else
	{
		player->restarTry();
		player->setPosition(glm::ivec2(INIT_PLAYER_X_TILES * 32, INIT_PLAYER_Y_TILES * 32));
		soundEngine->play2D("media/sounds/hurt.ogg", false);
	}
}

void Practica::playerLostGame()
{
	gameFinished = true;
	soundEngine->stopAllSounds();
	soundEngine->play2D("media/musica/dead.ogg", true);
}

void Practica::playerWonGame()
{
	gameFinished = true;
	soundEngine->stopAllSounds();
	soundEngine->play2D("media/musica/levelEnd.ogg", true);
}