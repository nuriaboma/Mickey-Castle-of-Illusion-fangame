#ifndef _ESCOLLIRNIVELL_INCLUDE
#define _ESCOLLIRNIVELL_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "HUD.h"

class EscollirNivell
{

public:
	EscollirNivell();
	~EscollirNivell();

	void init();
	void update(int deltaTime);
	void render();
	int entraNivell();



private:
	void initShaders();

private:
	TileMap* map;
	Player* player;
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
	int entra;
	HUD* hud;
	float totalTime;

};

#endif
