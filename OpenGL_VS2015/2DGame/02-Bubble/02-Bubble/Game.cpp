#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <irrKlang.h>
#include "Game.h"

void Game::init()
{
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	//Joc complet nom�s nnecessari aquest
	menu.init();



	// scene.init();
	//escollirnivell.init();
	//practica.init();
	//selecciodificultat.init();
	//credits.init();
	//instructions.init();
	
	menuActive = true;
	seleccioDificultatmenu = false;
	portesactive = false;
	practicaactive = false;
	sceneactive = false;
	instruactive = false;
	creditsactive = false;

}

bool Game::update(int deltaTime)
{
	// scene.update(deltaTime);
	//menu.update(deltaTime);
	//escollirnivell.update(deltaTime);
	//practica.update(deltaTime);
	//selecciodificultat.update(deltaTime);
	//credits.update(deltaTime);
	//instructions.update(deltaTime);

	//JOC COMPLET:
	if (menuActive) {
		menu.update(deltaTime); 
	}
	else if (seleccioDificultatmenu) {
		selecciodificultat.update(deltaTime);
	}
	else if (portesactive) {
		escollirnivell.update(deltaTime);
	}
	else if (practicaactive) {
		practica.update(deltaTime);
	}
	else if (sceneactive) {
		scene.update(deltaTime);
	}
	else if (instruactive) {
		instructions.update(deltaTime);
	}
	else if (creditsactive) {
		credits.update(deltaTime);
	}
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// scene.render();
	//credits.render();
	//menu.render();
	//escollirnivell.render();
	//practica.render();
	//selecciodificultat.render();
	//instructions.render();

	//JOC COMPLET
	if (menuActive) {
		menu.render();
	}
	else if (seleccioDificultatmenu) {
		selecciodificultat.render();
	}
	else if (portesactive) {
		escollirnivell.render();
	}
	else if (practicaactive) {
		practica.render();
	}
	else if (sceneactive) {
		scene.render();
	}
	else if (instruactive) {
		instructions.render();
	}
	else if (creditsactive) {
		credits.render();
	}
}

void Game::keyPressed(int key)
{
	if(key == GLFW_KEY_ESCAPE) // Escape code
		bPlay = false;
	keys[key] = true;


	//JOC COMPLET
	if (menuActive) {
		if (key == GLFW_KEY_DOWN) menu.moveSelectionDown();
		else if (key == GLFW_KEY_UP) menu.moveSelectionUp();
		else if (key == GLFW_KEY_ENTER) {
			int option = menu.getSelectedOption();
			menuActive = false;

			if (option == 0) {
				seleccioDificultatmenu = true;
				selecciodificultat.init();

			}
			else if (option == 1) {
				instruactive = true;
				instructions.init();

			}
			else if (option == 2) {
				creditsactive = true;
				credits.init();

			}
		}
	}
	else if (instruactive && key == GLFW_KEY_ENTER) {
		instruactive = false;
		menuActive = true; 
	}
	else if (creditsactive && key == GLFW_KEY_ENTER) {
		creditsactive = false;
		menuActive = true;
	}
	else if (seleccioDificultatmenu) {
		if (key == GLFW_KEY_DOWN) selecciodificultat.moveSelectionDown();
		else if (key == GLFW_KEY_UP) selecciodificultat.moveSelectionUp();
		else if (key == GLFW_KEY_ENTER) {
			selectedOptiondificultat = selecciodificultat.getSelectedOption();
			std::cout << selectedOptiondificultat << std::endl;
			seleccioDificultatmenu = false;
			portesactive = true;
			escollirnivell.init();

		}
	}
	else if (portesactive) {
		int resultat = escollirnivell.entraNivell();
		if (resultat == 1) {
			std::cout << "El jugador ha entrat en una porta des de Game!" << std::endl;
			std::cout << selectedOptiondificultat << std::endl;
			portesactive = false;
			if (selectedOptiondificultat == 0) {
				practicaactive = true;
				practica.init();
			}
			else if (selectedOptiondificultat == 1) {
				sceneactive = true;
				scene.init();
			}
		}
	}
	else if (practicaactive) {
		if (practica.gematr() == true) {
			practicaactive = false;
			seleccioDificultatmenu = true;
		}
	}
	else if (sceneactive) {
		if (scene.gematr() == true) {
			sceneactive = false;
			menuActive = true;
		}
	}
}

void Game::keyReleased(int key)
{
	keys[key] = false; 
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}



