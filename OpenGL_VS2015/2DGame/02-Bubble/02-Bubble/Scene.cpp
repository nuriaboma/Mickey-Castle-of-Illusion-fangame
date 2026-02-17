#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "Audio.h"


#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 61 //8 pract / 61 map1 /4              /BOSS 127  63      61 21
#define INIT_PLAYER_Y_TILES 42 //22 pract / 42 map1 /8 //39        /BOSS 24    6

enum DirectionEnemy2 { LEFT, RIGHT };

ISoundEngine* soundEngine = nullptr;

Scene::Scene()
{
	map = NULL;
	player = NULL;
	for (Enemy* e : enemies) {
		e = NULL;
	}
	for (Enemy2* e2 : enemies2) {
		e2 = NULL;
	}
	for (BolaBoss* b : bolasBoss) {
		b = NULL;
	}
	boss = NULL;
	for (Rock* rock : rocks) {
		rock = NULL;
	}
	soundEngine = createIrrKlangDevice();
	if (!soundEngine) {
		std::cerr << "Error inicializando irrKlang!" << std::endl;
	}

	hud = NULL;
}

Scene::~Scene()
{
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
	for (Enemy* e : enemies) {
		if (e != NULL)
			delete e;
	}
	for (Enemy2* e2 : enemies2) {
		if (e2 != NULL)
			delete e2;
	}
	for (BolaBoss* b : bolasBoss) {
		if (b != NULL)
			delete b;
	}
	if (boss != NULL)
		delete boss;
	for (Rock* rock : rocks) {
		delete rock;
	}
	if (soundEngine) {
		soundEngine->drop();
	}
	if (hud != NULL)
		delete hud;
}

