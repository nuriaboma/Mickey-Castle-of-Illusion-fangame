#include "Enemy2.h"
#include "Game.h"
#include <iostream>


enum Enemy2Anims
{
    STAND_LEFT, STAND_RIGHT, WALK_LEFT, WALK_RIGHT, DIE_LEFT, DIE_RIGHT, REVIVE_RIGHT, REVIVE_LEFT
};

enum DirectionEnemy2 { LEFT, RIGHT };
enum EnemyState { ALIVE, DYING, DEAD, REVIVING };

DirectionEnemy2 facingDirectionEnemy;
EnemyState enemyState;
bool collisionenemy;
float timeSinceDeath;


void Enemy2::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{

    facingDirectionEnemy = LEFT;
    enemyState = ALIVE;
    collisionenemy = false;
    killedbyplayer = false;
    timeSinceDeath = 0.0f;

    spritesheet.loadFromFile("images/soldat2.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);
    sprite = Sprite::createSprite(glm::ivec2(48, 64), glm::vec2(0.125f, 0.5f), &spritesheet, &shaderProgram);

    sprite->setNumberAnimations(8);
    sprite->setAnimationSpeed(WALK_RIGHT, 6);
    sprite->addKeyframe(WALK_RIGHT, glm::vec2(0.0f, 0.0f));
    sprite->addKeyframe(WALK_RIGHT, glm::vec2(0.125f, 0.0f));
    sprite->addKeyframe(WALK_RIGHT, glm::vec2(0.25f, 0.0f));

    sprite->setAnimationSpeed(STAND_RIGHT, 1);
    sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.125f, 0.0f));

    sprite->setAnimationSpeed(WALK_LEFT, 6);
    sprite->addKeyframe(WALK_LEFT, glm::vec2(0.25f, 0.5f));
    sprite->addKeyframe(WALK_LEFT, glm::vec2(0.125f, 0.5f));
    sprite->addKeyframe(WALK_LEFT, glm::vec2(0.0f, 0.5f));

    sprite->setAnimationSpeed(STAND_LEFT, 1);
    sprite->addKeyframe(STAND_LEFT, glm::vec2(0.125f, 0.5f));

    sprite->setAnimationSpeed(DIE_RIGHT, 4);
    sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.375f, 0.0f));
    sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.5f, 0.0f));
    sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.625f, 0.0f));
    sprite->addKeyframe(DIE_RIGHT, glm::vec2(0.75f, 0.0f));

    sprite->setAnimationSpeed(DIE_LEFT, 4);
    sprite->addKeyframe(DIE_LEFT, glm::vec2(0.375f, 0.5f));
    sprite->addKeyframe(DIE_LEFT, glm::vec2(0.5f, 0.5f));
    sprite->addKeyframe(DIE_LEFT, glm::vec2(0.625f, 0.5f));
    sprite->addKeyframe(DIE_LEFT, glm::vec2(0.75f, 0.5f));

    sprite->setAnimationSpeed(REVIVE_RIGHT, 4);
    sprite->addKeyframe(REVIVE_RIGHT, glm::vec2(0.75f, 0.0f));
    sprite->addKeyframe(REVIVE_RIGHT, glm::vec2(0.625f, 0.0f));
    sprite->addKeyframe(REVIVE_RIGHT, glm::vec2(0.5f, 0.0f));
    sprite->addKeyframe(REVIVE_RIGHT, glm::vec2(0.375f, 0.0f));

    sprite->setAnimationSpeed(REVIVE_LEFT, 4);
    sprite->addKeyframe(REVIVE_LEFT, glm::vec2(0.75f, 0.5f));
    sprite->addKeyframe(REVIVE_LEFT, glm::vec2(0.625f, 0.5f));
    sprite->addKeyframe(REVIVE_LEFT, glm::vec2(0.5f, 0.5f));
    sprite->addKeyframe(REVIVE_LEFT, glm::vec2(0.375f, 0.5f));

    sprite->setAnimationSpeed(WALK_LEFT, 3);
    sprite->setAnimationSpeed(WALK_RIGHT, 3);



    if (facingDirectionEnemy == RIGHT)
        sprite->changeAnimation(WALK_RIGHT);
    else
        sprite->changeAnimation(WALK_LEFT);


    tileMapDispl = tileMapPos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}


void Enemy2::update(int deltaTime)
{
    sprite->update(deltaTime);

    glm::vec2 enemyPos = posEnemy;

    if (enemyState == ALIVE) {
        if (facingDirectionEnemy == LEFT) {
            enemyPos.x -= 0.05 * deltaTime;

            if (map->collisionMoveLeft(enemyPos, glm::ivec2(48, 64))) {
                facingDirectionEnemy = RIGHT;
                sprite->changeAnimation(WALK_RIGHT);
            }
        }
        else if (facingDirectionEnemy == RIGHT) {
            enemyPos.x += 0.1 * deltaTime;

            if (map->collisionMoveRight(enemyPos, glm::ivec2(48, 64))) {
                facingDirectionEnemy = LEFT;
                sprite->changeAnimation(WALK_LEFT);
            }
        }
    }
    else if (enemyState == DYING) {
        if (sprite->isAnimationFinished()) {
            enemyState = DEAD;
            sprite->changeAnimation(facingDirectionEnemy == LEFT ? REVIVE_LEFT : REVIVE_RIGHT); 
        }
    }
    else if (enemyState == DEAD) {
        if (sprite->isAnimationFinished()) {
            enemyState = REVIVING;
        }
    }
    else if (enemyState == REVIVING) {
        if (sprite->isAnimationFinished()) {
            enemyState = ALIVE;
            if (facingDirectionEnemy == RIGHT)
                sprite->changeAnimation(WALK_RIGHT);
            else 
                sprite->changeAnimation(WALK_LEFT);
        }
    }

    setPosition(enemyPos);
}


void Enemy2::render()
{
	sprite->render();
}

void Enemy2::setPosition(const glm::vec2 &pos)
{
	posEnemy = pos;
	if (startY == 0.0)
		startY = posEnemy.y;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Enemy2::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

glm::vec2 Enemy2::getSize()
{
	return glm::vec2(36.f, 60.f);
}

glm::vec2 Enemy2::getPosition()
{
	return posEnemy;
}

void Enemy2::Killed()
{
    if (enemyState == ALIVE) {
        enemyState = DYING;
        if (facingDirectionEnemy == RIGHT)
            sprite->changeAnimation(DIE_RIGHT);
        else
            sprite->changeAnimation(DIE_LEFT);
    }
}

bool Enemy2::isAlive()
{
    return enemyState == ALIVE;
}