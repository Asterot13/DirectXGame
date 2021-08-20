#pragma once
#include "Vector3D.h"
#include "Vector2D.h"

class VertexrMesh
{
public:
	VertexrMesh() : m_position(), m_texcoord(), m_normal()
	{
	}

	VertexrMesh(Vector3D position, Vector2D texcoord, Vector3D normal) : m_position(position), m_texcoord(texcoord), m_normal(normal)
	{
	}

	VertexrMesh(const VertexrMesh& vector) : m_position(vector.m_position), m_texcoord(vector.m_texcoord), m_normal(vector.m_normal)
	{
	}

	~VertexrMesh()
	{

	}

public:
	Vector3D m_position;
	Vector2D m_texcoord;
	Vector3D m_normal;
};