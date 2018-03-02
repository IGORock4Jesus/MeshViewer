#pragma once

#include "Event.h"
#include "Window.h"



class Renderer
{
	Microsoft::WRL::ComPtr<IDirect3D9> direct;
	Microsoft::WRL::ComPtr<IDirect3DDevice9> device;
	D3DPRESENT_PARAMETERS parameters{ 0 };
	std::thread drawThread;
	bool isRunned;


	void StartDrawing();

public:
	Event<Renderer, Microsoft::WRL::ComPtr<IDirect3DDevice9>> Drawing;

	Renderer(std::shared_ptr<Window> window);
	~Renderer();

	Microsoft::WRL::ComPtr<IDirect3DDevice9> GetDevice() { return device; }
};
