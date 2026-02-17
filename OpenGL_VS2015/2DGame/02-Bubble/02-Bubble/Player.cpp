#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"
#include "Audio.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 67
#define FALL_STEP 3


bool isLanding = false;
bool isLosingLife = false;  // Indica si el jugador est� en l'estat "lost life"
float lostLifeTimer = 0.0f;
float speedX = 0.0f;      // Velocitat horitzontal actual
float maxSpeedX = 3.0f;   // Velocitat m�xima
float accelerationX = 0.5; // Velocitat d'acceleracio
float decelerationX = 1.f; // Velocitat de desacceleracio
bool lakehit = false;




enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, JUMP_RIGHT, FALL_RIGHT, JUMP_LEFT, FALL_LEFT, LAND_RIGHT, LAND_LEFT, CROUCH_RIGHT, CROUCH_LEFT, CAUDECUL_RIGHT, CAUDECUL_LEFT, CLIMB, READY_TO_PICK_RIGHT, READY_TO_PICK_LEFT,
	STAND_PICK_LEFT, STAND_PICK_RIGHT, MOVE_PICK_LEFT, MOVE_PICK_RIGHT, JUMP_PICK_LEFT, JUMP_PICK_RIGHT, FALL_PICK_LEFT, FALL_PICK_RIGHT, LAND_PICK_LEFT, LAND_PICK_RIGHT, LAND_CAUDECUL_RIGHT, LAND_CAUDECUL_LEFT,
	LOST_LIFE_LEFT, LOST_LIFE_RIGHT, DEAD, FRENAR_RIGHT, FRENAR_LEFT
};

enum Direction { LEFT, RIGHT };

Direction facingDirection;
Direction jumpDirection;


