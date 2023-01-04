#include "GameState.h"
#include <SAVAGE/Inc/SAVAGE.h>

int WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	SAVAGE::MainApp().AddState<GameState>("GameState");
	SAVAGE::MainApp().Run({L"Hello Cube"});
	return 0;
}