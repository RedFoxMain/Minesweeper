#include "GameLogic/game.h"
#include "Windows.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow){
	Game app;
	app.start();
	return 0;
}