void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{

	facingDirection = RIGHT;
	jumpDirection = RIGHT;
	bJumping = false;
	crouching = false;
	cauDeCul = false;
	climbing = false;
	posiblePickRock = NULL;
	pickingRock = false;
	enemykilled = false;
	destroyrock = false;
	wKeyReleased = true;
	sKeyReleased = true;
	vides = 3;
	tries = 3;
	cooldownTime = 2000;
	lastHitTime = 0;
	entraporta = false;
	hkeypressed = false;
	godMode = false;
	gKeyReleased = true;
	hKeyReleased = true;

	spritesheet.loadFromFile("images/Mickey2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(72, 96), glm::vec2(0.0625f, 0.0625f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(34);

	sprite->setAnimationSpeed(STAND_LEFT, 4);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.0625f));
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.0625f, 0.0625f));

	sprite->setAnimationSpeed(STAND_RIGHT, 4);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0625f, 0.f));

	sprite->setAnimationSpeed(MOVE_LEFT, 12);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0625f * 2, 0.0625f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0625f * 3, 0.0625f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0625f * 4, 0.0625f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0625f * 5, 0.0625f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0625f * 6, 0.0625f));
	//sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0625f * 7, 0.0625f));
	//sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0625f * 8, 0.0625f));
	//sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0625f * 9, 0.0625f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 12);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0625f * 2, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0625f * 3, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0625f * 4, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0625f * 5, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0625f * 6, 0.f));
	//sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0625f * 7, 0.f));
	//sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0625f * 8, 0.f));
	//sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0625f * 9, 0.f));

	sprite->setAnimationSpeed(JUMP_RIGHT, 8);
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.0625f * 0, 0.0625f * 2));

	sprite->setAnimationSpeed(FALL_RIGHT, 8);
	sprite->addKeyframe(FALL_RIGHT, glm::vec2(0.0625f * 1, 0.0625f * 2));

	sprite->setAnimationSpeed(JUMP_LEFT, 8);
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.0625f * 0, 0.0625f * 3));

	sprite->setAnimationSpeed(FALL_LEFT, 8);
	sprite->addKeyframe(FALL_LEFT, glm::vec2(0.0625f * 1, 0.0625f * 3));

	sprite->setAnimationSpeed(LAND_LEFT, 8);
	sprite->addKeyframe(LAND_LEFT, glm::vec2(0.0625f * 2, 0.0625f * 3));

	sprite->setAnimationSpeed(LAND_RIGHT, 8);
	sprite->addKeyframe(LAND_RIGHT, glm::vec2(0.0625f * 2, 0.0625f * 2));

	sprite->setAnimationSpeed(CROUCH_RIGHT, 2);
	sprite->addKeyframe(CROUCH_RIGHT, glm::vec2(0.0625f * 10, 0.0625f * 0));
	sprite->addKeyframe(CROUCH_RIGHT, glm::vec2(0.0625f * 11, 0.0625f * 0));

	sprite->setAnimationSpeed(CROUCH_LEFT, 2);
	sprite->addKeyframe(CROUCH_LEFT, glm::vec2(0.0625f * 10, 0.0625f * 1));
	sprite->addKeyframe(CROUCH_LEFT, glm::vec2(0.0625f * 11, 0.0625f * 1));

	sprite->setAnimationSpeed(CAUDECUL_RIGHT, 4);
	sprite->addKeyframe(CAUDECUL_RIGHT, glm::vec2(0.0625f * 3, 0.0625f * 2));
	sprite->addKeyframe(CAUDECUL_RIGHT, glm::vec2(0.0625f * 4, 0.0625f * 2));

	sprite->setAnimationSpeed(CAUDECUL_LEFT, 4);
	sprite->addKeyframe(CAUDECUL_LEFT, glm::vec2(0.0625f * 3, 0.0625f * 3));
	sprite->addKeyframe(CAUDECUL_LEFT, glm::vec2(0.0625f * 4, 0.0625f * 3));

	sprite->setAnimationSpeed(CLIMB, 3);
	sprite->addKeyframe(CLIMB, glm::vec2(0.0625f * 5, 0.0625f * 2));
	sprite->addKeyframe(CLIMB, glm::vec2(0.0625f * 6, 0.0625f * 2));

	sprite->setAnimationSpeed(READY_TO_PICK_RIGHT, 3);
	sprite->addKeyframe(READY_TO_PICK_RIGHT, glm::vec2(0.0625f * 0, 0.0625f * 4));
	sprite->addKeyframe(READY_TO_PICK_RIGHT, glm::vec2(0.0625f * 1, 0.0625f * 4));

	sprite->setAnimationSpeed(READY_TO_PICK_LEFT, 3);
	sprite->addKeyframe(READY_TO_PICK_LEFT, glm::vec2(0.0625f * 0, 0.0625f * 6));
	sprite->addKeyframe(READY_TO_PICK_LEFT, glm::vec2(0.0625f * 1, 0.0625f * 6));

	sprite->setAnimationSpeed(STAND_PICK_LEFT, 3);
	sprite->addKeyframe(STAND_PICK_LEFT, glm::vec2(0.0625f * 2, 0.0625f * 6));
	sprite->addKeyframe(STAND_PICK_LEFT, glm::vec2(0.0625f * 3, 0.0625f * 6));

	sprite->setAnimationSpeed(STAND_PICK_RIGHT, 3);
	sprite->addKeyframe(STAND_PICK_RIGHT, glm::vec2(0.0625f * 2, 0.0625f * 4));
	sprite->addKeyframe(STAND_PICK_RIGHT, glm::vec2(0.0625f * 3, 0.0625f * 4));

	sprite->setAnimationSpeed(MOVE_PICK_LEFT, 12);
	sprite->addKeyframe(MOVE_PICK_LEFT, glm::vec2(0.0625f * 5, 0.0625f * 6));
	sprite->addKeyframe(MOVE_PICK_LEFT, glm::vec2(0.0625f * 6, 0.0625f * 6));
	sprite->addKeyframe(MOVE_PICK_LEFT, glm::vec2(0.0625f * 7, 0.0625f * 6));
	sprite->addKeyframe(MOVE_PICK_LEFT, glm::vec2(0.0625f * 8, 0.0625f * 6));
	sprite->addKeyframe(MOVE_PICK_LEFT, glm::vec2(0.0625f * 9, 0.0625f * 6));
	sprite->addKeyframe(MOVE_PICK_LEFT, glm::vec2(0.0625f * 10, 0.0625f * 6));
	//sprite->addKeyframe(MOVE_PICK_LEFT, glm::vec2(0.0625f * 11, 0.0625f * 6));
	//sprite->addKeyframe(MOVE_PICK_LEFT, glm::vec2(0.0625f * 12, 0.0625f * 6));

	sprite->setAnimationSpeed(MOVE_PICK_RIGHT, 12);
	sprite->addKeyframe(MOVE_PICK_RIGHT, glm::vec2(0.0625f * 5, 0.0625f * 4));
	sprite->addKeyframe(MOVE_PICK_RIGHT, glm::vec2(0.0625f * 6, 0.0625f * 4));
	sprite->addKeyframe(MOVE_PICK_RIGHT, glm::vec2(0.0625f * 7, 0.0625f * 4));
	sprite->addKeyframe(MOVE_PICK_RIGHT, glm::vec2(0.0625f * 8, 0.0625f * 4));
	sprite->addKeyframe(MOVE_PICK_RIGHT, glm::vec2(0.0625f * 9, 0.0625f * 4));
	sprite->addKeyframe(MOVE_PICK_RIGHT, glm::vec2(0.0625f * 10, 0.0625f * 4));
	//sprite->addKeyframe(MOVE_PICK_RIGHT, glm::vec2(0.0625f * 11, 0.0625f * 4));
	//sprite->addKeyframe(MOVE_PICK_RIGHT, glm::vec2(0.0625f * 12, 0.0625f * 4));

	sprite->setAnimationSpeed(JUMP_PICK_LEFT, 4);
	sprite->addKeyframe(JUMP_PICK_LEFT, glm::vec2(0.0625f * 13, 0.0625f * 6));

	sprite->setAnimationSpeed(JUMP_PICK_RIGHT, 4);
	sprite->addKeyframe(JUMP_PICK_RIGHT, glm::vec2(0.0625f * 13, 0.0625f * 4));

	sprite->setAnimationSpeed(FALL_PICK_LEFT, 8);
	sprite->addKeyframe(FALL_PICK_LEFT, glm::vec2(0.0625f * 14, 0.0625f * 6));

	sprite->setAnimationSpeed(FALL_PICK_RIGHT, 8);
	sprite->addKeyframe(FALL_PICK_RIGHT, glm::vec2(0.0625f * 14, 0.0625f * 4));

	sprite->setAnimationSpeed(LAND_PICK_LEFT, 8);
	sprite->addKeyframe(LAND_PICK_LEFT, glm::vec2(0.0625f * 15, 0.0625f * 6));

	sprite->setAnimationSpeed(LAND_PICK_RIGHT, 8);
	sprite->addKeyframe(LAND_PICK_RIGHT, glm::vec2(0.0625f * 15, 0.0625f * 4));

	sprite->setAnimationSpeed(LAND_CAUDECUL_RIGHT, 6);
	sprite->addKeyframe(LAND_CAUDECUL_RIGHT, glm::vec2(0.0625f * 0, 0.0625f * 9));
	sprite->addKeyframe(LAND_CAUDECUL_RIGHT, glm::vec2(0.0625f * 1, 0.0625f * 9));
	sprite->addKeyframe(LAND_CAUDECUL_RIGHT, glm::vec2(0.0625f * 1, 0.0625f * 9));
	sprite->addKeyframe(LAND_CAUDECUL_RIGHT, glm::vec2(0.0625f * 1, 0.0625f * 9));

	sprite->setAnimationSpeed(LAND_CAUDECUL_LEFT, 6);
	sprite->addKeyframe(LAND_CAUDECUL_LEFT, glm::vec2(0.0625f * 0, 0.0625f * 8));
	sprite->addKeyframe(LAND_CAUDECUL_LEFT, glm::vec2(0.0625f * 1, 0.0625f * 8));
	sprite->addKeyframe(LAND_CAUDECUL_LEFT, glm::vec2(0.0625f * 1, 0.0625f * 8));
	sprite->addKeyframe(LAND_CAUDECUL_LEFT, glm::vec2(0.0625f * 1, 0.0625f * 8));


	sprite->setAnimationSpeed(LOST_LIFE_LEFT, 2);
	sprite->addKeyframe(LOST_LIFE_LEFT, glm::vec2(0.0625f * 3, 0.0625f * 8));

	sprite->setAnimationSpeed(LOST_LIFE_RIGHT, 2);
	sprite->addKeyframe(LOST_LIFE_RIGHT, glm::vec2(0.0625f * 3, 0.0625f * 9));

	sprite->setAnimationSpeed(DEAD, 8);
	sprite->addKeyframe(DEAD, glm::vec2(0.0625f * 2, 0.0625f * 10));

	sprite->setAnimationSpeed(FRENAR_RIGHT, 2);
	sprite->addKeyframe(FRENAR_RIGHT, glm::vec2(0.0625f * 2, 0.0625f * 9));


	sprite->setAnimationSpeed(FRENAR_LEFT, 2);
	sprite->addKeyframe(FRENAR_LEFT, glm::vec2(0.0625f * 2, 0.0625f * 8));


	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + getPosHitBox().x), float(tileMapDispl.y + getPosHitBox().y)));

}

