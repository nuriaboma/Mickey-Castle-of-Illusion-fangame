#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include <GLFW/glfw3.h>
#include "Scene.h"
#include "Menu.h"
#include "ShaderProgram.h"
#include "EscollirNivells.h"
#include "SeleccioDificultat.h"
#include "Practica.h"
#include "Credits.h"
#include "Instructions.h"
#include "Audio.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 750
//1024
//704

// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

private:
	Game() {}
	
public:
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);

	bool getKey(int key) const;

private:
	bool bPlay; // Continue to play game?
	bool keys[GLFW_KEY_LAST+1]; // Store key states so that 
							    // we can have access at any time
	
	Scene scene;
	Menu menu;
	EscollirNivell escollirnivell;
	Practica practica;
	SeleccioDificultat selecciodificultat;
	Credits credits;
	Instructions instructions;
	bool menuActive;
	int selectedOptiondificultat;
	bool seleccioDificultatmenu;
	bool portesactive;
	bool practicaactive;
	bool sceneactive;
	bool instruactive;
	bool creditsactive;



};


#endif // _GAME_INCLUDE


