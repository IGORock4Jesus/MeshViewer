#pragma once

#include "Event.h"


enum class MouseButton
{
	Left, Right, Middle, X1, X2
};

class Window {

	HWND handle;
	const std::wstring className = L"ME_CLASS";
	static std::map<HWND, Window*> links;

	static LRESULT CALLBACK Procedure(HWND h, UINT m, WPARAM w, LPARAM l);
	LRESULT Procedure(UINT m, WPARAM w, LPARAM l);

public:
	Event<Window, MouseButton, int, int> MouseDown;

	Window(HINSTANCE hinstance, const std::wstring& title);
	~Window();
	
	HWND GetHandle();
	void Close();
	RECT GetClientRect();
	void Run();
};