void Player::update(int deltaTime)
{
	bool enlaire = false;
	sprite->update(deltaTime);
	crouching = false;
	entraporta = false;

	



	if (lastHitTime > 0) {
		lastHitTime -= deltaTime;
	}
	if (Game::instance().getKey(GLFW_KEY_A)) {
		if (posiblePickRock != NULL && facingDirection == LEFT && !pickingRock) {
			changeAnimation(READY_TO_PICK_LEFT);
			if (Game::instance().getKey(GLFW_KEY_X))
			{
				if (soundEngine && !pickingRock) {
					soundEngine->play2D("media/sounds/pick.ogg", false);
				}
				pickingRock = true;
				posiblePickRock->pickUp();
			}


		}
		else {
			facingDirection = LEFT;
			if (!pickingRock) {
				crouching = false;
				//if (!cauDeCul && !climbing) {
					if (-2 < 0) {
						speedX = std::max(speedX - accelerationX, -maxSpeedX);
					}
					else if (-2 > 0) {
						speedX = std::min(speedX + accelerationX, maxSpeedX);
					}
					if (!cauDeCul && !climbing && sprite->animation() != MOVE_LEFT) {
						sprite->changeAnimation(MOVE_LEFT);
					}
					posPlayer.x += int(speedX);
					if ((-2 < 0 && map->collisionMoveLeft(getPosHitBox(), getSizeHitBox())) ||
						(-2 > 0 && map->collisionMoveRight(getPosHitBox(), getSizeHitBox()))) {
						posPlayer.x -= int(speedX);
						speedX = 0.0f;
					}

				//}
			}
			else if (pickingRock) {
				crouching = false;
				if (!cauDeCul && !climbing) {
					if (-2 < 0) {
						speedX = std::max(speedX - accelerationX, -maxSpeedX);
					}
					else if (-2 > 0) {
						speedX = std::min(speedX + accelerationX, maxSpeedX);
					}
					if (sprite->animation() != MOVE_PICK_LEFT) {
						sprite->changeAnimation(MOVE_PICK_LEFT);
					}
					posPlayer.x += int(speedX);
					if ((-2 < 0 && map->collisionMoveLeft(getPosHitBox(), getSizeHitBox())) ||
						(-2 > 0 && map->collisionMoveRight(getPosHitBox(), getSizeHitBox()))) {
						posPlayer.x -= int(speedX);
						speedX = 0.0f;
					}
				}
			}
		}
	}
	else if (Game::instance().getKey(GLFW_KEY_D)) {
		if (posiblePickRock != NULL && facingDirection == RIGHT && !pickingRock) {
			changeAnimation(READY_TO_PICK_RIGHT);
			if (Game::instance().getKey(GLFW_KEY_X))
			{
				if (soundEngine && !pickingRock) {
					soundEngine->play2D("media/sounds/pick.ogg", false);
				}
				pickingRock = true;
				posiblePickRock->pickUp();

			}
		}
		else {
			facingDirection = RIGHT;
			if (!pickingRock) {
				crouching = false;
				//if (!cauDeCul && !climbing) {
					if (2 < 0) {
						speedX = std::max(speedX - accelerationX, -maxSpeedX);
					}
					else if (2 > 0) {
						speedX = std::min(speedX + accelerationX, maxSpeedX);
					}
					if (!cauDeCul && !climbing && sprite->animation() != MOVE_RIGHT) {
						sprite->changeAnimation(MOVE_RIGHT);
					}
					posPlayer.x += int(speedX);
					if ((2 < 0 && map->collisionMoveLeft(getPosHitBox(), getSizeHitBox())) ||
						(2 > 0 && map->collisionMoveRight(getPosHitBox(), getSizeHitBox()))) {
						posPlayer.x -= int(speedX);
						speedX = 0.0f;
					}
				//}
			}
			else if (pickingRock) {
				crouching = false;
				if (!cauDeCul && !climbing) {
					if (2 < 0) {
						speedX = std::max(speedX - accelerationX, -maxSpeedX);
					}
					else if (2 > 0) {
						speedX = std::min(speedX + accelerationX, maxSpeedX);
					}
					if (sprite->animation() != MOVE_PICK_RIGHT) {
						sprite->changeAnimation(MOVE_PICK_RIGHT);
					}
					posPlayer.x += int(speedX);
					if ((2 < 0 && map->collisionMoveLeft(getPosHitBox(), getSizeHitBox())) ||
						(2 > 0 && map->collisionMoveRight(getPosHitBox(), getSizeHitBox()))) {
						posPlayer.x -= int(speedX);
						speedX = 0.0f;
					}
				}
			}
		}
	}
	else if (Game::instance().getKey(GLFW_KEY_S) && !bJumping) {
		if (Game::instance().getKey(GLFW_KEY_S) && map->isStair(getPosHitBox() + glm::ivec2(0, 1), getSizeHitBox()) && map->isStair(getPosHitBox() + glm::ivec2(25, 1), getSizeHitBox())) {
			changeAnimation(CLIMB);
			climbing = true;
			posPlayer.y += 2;
		}
		else if (map->collisionMoveDown(getPosHitBox(), getSizeHitBox(), &posPlayer.y, glm::ivec2(72, 95)) && !climbing) {
			crouching = true;
		}

		if (facingDirection == RIGHT && !climbing && sKeyReleased)
			sprite->changeAnimation(CROUCH_RIGHT);
		else if (facingDirection == LEFT && !climbing && sKeyReleased)
			sprite->changeAnimation(CROUCH_LEFT);
	}
	else if ((!bJumping && !cauDeCul && !crouching && !climbing)) {
		crouching = false;
		if (speedX != 0.0f) {
			if (speedX > 0.0f) {
				speedX = std::max(0.0f, speedX - decelerationX);
				if (!pickingRock)
					changeAnimation(FRENAR_RIGHT);

				if (map->collisionMoveRight(getPosHitBox(), getSizeHitBox())) {
					speedX = 0.0f;
				}
			}

			else {
				speedX = std::min(0.0f, speedX + decelerationX);
				if (!pickingRock)
					changeAnimation(FRENAR_LEFT);

				if (map->collisionMoveLeft(getPosHitBox(), getSizeHitBox())) {
					speedX = 0.0f;
				}
			}
			posPlayer.x += int(speedX);

			if (speedX == 0.0f && !bJumping && !cauDeCul && !crouching && !climbing && !pickingRock) {
				changeAnimation(facingDirection == LEFT ? STAND_LEFT : STAND_RIGHT);
			}
			else if (speedX == 0.0f && !bJumping && !cauDeCul && !crouching && !climbing && pickingRock) {
				changeAnimation(facingDirection == LEFT ? MOVE_PICK_LEFT : MOVE_PICK_RIGHT);
			}
		}
		else if (!bJumping && !cauDeCul && !crouching && !climbing && !pickingRock) {
			changeAnimation(facingDirection == LEFT ? STAND_LEFT : STAND_RIGHT);
		}
		else if (!bJumping && !cauDeCul && !crouching && !climbing && pickingRock) {
			changeAnimation(facingDirection == LEFT ? STAND_PICK_LEFT : STAND_PICK_RIGHT);
		}
	}

	if (map->isLake(getPosHitBox() + glm::ivec2(8, 1), getSizeHitBox() + glm::ivec2(-20, -40))) {
		Dead();
		lakehit = true;
	}

	if (pickingRock && Game::instance().getKey(GLFW_KEY_Z))
	{
		pickingRock = false;
		if (facingDirection == LEFT)
			posiblePickRock->throwRock(glm::vec2(-1, 0), 0.4f);
		else
			posiblePickRock->throwRock(glm::vec2(1, 0), 0.4f);
		posiblePickRock = NULL;

		if (soundEngine) {
			soundEngine->play2D("media/sounds/throw.ogg", false);
		}
	}

	//if (map->collisionWithRock(getPosHitBox() + glm::ivec2(0, 64), getSizeHitBox()))
	//{
	//	//std::cout << "rock down" << std::endl;
	//}


	if (climbing && !map->isStair(getPosHitBox(), getSizeHitBox())) {
		climbing = false;
	}
	else if (!climbing && map->isStair(getPosHitBox(), getSizeHitBox())) {
		sprite->changeAnimation(CLIMB);
	}



	if (Game::instance().getKey(GLFW_KEY_W) && map->isStair(getPosHitBox(), getSizeHitBox()) && !map->collisionMoveUp(getPosHitBox(), getSizeHitBox(), &posPlayer.y)) {
		if (!climbing)
			sprite->changeAnimation(CLIMB);
		climbing = true;
		bJumping = false;
		posPlayer.y -= 2;
		if (posPlayer.y % 32 == 0)
			soundEngine->play2D("media/sounds/stairs.ogg", false);
	}

	if (cauDeCul && (enemykilled || destroyrock))
	{
		//std::cout << "ENTRA" << std::endl;
		if (enemykilled) {
			enemykilled = false;
			if (soundEngine) {
				soundEngine->play2D("media/sounds/killSolid.ogg", false);
			}
		}
		else if (destroyrock) {
			destroyrock = false;
			if (soundEngine) {
				soundEngine->play2D("media/sounds/bounce.ogg", false);
			}
		}

		bJumping = true;
		jumpAngle = 0;
		startY = posPlayer.y;

		
	}


	if (bJumping)
	{
		//std::cout << "JUMPING" << std::endl;
		enlaire = true;
		if (Game::instance().getKey(GLFW_KEY_S))
		{
			cauDeCul = true;
			sKeyReleased = false;
			sprite->changeAnimation(facingDirection == LEFT ? CAUDECUL_LEFT : CAUDECUL_RIGHT);
		}
		if (!Game::instance().getKey(GLFW_KEY_S)) {
			sKeyReleased = true;
		}
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle == 180)
		{

			bJumping = false;
			posPlayer.y = startY;

		}
		else
		{
			posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle < 90)
			{
				if (map->collisionMoveUp(getPosHitBox(), getSizeHitBox(), &posPlayer.y))
				{
					//jumpAngle = 180 - jumpAngle;
					//posPlayer.y += FALL_STEP;
					bJumping = false;
					cauDeCul = false;
				}
				if (!cauDeCul && !pickingRock) {
					sprite->changeAnimation(facingDirection == LEFT ? JUMP_LEFT : JUMP_RIGHT);
				}
				else if (!cauDeCul && pickingRock) {
					//std::cout << "JUMPING" << std::endl;
					sprite->changeAnimation(facingDirection == LEFT ? JUMP_PICK_LEFT : JUMP_PICK_RIGHT);
				}
			}
			else
			{
				if (!cauDeCul && !pickingRock) {
					sprite->changeAnimation(facingDirection == LEFT ? FALL_LEFT : FALL_RIGHT);
				}
				else if (!cauDeCul && pickingRock) {
					//std::cout << "FALLING. Animation: " << sprite->animation() << std::endl;
					sprite->changeAnimation(facingDirection == LEFT ? FALL_PICK_LEFT : FALL_PICK_RIGHT);
				}
			}


			if (jumpAngle > 90 && map->collisionMoveDown(getPosHitBox(), getSizeHitBox(), &posPlayer.y, glm::ivec2(72, 96)))
			{
				bJumping = false;
				cauDeCul = false;
				if (!pickingRock && !cauDeCul) {
					//std::cout << "LAND" << std::endl;
					sprite->changeAnimation(facingDirection == LEFT ? LAND_LEFT : LAND_RIGHT);
				}
				else if (pickingRock && !cauDeCul){
					sprite->changeAnimation(facingDirection == LEFT ? LAND_PICK_LEFT : LAND_PICK_RIGHT);
				}
				
			}
		}
	}

	if (!climbing && !bJumping) {
		posPlayer.y += FALL_STEP;
		isLanding = false;
		if (map->collisionMoveDown(getPosHitBox(), getSizeHitBox(), &posPlayer.y, glm::ivec2(72, 96)))
		{
			//std::cout << sKeyReleased << std::endl;
			
			if (cauDeCul) {
				if (sprite->animation() == CAUDECUL_LEFT || sprite->animation() == CAUDECUL_RIGHT)
				{
					// Començar l'animació d'aterratge si encara no ho est� fent
					if (!isLanding)
					{
						sprite->changeAnimation(facingDirection == LEFT ? LAND_CAUDECUL_LEFT : LAND_CAUDECUL_RIGHT);
						isLanding = true;  // Estableix l'estat d'aterratge
						cauDeCul = false;
						sprite->changeAnimation(facingDirection == LEFT ? STAND_LEFT : STAND_RIGHT);
						
					}
				}

			}
			cauDeCul = false;


			//std::cout << "ENTRA" << std::endl;
			//std::cout << cauDeCul << std::endl;
			if (sprite->animation() == FALL_LEFT || sprite->animation() == FALL_RIGHT)
				sprite->changeAnimation(facingDirection == LEFT ? LAND_LEFT : LAND_RIGHT);
				
			if (sprite->animation() == FALL_PICK_LEFT || sprite->animation() == FALL_PICK_RIGHT)
				sprite->changeAnimation(facingDirection == LEFT ? LAND_PICK_LEFT : LAND_PICK_RIGHT);
				
			if (Game::instance().getKey(GLFW_KEY_W) && wKeyReleased)
			{
				wKeyReleased = false;
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
				if (!pickingRock) {
					sprite->changeAnimation(facingDirection == LEFT ? JUMP_LEFT : JUMP_RIGHT);
				}
				else {
					sprite->changeAnimation(facingDirection == LEFT ? JUMP_PICK_LEFT : JUMP_PICK_RIGHT);

				}

				if (soundEngine) {
					soundEngine->play2D("media/sounds/jump.ogg", false);
				}
			}
			if (!Game::instance().getKey(GLFW_KEY_W)) {
				wKeyReleased = true;
			}
		}
		else
		{
			if (!cauDeCul && !pickingRock) {
				sprite->changeAnimation(facingDirection == LEFT ? FALL_LEFT : FALL_RIGHT);
			}
			else if (!cauDeCul && pickingRock) {
				std::cout << sprite->animation() << std::endl;
				sprite->changeAnimation(facingDirection == LEFT ? FALL_PICK_LEFT : FALL_PICK_RIGHT);
			}else if (cauDeCul && !pickingRock){
			}

		}
		
		
	}

	if (pickingRock)
	{
		int facingDirectionPlayer = 0;
		if (facingDirection == RIGHT)
			facingDirectionPlayer = 1;
		posiblePickRock->updatePlayerPosition(posPlayer, facingDirectionPlayer);
		if (Game::instance().getKey(GLFW_KEY_S)) {
			sprite->changeAnimation(facingDirection == LEFT ? STAND_PICK_LEFT : STAND_PICK_RIGHT);
		}
	}

	if (Game::instance().getKey(GLFW_KEY_W) && map->isDoor(getPosHitBox() + glm::ivec2(0, 0), getSizeHitBox())) {
		entraporta = true;
	}

	
	if (Game::instance().getKey(GLFW_KEY_H)) {
		restartVides();
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}


