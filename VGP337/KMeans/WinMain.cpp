#include "GameState.h"
#include <SAVAGE/Inc/SAVAGE.h>

int WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	SAVAGE::MainApp().AddState<GameState>("GameState");
	SAVAGE::MainApp().Run({ L"KMeans Clustering", 1280, 1024 });
	return 0;
}