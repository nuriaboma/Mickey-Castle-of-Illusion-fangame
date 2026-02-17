#include "Boss.h"
#include <iostream>
#include "Audio.h"

void Boss::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	vides = 4;
	cooldownTime = 2000;
	lastHitTime = 0;
	posBoss = glm::ivec2(0.f, 0.f);
	if (!spritesheet.loadFromFile("images/boss.png", TEXTURE_PIXEL_FORMAT_RGBA)) {
		std::cout << "Error cargando la textura" << std::endl;
	}
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(160, 240), glm::vec2(0.25f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(0, 2);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(0, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(0, glm::vec2(0.5f, 0.f));


	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;

}


void Boss::update(int deltaTime)
{
	if (lastHitTime > 0) {
		lastHitTime -= deltaTime;
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBoss.x), float(tileMapDispl.y + posBoss.y)));
	sprite->update(deltaTime);
}

void Boss::render() const
{
	if (vides > 0)
		sprite->render();
}

void Boss::setPosition(const glm::ivec2& pos)
{
	posBoss = pos;
}

void Boss::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

glm::ivec2 Boss::getSize() const
{
	return glm::ivec2(160, 240);
}

glm::ivec2 Boss::getPosition() const
{
	return posBoss;
}

void Boss::restarVida()
{
	if (vides > 0 && lastHitTime <= 0)
	{
		soundEngine->play2D("media/sounds/explode.ogg", false);
		--vides;
		lastHitTime = cooldownTime;
	}
}

int Boss::getVides()
{
	return vides;
}