void Player::render()
{
	sprite->render();
}


void Player::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::vec2 Player::getPosition()
{
	return posPlayer;
}

glm::ivec2 Player::getPosHitBox()
{
	glm::ivec2 posHB;
	if (!crouching) {
		posHB = posPlayer + glm::ivec2(20, 34);
		
	}

	else {
		posHB = posPlayer + glm::ivec2(10, 54);
	}
	return posHB;
}

glm::ivec2 Player::getSizeHitBox()
{
	glm::ivec2 sizeHB;
	if (crouching)
		sizeHB = glm::ivec2(46, 42);
	else if (climbing)
		sizeHB = glm::ivec2(28, 62);
	else
		sizeHB = glm::ivec2(30, 62);

	return sizeHB;
}

void Player::setPosiblePickRock(Rock* rock)
{
	if (!pickingRock)
		posiblePickRock = rock;
}


int Player::getFacingDirection() {
	if (facingDirection == LEFT)
		return 0;
	else
		return 1;
}

void Player::changeAnimation(int animation) {
	if (sprite->animation() != animation) {
		sprite->changeAnimation(animation);
	}
}



bool Player::estaCaienDeCul()
{
	return cauDeCul;
}

void Player::enemyKilled()
{
	enemykilled = true;
}

void Player::destroyRock()
{
	destroyrock = true;
}


