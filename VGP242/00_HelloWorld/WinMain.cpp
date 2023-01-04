#include <Core/Inc/Core.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	LOG("Hello I am %s", "Viren");
	Sleep(1000);
	LOG("%d seconds have passed and I am still fine.", 1);

	return 0;
}