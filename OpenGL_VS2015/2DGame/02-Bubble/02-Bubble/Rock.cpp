#include "Rock.h"
#include <iostream>

enum ObjectAnims
{
	ROCK1, ROCK2, CHEST, CAKE, COIN, GEMA
};

void Rock::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, const int objectType)
{
	pickedUp = false;
	isThrown = false;
	velocity = glm::vec2(0.f, 0.f);
	playerFacingDirection = 0;
	glm::vec2 posPlayer = glm::vec2(0.f, 0.f);
	falling = glm::vec2(0.f, 0.f);
	type = objectType;
	deleteRock = false;

	if (type == 5)
		isThrown = true;

	srand(static_cast<unsigned int>(time(nullptr)));
	if (!spritesheet.loadFromFile("images/objects.png", TEXTURE_PIXEL_FORMAT_RGBA)) {
		std::cout << "Error cargando la textura" << std::endl;
	}
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(6);

		sprite->setAnimationSpeed(ROCK1, 8);
		sprite->addKeyframe(ROCK1, glm::vec2(0.125f * 0, 0.f));

		sprite->setAnimationSpeed(ROCK2, 8);
		sprite->addKeyframe(ROCK2, glm::vec2(0.125f * 1, 0.f));

		sprite->setAnimationSpeed(CHEST, 8);
		sprite->addKeyframe(CHEST, glm::vec2(0.125f * 2, 0.f));

		sprite->setAnimationSpeed(CAKE, 8);
		sprite->addKeyframe(CAKE, glm::vec2(0.125f * 3, 0.f));

		sprite->setAnimationSpeed(COIN, 8);
		sprite->addKeyframe(COIN, glm::vec2(0.125f * 4, 0.f));

		sprite->setAnimationSpeed(GEMA, 8);
		sprite->addKeyframe(GEMA, glm::vec2(0.125f * 5, 0.f));



	if (type == 0)
		sprite->changeAnimation(ROCK2);
	else if (type == 1)
		sprite->changeAnimation(ROCK1);
	else if (type == 2)
		sprite->changeAnimation(CHEST);
	else if (type == 3)
		sprite->changeAnimation(CAKE);
	else if (type == 4)
		sprite->changeAnimation(COIN);
	else if (type == 5)
		sprite->changeAnimation(GEMA);
	tileMapDispl = tileMapPos;
	
}




void Rock::update(int deltaTime)
{
	if (isThrown) {
		falling += glm::vec2(0, 0.01);
		posRock += glm::ivec2((velocity + falling) * float(deltaTime));
		glm::vec2 rockSize = glm::ivec2(32, 32);
		
		if (map->collisionMoveDown(posRock, rockSize, &posRock.y, rockSize)) {
			if (type == 0)
			{
				deleteRock = true;
				return; 
			}
			else if (type == 1)
			{
				isThrown = false;
				map->addRock(posRock, type);
			}
			else if (type == 2)
			{
				isThrown = false;
				destroyChest();
			}
			else 
			{
				isThrown = false;
			}
			falling = glm::vec2(0.f, 0.f);
		}
		else if (map->collisionMoveLeft(posRock, rockSize) || map->collisionMoveRight(posRock, rockSize))
		{
			if (type == 0)
			{
				deleteRock = true;
				return;
			}

			else if (type == 1)
			{
				posRock -= glm::ivec2((velocity + falling) * float(deltaTime));
				velocity = glm::vec2(0.f, 0.f);
			}

			else if (type == 2)
			{
				posRock -= glm::ivec2((velocity + falling) * float(deltaTime));
				velocity = glm::vec2(0.f, 0.f);
				destroyChest();
			}
		}
	}

	else if (pickedUp) {
		if (playerFacingDirection == 0) // esquerra
		{
			posRock = posPlayer + glm::vec2(14.f, 17.f);
		}
		else // dreta
		{
			posRock = posPlayer + glm::vec2(25.f, 17.f);
		}

	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posRock.x), float(tileMapDispl.y + posRock.y)));
	sprite->update(deltaTime);
}

void Rock::render() const
{
	sprite->render();
}

void Rock::setPosition(const glm::ivec2 &pos)
{
	posRock = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posRock.x), float(tileMapDispl.y + posRock.y)));
}

void Rock::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

glm::ivec2 Rock::getSize() const
{
	return glm::ivec2(32, 32); // Tamaño de la roca
}

glm::ivec2 Rock::getPosition() const
{
	return posRock;
}

bool Rock::isPickedUp() const
{
	return pickedUp;
}

void Rock::pickUp()
{
	pickedUp = true;
	isThrown = false;
	map->deleteRock(posRock);
}

void Rock::throwRock(const glm::vec2 &direction, float speed)
{
	if (pickedUp) {
		pickedUp = false;
		isThrown = true;
		velocity = direction * speed;
		falling = glm::vec2(0.f, 0.f);
	}
}

void Rock::updatePlayerPosition(glm::vec2 position, int faceDir)
{
	posPlayer = position;
	playerFacingDirection = faceDir;
}

bool Rock::isDeleted()
{
	return deleteRock;
}

int Rock::getType()
{
	return type;
}

void Rock::destroyChest()
{
	int randomType = rand() % 2;

	if (randomType == 0) {
		type = 3;
		sprite->changeAnimation(CAKE);
	}
	else {
		type = 4;
		sprite->changeAnimation(COIN);
	}
}

bool Rock::thrown()
{
	return isThrown;
}