void Player::lostLife(float enemypos, float enemysize) {
	if (isLosingLife) return; // Prevents animation from re-triggering

	const float jumpDistance = 30.0f;
	const float jumpHeight = 40.0f;

	if (enemypos <= posPlayer.x) {
		sprite->changeAnimation(LOST_LIFE_LEFT);
		posPlayer.x += jumpDistance;
		sprite->changeAnimation(LOST_LIFE_LEFT);
	}
	else {
		sprite->changeAnimation(LOST_LIFE_RIGHT);
		posPlayer.x -= jumpDistance;
		sprite->changeAnimation(LOST_LIFE_RIGHT);
	}

	posPlayer.y -= jumpHeight;
	isLosingLife = true; // Set flag to indicate the player is in "lost life" state

}


bool Player::entraPorta() {

	return entraporta;
}

void Player::Dead()
{
	sprite->changeAnimation(DEAD);
}

void Player::Hit()
{
	sprite->changeAnimation(facingDirection == LEFT ? LOST_LIFE_LEFT : LOST_LIFE_RIGHT);
}

int Player::getVides()
{
	return vides;
}

int Player::getTries()
{
	return tries;
}

int Player::getScore()
{
	return score;
}

void Player::restarVida(float enemypos, float enemysize)
{
	if (lastHitTime <= 0)
	{
		soundEngine->play2D("media/sounds/explode.ogg", false);
		--vides;
		lastHitTime = cooldownTime;
		lostLife(enemypos, enemysize);
	}
}

void Player::restarTry()
{
	if (tries > 0)
	{
		--tries;
		vides = 3;
	}
}

void Player::restartVides()
{
		vides = 3;
}

void Player::sumarPunts(int punts)
{
	score += punts;
}

void Player::sumarVida()
{
	if (vides < 3)
		++vides;
	else
		sumarPunts(20);
}

bool Player::inlake() {
	return lakehit;
}