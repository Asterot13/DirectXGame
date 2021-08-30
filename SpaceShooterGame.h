#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputListener.h"
#include "Matrix4x4.h"

class SpaceShooterGame : public Window, public InputListener
{
public:
	SpaceShooterGame();
	~SpaceShooterGame();

	// Inherited via Window
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
	virtual void onFocus() override;
	virtual void onKillFocus() override;
	virtual void onSize() override;

	// Inherited via InputListener
	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;
	virtual void onMouseMove(const Point& mouse_pos) override;

	virtual void onLeftMouseDown(const Point& mouse_pos) override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;
	virtual void onRightMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;

public:
	void render();
	void update();
	void updateCamera();
	void updateThirdPersonCamera();
	void updateModel(Vector3D position, Vector3D rotation, Vector3D scale, const std::vector<MaterialPtr>& list_material);
	void updateSkybox();
	void updateLight();
	void updateSpaceShip();
	void drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& list_material);

private:

	SwapChainPtr m_swap_chain;

	TexturePtr m_sky_tex;
	TexturePtr m_spaceship_tex;
	TexturePtr m_asteroid_tex;

	MeshPtr m_skymesh;
	MeshPtr m_spaceship_mesh;
	MeshPtr m_asteroid_mesh;

	MaterialPtr m_base_mat;
	MaterialPtr m_skymat;
	MaterialPtr m_spaceship_mat;
	MaterialPtr m_asteroid_mat;

private:

	float m_old_delta;
	float m_new_delta;
	float m_delta_time;

	float m_cam_distance = 14.0f;
	Vector3D m_cam_rot;
	Vector3D m_cam_pos;

	float m_spaceship_speed = 125.0f;
	Vector3D m_spaceship_pos;
	Vector3D m_spaceship_rot;

	float m_delta_mouse_x = 0.0f, m_delta_mouse_y = 0.0f;

	float m_forward = 0.0f;
	float m_rightward = 0.0f;

	Matrix4x4 m_world_cam;
	Matrix4x4 m_view_cam;
	Matrix4x4 m_proj_cam;
	Matrix4x4 m_light_rot_matrix;

	float m_time = 0.0f;

	bool m_play_state = false;
	bool m_fullscreen_state = false;

	std::vector<MaterialPtr> m_list_materials;
	Vector4D m_light_position;
};
