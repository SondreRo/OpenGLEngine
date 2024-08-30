// OpenGLEngine.cpp : Defines the entry point for the application.
//

#include "OpenGLEngine.h"
#include "core/Application.h"

using namespace std;

int main()
{
	Application::get().init();
	Application::get().Run();
	return 0;
}