void Scene::init()
{
	gkeypressed = false;
	initShaders();
	if (soundEngine) {
		soundEngine->play2D("media/musica/woods.ogg", true);
	}
	gematrobada = false;
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	//map = TileMap::createTileMap("levels/practice.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	//map = TileMap::createTileMap("levels/escollirnivell.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);


	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	Enemy *enemy = new Enemy();
	enemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	enemy->setPosition(glm::vec2(43 * map->getTileSize(), 38 * map->getTileSize())); // Posició inicial del primer enemic
	enemy->setTileMap(map);
	enemies.push_back(enemy);

	Enemy* enemy2 = new Enemy();
	enemy2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	enemy2->setPosition(glm::vec2(27 * map->getTileSize(), 37 * map->getTileSize())); // Posició inicial del segon enemic
	enemy2->setTileMap(map);
	enemies.push_back(enemy2);

	Enemy* enemy3 = new Enemy();
	enemy3->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	enemy3->setPosition(glm::vec2(30 * map->getTileSize(), 25 * map->getTileSize())); // Posició inicial del segon enemic
	enemy3->setTileMap(map);
	enemies.push_back(enemy3);

	Enemy* enemy4 = new Enemy();
	enemy4->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	enemy4->setPosition(glm::vec2(40 * map->getTileSize(), 25 * map->getTileSize())); // Posició inicial del segon enemic
	enemy4->setTileMap(map);
	enemies.push_back(enemy4);

	Enemy* enemy5 = new Enemy();
	enemy5->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	enemy5->setPosition(glm::vec2(80 * map->getTileSize(), 25 * map->getTileSize())); // Posició inicial del segon enemic
	enemy5->setTileMap(map);
	enemies.push_back(enemy5);

	Enemy* enemy6 = new Enemy();
	enemy6->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	enemy6->setPosition(glm::vec2(110 * map->getTileSize(), 36 * map->getTileSize())); // Posició inicial del segon enemic
	enemy6->setTileMap(map);
	enemies.push_back(enemy6);

	Enemy2* enemy21 = new Enemy2();
	enemy21->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	enemy21->setPosition(glm::vec2(47 * map->getTileSize(), 43 * map->getTileSize())); // Posició inicial del segon enemic
	enemy21->setTileMap(map);
	enemies2.push_back(enemy21);

	//Enemy2* enemy22 = new Enemy2();
	//enemy22->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	//enemy22->setPosition(glm::vec2(3 * map->getTileSize(), 31 * map->getTileSize())); // Posició inicial del segon enemic
	//enemy22->setTileMap(map);
	//enemies2.push_back(enemy22);
	
	BolaBoss *bolaBoss1 = new BolaBoss();
	bolaBoss1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::vec2(1, 0), 0.15f);
	bolaBoss1->setStartPosition(glm::vec2(109 * map->getTileSize(), 14 * map->getTileSize()));
	bolaBoss1->setTileMap(map);
	bolasBoss.push_back(bolaBoss1);

	BolaBoss* bolaBoss2 = new BolaBoss();
	bolaBoss2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::vec2(1, 0.5), 0.15f);
	bolaBoss2->setStartPosition(glm::vec2(109 * map->getTileSize(), 14 * map->getTileSize()));
	bolaBoss2->setTileMap(map);
	bolasBoss.push_back(bolaBoss2);

	BolaBoss* bolaBoss3 = new BolaBoss();
	bolaBoss3->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::vec2(1, 1), 0.15f);
	bolaBoss3->setStartPosition(glm::vec2(109 * map->getTileSize(), 14 * map->getTileSize()));
	bolaBoss3->setTileMap(map);
	bolasBoss.push_back(bolaBoss3);

	boss = new Boss();
	boss->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	boss->setPosition(glm::vec2(105 * map->getTileSize(), 13 * map->getTileSize()));
	boss->setTileMap(map);

	const std::vector<objectInfo>& objects = map->getObjects();
	for (const objectInfo& obj : objects) {
		addRock(glm::ivec2(obj.position.x, obj.position.y), obj.type);
	}

	hud = new HUD();
	hud->init(texProgram);

	//projection = glm::ortho(0.f + 680, float(SCREEN_WIDTH) + 680, float(SCREEN_HEIGHT) + 730, 0.f + 730);
	currentTime = 0.0f;

	cameraPosition = player->getPosition();
	gameFinished = false;
	totalTime = 700.f;
	
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	if (totalTime - (currentTime/1000) < 0.f && !gameFinished)
		playerLostGame();

	player->update(deltaTime);
	
	
	for (Enemy* e : enemies) {
		e->detectPlayer(player->getPosition());
		e->update(deltaTime);
	}
	for (Enemy2* e2 : enemies2) {
		e2->update(deltaTime);
	}
	hud->update(player->getVides(), player->getTries(), player->getScore(), cameraPosition, totalTime - (currentTime / 1000));
	for (BolaBoss* bolaBoss : bolasBoss) {
		bolaBoss->update(deltaTime);
	}
	boss->update(deltaTime);
	float mapHeight = 1536;
	float mapWidth = 4224;

	if (player->getPosition().x < 280)
		cameraPosition.x = 280;
	else if (player->getPosition().x > 3960)
		cameraPosition.x = 3960;
	else
		cameraPosition.x = player->getPosition().x;

	int yPosPlayer = player->getPosition().y;
	std::cout << yPosPlayer << std::endl;
	if (yPosPlayer > 1137)
		cameraPosition.y = 1345; //1296
	else if (yPosPlayer > 689)
		cameraPosition.y = 964;
	else if (yPosPlayer > 349)
		cameraPosition.y = 629;
	else 
		cameraPosition.y = 249; //map1

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


	//cameraPosition.y = 720; //pract
	//cameraPosition.y = 224; //escNiv

	glm::vec2 posPlayer = player->getPosHitBox();
	glm::vec2 sizePlayer = player->getSizeHitBox();
	//glm::vec2 posEnemy = enemy->getPosition();
	//glm::vec2 sizeEnemy = enemy->getSize();

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

	if (posPlayer.y == 610 && posPlayer.x > 3280 && soundEngine->isCurrentlyPlaying("media/musica/woods.ogg"))
	{
		soundEngine->stopAllSounds();
		soundEngine->play2D("media/musica/boss.ogg", true);
	}
	if (player->inlake()) {
		playerLostAllLives();
	}

	if (Game::instance().getKey(GLFW_KEY_G) && !gkeypressed) {
		gkeypressed = true;
	}
	else if (Game::instance().getKey(GLFW_KEY_G) && gkeypressed) {
		gkeypressed = false;
	}

}

