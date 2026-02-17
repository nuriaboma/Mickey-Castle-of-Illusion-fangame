#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "Enemy2.h"
#include "Rock.h"
#include "BolaBoss.h"
#include "Boss.h"
#include "HUD.h"
// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

	void addRock(const glm::ivec2 &position, int type);
	bool gematr();
private:
	void initShaders();
	void detectPlayerEnemyCollision();
	void rockReadyToPick(const glm::ivec2 &position);
	void bossDead();
	void playerLostLive(float enemypos, float enemysize);
	void playerLostAllLives();
	void playerLostGame();
	void playerWonGame();
private:
	TileMap *map;
	Player *player;
	std::vector<Rock*> rocks;
	glm::vec2 posiblePickingRock;
	ShaderProgram texProgram;
	float currentTime;
	float totalTime;
	glm::mat4 projection;
	glm::vec2 cameraPosition; // Posición de la cámara
	const float CAMERA_OFFSET_X = 256.f; // Desplazamiento de la cámara (mitad del ancho de la pantalla)
	const float CAMERA_OFFSET_Y = 176.f;
	bool collisionObject;
	std::vector<Enemy*> enemies;
	std::vector<Enemy2*> enemies2;
	int coin = 300;
	int gem = 500;
	std::vector<BolaBoss*> bolasBoss;
	Boss* boss;
	bool gameFinished;
	HUD* hud;
	bool gkeypressed;
	bool gematrobada;
};


#endif 

