#include "stdafx.h"
#include "Window.h"

std::map<HWND, Window*> Window::links;


LRESULT CALLBACK Window::Procedure(HWND h, UINT m, WPARAM w, LPARAM l)
{
	LRESULT result = LRESULT{ 0 };
	auto win = links.find(h);
	if (win != links.end()) {
		result = win->second->Procedure(m, w, l);
	}

	if (!result) {
		switch (m)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			result = DefWindowProc(h, m, w, l);
		}
	}

	return result;
}

LRESULT Window::Procedure(UINT m, WPARAM w, LPARAM l) {
	switch (m)
	{
	case WM_LBUTTONDOWN:
		MouseDown(MouseButton::Left, GET_X_LPARAM(l), GET_Y_LPARAM(l));
		break;
	case WM_RBUTTONDOWN:
		MouseDown(MouseButton::Right, GET_X_LPARAM(l), GET_Y_LPARAM(l));
		break;
	case WM_MBUTTONDOWN:
		MouseDown(MouseButton::Middle, GET_X_LPARAM(l), GET_Y_LPARAM(l));
		break;
	}
	return (LRESULT)0;
}

Window::Window(HINSTANCE hinstance, const std::wstring& title) {
	WNDCLASS wc{ 0 };
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hInstance = hinstance;
	wc.lpfnWndProc = Procedure;
	wc.lpszClassName = className.c_str();
	wc.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	if (!RegisterClass(&wc))
		throw std::exception("::RegisterClass()");

	handle = CreateWindow(className.c_str(), title.c_str(), WS_POPUPWINDOW, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), HWND_DESKTOP, nullptr, hinstance, this);
	if (!handle) {
		throw std::exception("::RegisterClass()");
	}

	ShowWindow(handle, SW_NORMAL);
	UpdateWindow(handle);
}
Window::~Window() {
	if (handle) {
		CloseWindow(handle);
	}
}
HWND Window::GetHandle()
{
	return handle;
}

void Window::Close()
{
	::SendMessage(handle, WM_CLOSE, 0, 0);
}
RECT Window::GetClientRect()
{
	RECT rect;
	::GetClientRect(handle, &rect);
	return rect;
}

void Window::Run()
{
	MSG msg{ 0 };
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}