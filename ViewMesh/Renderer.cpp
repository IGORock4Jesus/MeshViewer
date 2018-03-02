#include "stdafx.h"
#include "Renderer.h"
#include "Window.h"


Renderer::Renderer(std::shared_ptr<Window> window)
{
	if (!(direct = Direct3DCreate9(D3D_SDK_VERSION)))
		throw std::exception("Direct3DCreate9()");
	parameters.AutoDepthStencilFormat = D3DFMT_D24S8;
	RECT cr;
	HWND hwnd = window->GetHandle();
	::GetClientRect(hwnd, &cr);
	parameters.BackBufferWidth = cr.right - cr.left;
	parameters.BackBufferHeight = cr.bottom - cr.top;
	parameters.BackBufferFormat = D3DFMT_A8R8G8B8;
	parameters.EnableAutoDepthStencil = true;
	parameters.hDeviceWindow = hwnd;
	parameters.Windowed = true;
	parameters.SwapEffect = D3DSWAPEFFECT::D3DSWAPEFFECT_DISCARD;
	if (FAILED(direct->CreateDevice(0, D3DDEVTYPE::D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &parameters, &device)))
	{
		throw std::exception("Direct3DCreate9()");
	}

	device->SetRenderState(D3DRS_LIGHTING, FALSE);

	isRunned = true;
	drawThread = std::thread{ &Renderer::StartDrawing, this };
}

Renderer::~Renderer() {
	isRunned = false;
	if (drawThread.joinable())
		drawThread.join();
}

void Renderer::StartDrawing()
{
	while (isRunned) {
		device->Clear(0, nullptr, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffbbbbbb, 1.0f, 0);
		device->BeginScene();

		Drawing(device);

		device->EndScene();
		device->Present(nullptr, nullptr, nullptr, nullptr);

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}
