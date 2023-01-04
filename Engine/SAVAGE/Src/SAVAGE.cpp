#include "Precompiled.h"
#include "SAVAGE.h"

SAVAGE::App& SAVAGE::MainApp()
{
	static App sApp;
	return sApp;
}