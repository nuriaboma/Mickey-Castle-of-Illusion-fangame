#include "Enemy.h"
#include "Game.h"
#include "Audio.h"

enum EnemyAnims
{
    FACE_UP, FACE_DOWN, CHAIN
};

const int MAX_CHAIN_LENGTH = 50; // O el valor que consideris adequat


void Enemy::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
    isFalling = false;
    isAscending = false;
    isWaiting = false;
    isAbove = true;
    tileMapDispl = tileMapPos;
    fallVelocity = glm::vec2(0.f, 0.f);
    waitTime = 1600; // Temps d'espera en mil·lisegons
    waitTimer = 0; // Temporitzador d'espera
    chainLength = 5; // Inicialment la cadena és curta

    spritesheet.loadFromFile("images/cau3.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);
    sprite = Sprite::createSprite(glm::ivec2(48, 48), glm::vec2(0.5, 0.5), &spritesheet, &shaderProgram);

    sprite->setNumberAnimations(3);


    sprite->setAnimationSpeed(FACE_UP, 1);
    sprite->addKeyframe(FACE_UP, glm::vec2(0.0f, 0.0f));

    sprite->setAnimationSpeed(FACE_DOWN, 1);
    sprite->addKeyframe(FACE_DOWN, glm::vec2(0.5f, 0.0f));

    //sprite->setAnimationSpeed(0, 8);
    sprite->changeAnimation(0);

    startY = 0.0;
}


void Enemy::update(int deltaTime)
{
    if (isFalling)
    {
        // Actualització de la velocitat de caiguda
        fallVelocity.y += 0.0005f * deltaTime;
        posEnemy.y += fallVelocity.y * deltaTime;

        // Comprova les col·lisions
        if (map->collisionMoveDown(posEnemy, glm::ivec2(48, 48), &posEnemy.y, glm::ivec2(55, 55)))
        {
            isFalling = false;
            fallVelocity.y = 0.f;
            isWaiting = true;
            waitTimer = 0;
            soundEngine->play2D("media/sounds/explode.ogg", false);
            // Estableix la longitud de la cadena a la distància caiguda
            chainLength = posEnemy.y - startY;
            std::cout << chainLength << std::endl;

        }
    }
    else if (isWaiting)
    {
        // Lògica d'espera
        waitTimer += deltaTime;

        if (waitTimer >= waitTime)
        {
            isWaiting = false;
            isAscending = true;
        }
    }
    else if (isAscending)
    {
        if (posEnemy.y > startY)
        {
            posEnemy.y -= 0.1f * deltaTime;
        }

        // No redueixis la longitud de la cadena fins que l'ascens estigui complet
        if (posEnemy.y <= startY)
        {
            posEnemy.y = startY;
            isAscending = false;
        }
    }

    // Actualitza la posició del sprite
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x + 8), float(tileMapDispl.y + posEnemy.y+8)));
    sprite->update(deltaTime);
}





void Enemy::render()
{
    // Renderitza la tona
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x + 8),
        float(tileMapDispl.y + posEnemy.y + 8))); // Posició de la tona
    sprite->changeAnimation(isFalling ? FACE_DOWN : FACE_UP);
    sprite->render();
}




void Enemy::setPosition(const glm::vec2& pos)
{
    posEnemy = pos + glm::vec2(0, 8);
    if (startY == 0.0)
        startY = posEnemy.y;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Enemy::setTileMap(TileMap *tileMap)
{
    map = tileMap;
}

void Enemy::detectPlayer(const glm::vec2 &playerPosition)
{
    if (!isFalling && !isAscending && !isWaiting && playerPosition.x > posEnemy.x - 64 && playerPosition.x < posEnemy.x + 94)
    {
        isFalling = true;
    }
}


glm::vec2 Enemy::getSize()
{
    return glm::vec2(48.f, 48.f);
}

glm::vec2 Enemy::getPosition()
{
    return posEnemy;
}