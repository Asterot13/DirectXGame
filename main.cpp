#include "AppWindow.h"
#include "SpaceShooterGame.h"
#include "BumpMappingDemo.h"
#include "FrameBufferDemo.h"
#include "InputSystem.h"
#include "MiniGame.h"
#include "PostProcessingDemo.h"
#include "GraphicsEngine.h"

int main()
{
	try 
	{
		GraphicsEngine::create();
		InputSystem::create();
	}
	catch (...)
	{
		return -1;
	}

	{
		try
		{
			PostProcessingDemo app;

			while (app.isRun());
		}
		catch (...)
		{
			GraphicsEngine::release();
			InputSystem::release();
			return -1;
		}
	}

	GraphicsEngine::release();
	InputSystem::release();

	return 0;
}