#include "stdafx.h"
#include "Window.h"
#include "Renderer.h"



int WINAPI wWinMain(HINSTANCE hinstance, HINSTANCE, LPWSTR, int) {
	std::shared_ptr<Window> window{ new Window{ hinstance, L"NoEngine - Mesh Viewer" } };
	std::shared_ptr<Renderer> renderer{ new Renderer{ window } };

	window->Run();


	return 0;
}