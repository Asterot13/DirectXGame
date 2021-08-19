#include "GraphicsEngine.h"


GraphicsEngine::GraphicsEngine()
{
}

bool GraphicsEngine::Init()
{
	m_render_system = new RenderSystem();
	m_render_system->Init();
	return true;
}

bool GraphicsEngine::Release()
{
	m_render_system->Release();
	delete m_render_system;
	return true;
}

RenderSystem* GraphicsEngine::getRenderSystem()
{
	return m_render_system;
}

GraphicsEngine::~GraphicsEngine()
{
}

GraphicsEngine* GraphicsEngine::get()
{
	static GraphicsEngine engine;
	return &engine;
}
