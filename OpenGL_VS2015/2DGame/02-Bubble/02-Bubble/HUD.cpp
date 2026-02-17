#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "HUD.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 750

void HUD::init(ShaderProgram& shaderProgram) {
 
    backgroundTexture.loadFromFile("images/hud/blackBackground.png", TEXTURE_PIXEL_FORMAT_RGBA);
    background = Sprite::createSprite(glm::ivec2(1000, 70), glm::vec2(1.0f, 1.0f), &backgroundTexture, &shaderProgram);
    background->setPosition(glm::vec2(1952, 1344));

    powerTexture.loadFromFile("images/hud/power.png", TEXTURE_PIXEL_FORMAT_RGBA);
    power = Sprite::createSprite(glm::ivec2(172, 54), glm::vec2(1.0f, 1.0f), &powerTexture, &shaderProgram);
    power->setPosition(glm::vec2(1952, 1344));

    triesTexture.loadFromFile("images/hud/tries.png", TEXTURE_PIXEL_FORMAT_RGBA);
    tries = Sprite::createSprite(glm::ivec2(92, 54), glm::vec2(1.0f, 1.0f), &triesTexture, &shaderProgram);
    tries->setPosition(glm::vec2(1952, 1344));

    scoreTexture.loadFromFile("images/hud/score.png", TEXTURE_PIXEL_FORMAT_RGBA);
    score = Sprite::createSprite(glm::ivec2(108, 54), glm::vec2(1.0f, 1.0f), &scoreTexture, &shaderProgram);
    score->setPosition(glm::vec2(1952, 1344));

    timeTexture.loadFromFile("images/hud/time.png", TEXTURE_PIXEL_FORMAT_RGBA);
    time = Sprite::createSprite(glm::ivec2(76, 54), glm::vec2(1.0f, 1.0f), &timeTexture, &shaderProgram);
    time->setPosition(glm::vec2(1952, 1344));

    starTexture.loadFromFile("images/hud/star.png", TEXTURE_PIXEL_FORMAT_RGBA);
    for (int i = 0; i < 3; ++i) {
        Sprite* star = Sprite::createSprite(glm::ivec2(28, 28), glm::vec2(0.5f, 1.0f), &starTexture, &shaderProgram);
        star->setNumberAnimations(2);
        star->setAnimationSpeed(0, 4);
        star->addKeyframe(0, glm::vec2(0.f, 0.f));
   
        star->setAnimationSpeed(1, 4);
        star->addKeyframe(1, glm::vec2(0.5f, 0.f));
 
        star->changeAnimation(0);
        star->setPosition(glm::vec2(1952, 1344));
        lifeStars.push_back(star);
    }

    numsTexture.loadFromFile("images/hud/numbers.png", TEXTURE_PIXEL_FORMAT_RGBA);
    for (int i = 0; i < 2; ++i) {
        Sprite* num = Sprite::createSprite(glm::ivec2(11.75, 10.25), glm::vec2(0.0625f, 1.0f), &numsTexture, &shaderProgram);
        num->setNumberAnimations(10);
        num->setAnimationSpeed(0, 4);
        num->addKeyframe(0, glm::vec2(0.0625f * 0.f, 0.f));

        num->setAnimationSpeed(1, 4);
        num->addKeyframe(1, glm::vec2(0.0625f * 1.f, 0.f));

        num->setAnimationSpeed(2, 4);
        num->addKeyframe(2, glm::vec2(0.0625f * 2.f, 0.f));

        num->setAnimationSpeed(3, 4);
        num->addKeyframe(3, glm::vec2(0.0625f * 3.f, 0.f));

        num->setAnimationSpeed(4, 4);
        num->addKeyframe(4, glm::vec2(0.0625f * 4, 0.f));

        num->setAnimationSpeed(5, 4);
        num->addKeyframe(5, glm::vec2(0.0625f * 5.f, 0.f));

        num->setAnimationSpeed(6, 4);
        num->addKeyframe(6, glm::vec2(0.0625f * 6.f, 0.f));

        num->setAnimationSpeed(7, 4);
        num->addKeyframe(7, glm::vec2(0.0625f * 7.f, 0.f));

        num->setAnimationSpeed(8, 4);
        num->addKeyframe(8, glm::vec2(0.0625f * 8.f, 0.f));

        num->setAnimationSpeed(9, 4);
        num->addKeyframe(9, glm::vec2(0.0625f * 9.f, 0.f));

        num->changeAnimation(0);
        num->setPosition(glm::vec2(1952, 1344));
        triesNums.push_back(num);
    }

    for (int i = 0; i < 6; ++i) {
        Sprite* num = Sprite::createSprite(glm::ivec2(11.75, 10.25), glm::vec2(0.0625f, 1.0f), &numsTexture, &shaderProgram);
        num->setNumberAnimations(10);
        num->setAnimationSpeed(0, 4);
        num->addKeyframe(0, glm::vec2(0.0625f * 0.f, 0.f));

        num->setAnimationSpeed(1, 4);
        num->addKeyframe(1, glm::vec2(0.0625f * 1.f, 0.f));

        num->setAnimationSpeed(2, 4);
        num->addKeyframe(2, glm::vec2(0.0625f * 2.f, 0.f));

        num->setAnimationSpeed(3, 4);
        num->addKeyframe(3, glm::vec2(0.0625f * 3.f, 0.f));

        num->setAnimationSpeed(4, 4);
        num->addKeyframe(4, glm::vec2(0.0625f * 4, 0.f));

        num->setAnimationSpeed(5, 4);
        num->addKeyframe(5, glm::vec2(0.0625f * 5.f, 0.f));

        num->setAnimationSpeed(6, 4);
        num->addKeyframe(6, glm::vec2(0.0625f * 6.f, 0.f));

        num->setAnimationSpeed(7, 4);
        num->addKeyframe(7, glm::vec2(0.0625f * 7.f, 0.f));

        num->setAnimationSpeed(8, 4);
        num->addKeyframe(8, glm::vec2(0.0625f * 8.f, 0.f));

        num->setAnimationSpeed(9, 4);
        num->addKeyframe(9, glm::vec2(0.0625f * 9.f, 0.f));

        num->changeAnimation(0);
        num->setPosition(glm::vec2(1952, 1344));
        scoreNums.push_back(num);
    }

    for (int i = 0; i < 3; ++i) {
        Sprite* num = Sprite::createSprite(glm::ivec2(11.75, 10.25), glm::vec2(0.0625f, 1.0f), &numsTexture, &shaderProgram);
        num->setNumberAnimations(10);
        num->setAnimationSpeed(0, 4);
        num->addKeyframe(0, glm::vec2(0.0625f * 0.f, 0.f));

        num->setAnimationSpeed(1, 4);
        num->addKeyframe(1, glm::vec2(0.0625f * 1.f, 0.f));

        num->setAnimationSpeed(2, 4);
        num->addKeyframe(2, glm::vec2(0.0625f * 2.f, 0.f));

        num->setAnimationSpeed(3, 4);
        num->addKeyframe(3, glm::vec2(0.0625f * 3.f, 0.f));

        num->setAnimationSpeed(4, 4);
        num->addKeyframe(4, glm::vec2(0.0625f * 4, 0.f));

        num->setAnimationSpeed(5, 4);
        num->addKeyframe(5, glm::vec2(0.0625f * 5.f, 0.f));

        num->setAnimationSpeed(6, 4);
        num->addKeyframe(6, glm::vec2(0.0625f * 6.f, 0.f));

        num->setAnimationSpeed(7, 4);
        num->addKeyframe(7, glm::vec2(0.0625f * 7.f, 0.f));

        num->setAnimationSpeed(8, 4);
        num->addKeyframe(8, glm::vec2(0.0625f * 8.f, 0.f));

        num->setAnimationSpeed(9, 4);
        num->addKeyframe(9, glm::vec2(0.0625f * 9.f, 0.f));

        num->changeAnimation(0);
        num->setPosition(glm::vec2(1952, 1344));
        timeNums.push_back(num);
    }

}


