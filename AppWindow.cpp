#include "AppWindow.h"
#include <Windows.h>
#include "Vector2D.h"
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "Mesh.h"

struct vertex
{
	Vector3D position;
	Vector2D texcoord;
};

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	Vector4D m_light_direction;
	Vector4D m_camera_position;
	Vector4D m_light_position = Vector4D(0,1,0,0);
	float m_light_radius = 4.0f;
	float m_time = 0.0f;
};

AppWindow::AppWindow()
{
}

void AppWindow::render()
{
	GraphicsEngine::get()->getRenderSystem()->getImmidiateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0, 0.3f, 0.4f, 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmidiateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	// Compute transform matrices
	update();

	//for (int i = 0; i < 3; i++)
	//{
	//	// Render model
	//	updateModel(Vector3D(0, 2.0f, -4.0f + 4.0f * i), m_mat);
	//	drawMesh(m_torus_mesh, m_mat);

	//	// Render model
	//	updateModel(Vector3D(4.0f, 2, -4.0f + 4.0f * i), m_earth_mat);
	//	drawMesh(m_skymesh, m_earth_mat);

	//	// Render model
	//	updateModel(Vector3D(-4.0f, 2.0f, -4.0f + 4.0f * i), m_bricks_mat);
	//	drawMesh(m_suzanne_mesh, m_bricks_mat);
	//}

	//updateModel(Vector3D(0, 0, 0), m_mat);
	//drawMesh(m_plane_mesh, m_mat);

	m_list_materials.clear();
	m_list_materials.push_back(m_barrelmat);
	m_list_materials.push_back(m_brickmat);
	m_list_materials.push_back(m_windowmat);
	m_list_materials.push_back(m_woodmat);

	for (int i = 0; i < 3; i++) 
	{
		for (int j = 0; j < 3; j++)
		{
			updateModel(Vector3D(-14.0f + 14.0f*i, 0, -14.0f + 14.0f * j), m_list_materials);
			drawMesh(m_housemesh, m_list_materials);
		}
	}

	m_list_materials.clear();
	m_list_materials.push_back(m_terrainmat);

	updateModel(Vector3D(0, 0, 0), m_list_materials);
	drawMesh(m_terrainmesh, m_list_materials);


	m_list_materials.clear();
	m_list_materials.push_back(m_skymat);

	// Render skybox / skysphere
	drawMesh(m_skymesh, m_list_materials);

	m_swap_chain->present(true);

	m_old_delta = m_new_delta;
	m_new_delta = (float)::GetTickCount64();

	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;
	m_time += m_delta_time;
}

void AppWindow::update()
{
	updateCamera();
	updateLight();
	updateSkybox();
}

void AppWindow::updateCamera()
{
	Matrix4x4 world_cam, temp;
	world_cam.setIdentity();

	temp.setIdentity();
	temp.setRotationX(m_rot_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	world_cam *= temp;

	Vector3D new_pos = m_world_cam.getTranslation() + world_cam.getZDirection() * (m_forward * 0.05f);
	new_pos = new_pos + world_cam.getXDirection() * (m_rightward * 0.05f);

	world_cam.setTranslation(new_pos);

	m_world_cam = world_cam;

	world_cam.inverse();

	m_view_cam = world_cam;

	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	m_proj_cam.setPerspectiveFovLH(1.57f, ((float)width / (float)height), 0.1f, 100.0f);
}

void AppWindow::updateModel(Vector3D position, const std::vector<MaterialPtr>& list_material)
{
	constant cc;

	Matrix4x4 m_light_rot_matrix;
	m_light_rot_matrix.setIdentity();
	m_light_rot_matrix.setRotationY(m_lightrot_y);


	cc.m_world.setIdentity();
	cc.m_world.setTranslation(position);
	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;
	cc.m_camera_position = m_world_cam.getTranslation();

	cc.m_light_position = m_light_position;
	cc.m_light_direction = m_light_rot_matrix.getZDirection();
	cc.m_time = m_time;
	cc.m_light_radius = m_light_radius;

	for (size_t m = 0; m < list_material.size(); m++)
	{
		list_material[m]->setData(&cc, sizeof(constant));
		/*m_mat->setData(&cc, sizeof(constant));
		m_earth_mat->setData(&cc, sizeof(constant));
		m_bricks_mat->setData(&cc, sizeof(constant));*/
	}
	//m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmidiateDeviceContext(), &cc);
}

void AppWindow::updateSkybox()
{
	constant cc;

	cc.m_world.setIdentity();
	cc.m_world.setScale(Vector3D(100.0f, 100.0f, 100.0f));
	cc.m_world.setTranslation(m_world_cam.getTranslation());
	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;

	m_skymat->setData(&cc, sizeof(constant));
	//m_sky_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmidiateDeviceContext(), &cc);
}

void AppWindow::updateLight()
{
	m_lightrot_y += 1.57f * m_delta_time;
	float dist_from_origin = 3.0f;
	//m_light_position = Vector4D(cos(m_lightrot_y) * dist_from_origin, 2.0f, sin(m_lightrot_y) * dist_from_origin, 1.0f);
	m_light_position = Vector4D(180, 140, 70, 1.0f);
}

void AppWindow::drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& list_material)
{
	// Set the vertices of triangle to draw
	GraphicsEngine::get()->getRenderSystem()->getImmidiateDeviceContext()->setVertexBuffer(mesh->getVertexBuffer());
	// Set the indices of the triangle to draw
	GraphicsEngine::get()->getRenderSystem()->getImmidiateDeviceContext()->setIndexBuffer(mesh->getIndexBuffer());

	for (size_t m = 0; m < mesh->getNumMaterialsSlots(); m++)
	{
		if (m >= list_material.size()) break;

		MaterialSlot mat = mesh->getMaterialSlot(m);

		GraphicsEngine::get()->setMaterial(list_material[m]);

		GraphicsEngine::get()->getRenderSystem()->getImmidiateDeviceContext()->drawIndexedTriangleList(mat.num_indices, 0, mat.start_index);
	}
}

AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();

	InputSystem::get()->addListener(this);
	m_play_state = true;
	InputSystem::get()->showCursor(false);

	//m_wall_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\wall.jpg");
	//m_bricks_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\brick.png");
	//m_earth_color_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\earth_color.jpg");
	//m_earth_spec_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\earth_spec.jpg");
	//m_clouds_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\clouds.jpg");
	//m_earth_night_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\earth_night.jpg");
	m_sky_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\sky.jpg");
	m_sand_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\sand.jpg");

	m_barrel_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\barrel.jpg");
	m_bricks_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\house_brick.jpg");
	m_window_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\house_windows.jpg");
	m_wood_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\house_wood.jpg");

	//m_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\scene.obj");
	//m_torus_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\torus.obj");
	//m_suzanne_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\suzanne.obj");
	//m_plane_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\plane.obj");
	m_skymesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\sphere.obj");
	m_terrainmesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\terrain.obj");
	m_housemesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\house.obj");

	RECT rc = this->getClientWindowRect();
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	m_world_cam.setTranslation(Vector3D(0, 0, -1));

	m_mat = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"PointLightPixelShader.hlsl");
	m_mat->addTexture(m_wall_tex);
	m_mat->setCullMode(CULL_MODE_BACK);

	m_terrainmat = GraphicsEngine::get()->createMaterial(m_mat);
	m_terrainmat->addTexture(m_sand_tex);
	m_terrainmat->setCullMode(CULL_MODE_BACK);

	m_barrelmat = GraphicsEngine::get()->createMaterial(m_mat);
	m_barrelmat->addTexture(m_barrel_tex);
	m_barrelmat->setCullMode(CULL_MODE_BACK);

	m_brickmat = GraphicsEngine::get()->createMaterial(m_mat);
	m_brickmat->addTexture(m_bricks_tex);
	m_brickmat->setCullMode(CULL_MODE_BACK);

	m_windowmat = GraphicsEngine::get()->createMaterial(m_mat);
	m_windowmat->addTexture(m_window_tex);
	m_windowmat->setCullMode(CULL_MODE_BACK);

	m_woodmat = GraphicsEngine::get()->createMaterial(m_mat);
	m_woodmat->addTexture(m_wood_tex);
	m_woodmat->setCullMode(CULL_MODE_BACK);

	//m_earth_mat = GraphicsEngine::get()->createMaterial(L"VertexShader.hlsl", L"PixelShader.hlsl");
	//m_earth_mat->addTexture(m_earth_color_tex);
	//m_earth_mat->addTexture(m_earth_spec_tex);
	//m_earth_mat->addTexture(m_clouds_tex);
	//m_earth_mat->addTexture(m_earth_night_tex);
	//m_earth_mat->setCullMode(CULL_MODE_BACK);

	//m_bricks_mat = GraphicsEngine::get()->createMaterial(m_mat);
	//m_bricks_mat->addTexture(m_bricks_tex);
	//m_bricks_mat->setCullMode(CULL_MODE_BACK);
	
	m_skymat = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"SkyBoxShader.hlsl");
	m_skymat->addTexture(m_sky_tex);
	m_skymat->setCullMode(CULL_MODE_FRONT);

	m_world_cam.setTranslation(Vector3D(0, 0, -2));

	m_list_materials.reserve(32);
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	InputSystem::get()->update();
	this->render();
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_swap_chain->setFullScreen(false, 1, 1);
}

void AppWindow::onFocus()
{
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

void AppWindow::onSize()
{
	RECT rc = this->getClientWindowRect();
	m_swap_chain->resize(rc.right, rc.bottom);
	this->render();
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

	if (key == 'O')
	{
		m_light_radius -= 1.0f * m_delta_time;
	}
	else if (key == 'P')
	{
		//m_rot_y -= 3.14f * m_delta_time;
		m_light_radius += 1.0f * m_delta_time;
	}
}

void AppWindow::onKeyUp(int key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;

	if (key == 'G')
	{
		m_play_state = (m_play_state) ? false : true;
		InputSystem::get()->showCursor(!m_play_state);
	}
	else if (key == 'F')
	{
		m_fullscreen_state = (m_fullscreen_state) ? false : true;
		RECT size_screen = this->getScreenSize();

		m_swap_chain->setFullScreen(m_fullscreen_state, size_screen.right, size_screen.bottom);
	}
}

void AppWindow::onMouseMove(const Point& mouse_pos)
{
	if (!m_play_state) return;

	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	m_rot_x += (mouse_pos.m_y - (height / 2.0f)) * m_delta_time * 0.1f;
	m_rot_y += (mouse_pos.m_x - (width / 2.0f)) * m_delta_time * 0.1f;

	InputSystem::get()->setCursorPosition(Point((int)width / 2.0f, (int)height / 2.0f));
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
