#include "AppWindow.h"
#include <Windows.h>
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"

struct vertex
{
	Vector3D position;
	Vector3D color;
	Vector3D color1;
};

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	unsigned int m_time;
};

AppWindow::AppWindow()
{
}

void AppWindow::update()
{
	constant cc;
	cc.m_time = (int)::GetTickCount64();

	m_delta_pos += m_delta_time / 10.0f;

	if (m_delta_pos > 1.0f)
		m_delta_pos = 0;

	m_delta_scale += m_delta_time / 0.55f;

	Matrix4x4 temp;

	/*cc.m_world.setScale(Vector3D(m_scale_cube, m_scale_cube, m_scale_cube));

	temp.setIdentity();
	temp.setRotationZ(0.0f);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationX(m_rot_x);
	cc.m_world *= temp;*/

	cc.m_world.setIdentity();

	Matrix4x4 world_cam;
	world_cam.setIdentity();

	temp.setIdentity();
	temp.setRotationX(m_rot_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	world_cam *= temp;

	Vector3D new_pos = m_world_cam.getTranslation() + world_cam.getZDirection() * (m_forward*0.3f);
	new_pos = new_pos + world_cam.getXDirection() * (m_rightward * 0.3f);

	world_cam.setTranslation(new_pos);

	m_world_cam = world_cam;

	world_cam.inverse();

	cc.m_view = world_cam;
	/*cc.m_proj.setOrtohoLH(
		(this->getClientWindowRect().right - this->getClientWindowRect().left) / 300.0f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 300.0f,
		-4.0f,
		4.0f
	);*/

	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	cc.m_proj.setPerspectiveFovLH(1.57f, ((float)width / (float)height), 0.1f, 100.0f);

	m_cb->update(GraphicsEngine::get()->getImmidiateDeviceContext(), &cc);
}

AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();

	InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(false);

	GraphicsEngine::get()->Init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();
	
	RECT rc = this->getClientWindowRect();
	m_swap_chain->Init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	m_world_cam.setTranslation(Vector3D(0, 0, -2));

	vertex vertex_list[] =
	{
		// FRONT FACE
		{Vector3D(-0.5f, -0.5f, -0.5f),    Vector3D(1,0,0),   Vector3D(0.2f,0,0)},	   // POS1
		{Vector3D(-0.5f, 0.5f, -0.5f),     Vector3D(1,1,0),   Vector3D(0.2f,0.2f,1)},  // POS2
		{Vector3D(0.5f, 0.5f, -0.5f),      Vector3D(1,1,0),   Vector3D(0.2f,0.2f,0)},  // POS3
		{Vector3D(0.5f, -0.5f, -0.5f),     Vector3D(1,0,0),   Vector3D(0.2f,0,0)},	   // POS4

		// BACK FACE
		{Vector3D(0.5f, -0.5f, 0.5f),      Vector3D(0,1,0),   Vector3D(0,0.2f,0)},	   // POS5
		{Vector3D(0.5f, 0.5f, 0.5f),       Vector3D(0,1,1),   Vector3D(0,0.2f,0.2f)},  // POS6
		{Vector3D(-0.5f, 0.5f, 0.5f),      Vector3D(0,1,1),   Vector3D(1,0.2f,0.2f)},  // POS7
		{Vector3D(-0.5f, -0.5f, 0.5f),     Vector3D(0,1,0),   Vector3D(0,0.2f,0)}      // POS8
	};

	m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(vertex_list);

	unsigned int index_list[] =
	{
		//FRONT SIDE
		0, 1, 2, //FIRST TRIANGLE
		2, 3, 0, //SECOND TRIANGLE
		//BACK SIDE
		4, 5, 6,
		6, 7, 4,
		//TOP SIDE
		1, 6, 5,
		5, 2, 1,
		//BOTTOM SIDE
		7, 0, 3,
		3, 4, 7,
		//RIGHT SIDE
		3, 2, 5,
		5, 4, 3,
		//LEFT SIDE
		7, 6, 1,
		1, 0, 7
	};

	m_ib =GraphicsEngine::get()->createIndexBuffer();
	UINT size_index_list = ARRAYSIZE(index_list);

	m_ib->Load(index_list, size_index_list);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	
	m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	m_vb->Load(vertex_list, sizeof(vertex), size_list, shader_byte_code, size_shader);

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

	InputSystem::get()->update();

	GraphicsEngine::get()->getImmidiateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0, 0.3f, 0.4f, 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmidiateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	update();

	GraphicsEngine::get()->getImmidiateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getImmidiateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	GraphicsEngine::get()->getImmidiateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmidiateDeviceContext()->setPixelShader(m_ps);

	GraphicsEngine::get()->getImmidiateDeviceContext()->setVertexBuffer(m_vb);

	GraphicsEngine::get()->getImmidiateDeviceContext()->setIndexBuffer(m_ib);

	GraphicsEngine::get()->getImmidiateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);
	
	m_swap_chain->present(true);

	m_old_delta = m_new_delta;
	m_new_delta = (float)::GetTickCount64();

	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_vb->Release();
	m_ib->Release();
	m_cb->Release();
	m_swap_chain->Release();
	m_vs->Release();
	m_ps->Release();
	GraphicsEngine::get()->Release();
}

void AppWindow::onFocus()
{
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

void AppWindow::onKeyDown(int key)
{
	if (key == 'W')
	{
		//m_rot_x += 3.14f * m_delta_time;
		m_forward = 1.0f;
	}
	else if (key == 'S')
	{
		//m_rot_x -= 3.14f * m_delta_time;
		m_forward = -1.0f;
	}

	if (key == 'A')
	{
		//m_rot_y += 3.14f * m_delta_time;
		m_rightward = -1.0f;
	}
	else if (key == 'D')
	{
		//m_rot_y -= 3.14f * m_delta_time;
		m_rightward = 1.0f;
	}
}

void AppWindow::onKeyUp(int key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;
}

void AppWindow::onMouseMove(const Point& mouse_pos)
{
	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	m_rot_x += (mouse_pos.m_y - (height / 2.0f)) * m_delta_time * 0.1f;
	m_rot_y += (mouse_pos.m_x - (width / 2.0f)) * m_delta_time * 0.1f;

	InputSystem::get()->setCursorPosition(Point((float)width / 2.0f, (float)height / 2.0f));
}

void AppWindow::onLeftMouseDown(const Point& mouse_pos)
{
	m_scale_cube = 0.5f;
}

void AppWindow::onLeftMouseUp(const Point& mouse_pos)
{
	m_scale_cube = 1.0f;
}

void AppWindow::onRightMouseDown(const Point& mouse_pos)
{
	m_scale_cube = 2.0f;
}

void AppWindow::onRightMouseUp(const Point& mouse_pos)
{
	m_scale_cube = 1.0f;
}
