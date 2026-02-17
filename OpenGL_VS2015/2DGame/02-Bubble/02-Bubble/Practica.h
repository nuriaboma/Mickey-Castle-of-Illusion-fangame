#ifndef _PRACTICA_INCLUDE
#define _PRACTICA_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "Enemy2.h"
#include "Rock.h"
#include "HUD.h"
#include "Audio.h"

class Practica
{

public:
	Practica();
	~Practica();

	void init();
	void update(int deltaTime);
	void render();

	void addRock(const glm::ivec2& position, int type);
	bool gematr();
private:
	void initShaders();
	void detectPlayerEnemyCollision();
	void rockReadyToPick(const glm::ivec2& position);
	void playerLostLive(float enemypos, float enemysize);
	void playerLostAllLives();
	void playerLostGame();
	void playerWonGame();
	

private:
	TileMap* map;
	Player* player;
	Enemy* enemy;
	Enemy* enemy2;
	Enemy2* enemy21;
	std::vector<Enemy*> enemies;
	std::vector<Enemy2*> enemies2;

	std::vector<Rock*> rocks;
	glm::vec2 posiblePickingRock;

	ShaderProgram texProgram;
	float currentTime;

	glm::mat4 projection;
	glm::vec2 cameraPosition; // Posición de la cámara
	const float CAMERA_OFFSET_X = 256.f; // Desplazamiento de la cámara (mitad del ancho de la pantalla)
	const float CAMERA_OFFSET_Y = 176.f;

	bool collisionObject;
	int vides;
	int coin = 300;
	int gem = 500;
	HUD* hud;
	float totalTime;
	bool gameFinished;
	bool gkeypressed;
	bool hkeypressed;
	bool gematrobada;

};

#endif