void HUD::update(int currLives, int currTries, int currScoree, glm::vec2 cameraPosition, float currentTime) {
    currentLives = currLives;
    currentTries = currTries;
    currentScore = currScoree;
    glm::vec2 backgroundPosition = cameraPosition + glm::vec2(-280, 115);
    background->setPosition(backgroundPosition);

    glm::vec2 powerPosition = cameraPosition + glm::vec2(-230, 120);
    power->setPosition(powerPosition);

    glm::vec2 triesPosition = cameraPosition + glm::vec2(-52, 120);
    tries->setPosition(triesPosition);

    glm::vec2 scorePosition = cameraPosition + glm::vec2(46, 120);
    score->setPosition(scorePosition);

    glm::vec2 timePosition = cameraPosition + glm::vec2(160, 120);
    time->setPosition(timePosition);

    for (int i = 0; i < 3; ++i)
    {
        Sprite* star = lifeStars[i];
        glm::vec2 starPosition = cameraPosition + glm::vec2(-220 + i*31, 137);
        star->setPosition(glm::vec2(starPosition));
    }
    int triesPrint = currentTries;
    for (int i = 0; i < 2; ++i)
    {
        Sprite* num = triesNums[i];
        glm::vec2 triesPosition = cameraPosition + glm::vec2(-5 - i * 13, 148);
        num->setPosition(glm::vec2(triesPosition));
  
        int tryPrint = triesPrint % 10;
        triesPrint /= 10;
        num->changeAnimation(tryPrint);
       
    }

    int scoresPrint = currentScore;
    for (int i = 0; i < 6; ++i)
    {
        Sprite* num = scoreNums[i];
        glm::vec2 scoresPosition = cameraPosition + glm::vec2(126 - i * 13, 148);
        num->setPosition(glm::vec2(scoresPosition));

        int scorePrint = scoresPrint % 10;
        scoresPrint /= 10;
        num->changeAnimation(scorePrint);
    }

    int timesPrint = (int)currentTime;
    for (int i = 0; i < 3; ++i)
    {
        Sprite* num = timeNums[i];
        glm::vec2 timesPosition = cameraPosition + glm::vec2(205 - i * 13, 148);
        num->setPosition(glm::vec2(timesPosition));

        int timePrint = timesPrint % 10;
        timesPrint /= 10;
        num->changeAnimation(timePrint);
    }
}

void HUD::render() {

    background->render();
    power->render();
    tries->render();
    score->render();
    time->render();
    for (int i = 0; i < 3; ++i) {
        if (i < currentLives)
            lifeStars[i]->changeAnimation(0);
        else 
            lifeStars[i]->changeAnimation(1);
        lifeStars[i]->render();
    }
    for (int i = 0; i < 2; ++i)
    {
        triesNums[i]->render();
    }
    for (int i = 0; i < 6; ++i)
    {
        scoreNums[i]->render();
    }
    for (int i = 0; i < 3; ++i)
    {
        timeNums[i]->render();
    }
}
