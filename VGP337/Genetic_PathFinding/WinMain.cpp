#include "GameState.h"
#include <SAVAGE/Inc/SAVAGE.h>

int WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	SAVAGE::MainApp().AddState<GameState>("GameState");
	SAVAGE::MainApp().Run({ L"GeneticPathFinding", 1920, 1080 });
	return 0;
}