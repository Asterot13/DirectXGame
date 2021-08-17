#pragma once
#include <Windows.h>

class Window
{
public:
	Window();
	// Initialize the window
	bool Init();
	bool Broadcast();

	// Release the window
	bool Release();
	bool isRun();

	RECT getClientWindowRect();
	void setHWND(HWND hwnd);

	// EVENTS
	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();

	~Window();

protected:

	HWND m_hwnd;
	bool m_isRun;
};

