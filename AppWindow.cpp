#include "AppWindow.h"
#include <Windows.h>
#include "Vector3D.h"

struct vertex
{
	Vector3D position;
	Vector3D position1;
	Vector3D color;
	Vector3D color1;
};

__declspec(align(16))
struct constant
{
	unsigned int m_time;
};

AppWindow::AppWindow()
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();
	GraphicsEngine::get()->Init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();
	
	RECT rc = this->getClientWindowRect();
	m_swap_chain->Init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	vertex list[] =
	{
		{Vector3D(-0.5f, -0.5f, 0.0f),   Vector3D(-0.32f, -0.11f, 0.0f),   Vector3D(0,0,0),   Vector3D(0,1,0)},  // POS1
		{Vector3D(-0.5f, 0.5f, 0.0f),    Vector3D(0.11f, 0.78f, 0.0f),     Vector3D(1,1,0),   Vector3D(0,1,1)},  // POS2
		{Vector3D(0.5f, -0.5f, 0.0f),    Vector3D(0.75f, -0.73f, 0.0f),    Vector3D(0,0,1),   Vector3D(1,0,0)},  // POS3
		{Vector3D(0.5f, 0.5f, 0.0f),     Vector3D(0.88f, 0.77f, 0.0f),     Vector3D(1,1,1),   Vector3D(0,0,1)}
	};

	m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(list);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	
	m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	m_vb->Load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);

	GraphicsEngine::get()->releaseCompiledShader();

	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	constant cc;
	cc.m_time = 0;

	m_cb = GraphicsEngine::get()->createConstantBuffer();
	m_cb->Load(&cc, sizeof(constant));
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	GraphicsEngine::get()->getImmidiateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0, 0.3f, 0.4f, 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmidiateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	constant cc;
	cc.m_time = ::GetTickCount64();

	m_cb->update(GraphicsEngine::get()->getImmidiateDeviceContext(), &cc);

	GraphicsEngine::get()->getImmidiateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getImmidiateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	GraphicsEngine::get()->getImmidiateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmidiateDeviceContext()->setPixelShader(m_ps);

	GraphicsEngine::get()->getImmidiateDeviceContext()->setVertexBuffer(m_vb);

	GraphicsEngine::get()->getImmidiateDeviceContext()->drawTriangleStrip(m_vb->getSizeVertexList(), 0);
	
	m_swap_chain->present(true);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_vb->Release();
	m_swap_chain->Release();
	m_vs->Release();
	m_ps->Release();
	GraphicsEngine::get()->Release();
}
