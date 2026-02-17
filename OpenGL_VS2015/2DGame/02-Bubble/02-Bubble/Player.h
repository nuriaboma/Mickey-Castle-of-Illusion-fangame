#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include <chrono>
#include <thread>
#include "Sprite.h"
#include "TileMap.h"
#include "Rock.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	glm::vec2 getPosition();
	glm::ivec2 getPosHitBox();
	glm::ivec2 getSizeHitBox();
	void setPosiblePickRock(Rock* rock);
	int getFacingDirection(); // 0=esquerra, 1=dreta
	bool estaCaienDeCul();
	void enemyKilled();
	void destroyRock();
	void lostLife(float enemypos, float enemysize);
	void Dead();
	void Hit();
	void changeAnimation(int animation);
	int getVides();
	int getTries();
	int getScore();
	void restarVida(float enemypos, float enemysize);
	void restarTry();
	void sumarPunts(int punts);
	void sumarVida();
	bool entraPorta();
	void restartVides();
	bool inlake();

private:
	bool bJumping;
	bool cauDeCul;
	bool crouching;
	bool climbing;
	Rock* posiblePickRock;
	bool pickingRock;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	bool enemykilled;
	bool destroyrock;
	bool wKeyReleased;
	bool sKeyReleased;
	std::chrono::steady_clock::time_point invincibleStartTime;
	int vides;
	int tries;
	int score;
	int cooldownTime;
	int lastHitTime;
	bool entraporta;
	bool hkeypressed;
	bool gKeyReleased;
	bool hKeyReleased;
	bool godMode;
};


#endif // _PLAYER_INCLUDE


