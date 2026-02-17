#include "BolaBoss.h"
#include <iostream>


void BolaBoss::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, const glm::vec2& direction, float speed)
{
	isThrown = false;
	velocity = glm::vec2(0.f, 0.f);
	posBola = glm::ivec2(0.f, 0.f);
	startPos = glm::ivec2(0.f, 0.f);
	velocity = direction * speed;
	if (!spritesheet.loadFromFile("images/bolaBoss.png", TEXTURE_PIXEL_FORMAT_RGBA)) {
		std::cout << "Error cargando la textura" << std::endl;
	}
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));


	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;

}




void BolaBoss::update(int deltaTime)
{
	if (isThrown) {
		posBola += glm::ivec2((velocity) * float(deltaTime));
		glm::vec2 bolaSize = getSize();

		if (map->collisionMoveDown(posBola, bolaSize, &posBola.y, bolaSize) ||
			map->collisionMoveLeft(posBola, bolaSize) || 
			map->collisionMoveRight(posBola, bolaSize)) 
		{
			isThrown = false;
		}
	}

	else {
		posBola = startPos;
		isThrown = true;
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBola.x), float(tileMapDispl.y + posBola.y)));
	sprite->update(deltaTime);
}

void BolaBoss::render() const
{
	if (isThrown)
		sprite->render();
}

void BolaBoss::setStartPosition(const glm::ivec2& pos)
{
	startPos = pos;
}

void BolaBoss::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

glm::ivec2 BolaBoss::getSize() const
{
	return glm::ivec2(16, 16);
}

glm::ivec2 BolaBoss::getPosition() const
{
	return posBola;
}
