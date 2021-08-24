#pragma once
#include <Windows.h>

class Window
{
public:
	// Initialize the window
	Window();

	bool isRun();

	RECT getClientWindowRect();
	RECT getScreenSize();

	// EVENTS
	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();
	virtual void onSize();

	// Release the window
	~Window();

private:
	bool Broadcast();

protected:

	HWND m_hwnd;
	bool m_isRun;
	bool m_isInit = false;
};

