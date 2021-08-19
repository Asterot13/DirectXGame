#pragma once
#include <d3d11.h>
#include "Prerequisites.h"
#include "RenderSystem.h"

class GraphicsEngine
{
public:

	GraphicsEngine();
	// Initialize the graphics engine and DirectX11 Device
	bool Init();

	// Release all the resources loaded
	bool Release();
	RenderSystem* getRenderSystem();
	~GraphicsEngine();

public:

	static GraphicsEngine* get();

private:

	RenderSystem* m_render_system = nullptr;

};
