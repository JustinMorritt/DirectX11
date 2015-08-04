//ENTRY POINT 
#include "Engine.h"




//Win 32 entry point ...   "handle to the application"			"optional old deprecated"    "command line arguments" "show code optional"                          
int WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd)
{
	//CREATE THE TEST APP
	Engine App(hInstance);
	if (!App.Init())return 1;

	//MessageBox(NULL, "Hello, World", "Test", NULL);
	return App.Run();
}