void Scene::render()
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
	for (Enemy* e : enemies) {
		e->render();
	}
	for (Enemy2* e2 : enemies2) {
		e2->render();
	}
	if (boss->getVides() > 0)
	for (BolaBoss* bolaBoss : bolasBoss) {
		bolaBoss->render();
	}
	boss->render();
	hud->render();
}

void Scene::initShaders()
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


void Scene::addRock(const glm::ivec2 &position, int type) {
	Rock* newRock = new Rock();
	newRock->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, type);
	newRock->setPosition(position);
	newRock->setTileMap(map); 
	rocks.push_back(newRock); 
}

void Scene::detectPlayerEnemyCollision() {
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

	for (BolaBoss* bolaBoss : bolasBoss) {
		glm::vec2 posEnemy = bolaBoss->getPosition();
		glm::vec2 sizeEnemy = bolaBoss->getSize();
		if (boss->getVides() > 0 && posPlayer.x + sizePlayer.x >= posEnemy.x && posPlayer.x <= posEnemy.x + sizeEnemy.x &&
			posPlayer.y + sizePlayer.y >= posEnemy.y && posPlayer.y <= posEnemy.y + sizeEnemy.y)
		{
		
			if (!gkeypressed) {
				playerLostLive(posEnemy.x, sizeEnemy.x);
			}
			
		}
	}

	glm::vec2 posEnemy = boss->getPosition();
	glm::vec2 sizeEnemy = boss->getSize();
	if (boss->getVides() > 0 && posPlayer.x + sizePlayer.x >= posEnemy.x && posPlayer.x <= posEnemy.x + sizeEnemy.x &&
		posPlayer.y + sizePlayer.y >= posEnemy.y && posPlayer.y <= posEnemy.y + sizeEnemy.y)
	{
		if (!gkeypressed) {
			playerLostLive(posEnemy.x, sizeEnemy.x);
		}
	}


	Rock* rockToDelete = NULL;
	for (Rock* rock : rocks) {
		glm::vec2 rockPos = rock->getPosition();
		glm::vec2 rockSize = rock->getSize();

		glm::vec2 bossPos = boss->getPosition();
		glm::ivec2 bossSize = boss->getSize();

		if (rock->thrown() && rockPos.x + rockSize.x >= bossPos.x && rockPos.x <= bossPos.x + bossSize.x &&
			rockPos.y + rockSize.y >= bossPos.y && rockPos.y <= bossPos.y + bossSize.y && boss->getVides() != 0)
		{
			boss->restarVida();
			
			if (boss->getVides() == 0)
				bossDead();
			break;
		}

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
					playerWonGame();
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

bool Scene::gematr() {
	return gematrobada;
}


void Scene::rockReadyToPick(const glm::ivec2 &position)
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
		player->setPosiblePickRock(NULL);
}


void Scene::bossDead()
{
	std::cout << "HAS GUANYAT!!!!!!!" << std::endl;
	Rock* newRock = new Rock();
	newRock->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 5);
	newRock->setPosition(boss->getPosition() + glm::ivec2(0, 16));
	newRock->setTileMap(map);
	rocks.push_back(newRock);
}

void Scene::playerLostLive(float enemypos, float enemysize)
{
	player->Hit();
	player->restarVida(enemypos, enemysize);
	if (player->getVides() == 0)
		playerLostAllLives();
}

void Scene::playerLostAllLives()
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

void Scene::playerLostGame()
{
	gameFinished = true;
	soundEngine->stopAllSounds();
	soundEngine->play2D("media/musica/dead.ogg", true);
}

void Scene::playerWonGame()
{
	gameFinished = true;
	soundEngine->stopAllSounds();
	soundEngine->play2D("media/musica/levelEnd.ogg", true);
}