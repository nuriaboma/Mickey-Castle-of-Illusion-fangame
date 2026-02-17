#ifndef _HUD_INCLUDE
#define _HUD_INCLUDE

#include <chrono>
#include <thread>
#include "Sprite.h"
#include "TileMap.h"

class HUD {
public:
    void init(ShaderProgram& shaderProgram);
    void update(int currLives, int currTries, int currScore, glm::vec2 cameraPosition, float currentTime);
    void render();

private:
    int currentLives;           
    int currentTries;            
    int currentScore;           
    std::vector<Sprite*> lifeStars;
    Texture starTexture;
    Texture backgroundTexture;
    Sprite* background;
    Texture powerTexture;
    Sprite* power;
    Texture triesTexture;
    Sprite* tries;
    Texture scoreTexture;
    Sprite* score;
    Texture timeTexture;
    Sprite* time;
    std::vector<Sprite*> triesNums;
    std::vector<Sprite*> scoreNums;
    std::vector<Sprite*> timeNums;
    Texture numsTexture;
};



#endif
