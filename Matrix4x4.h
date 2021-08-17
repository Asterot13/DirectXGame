#pragma once
#include <memory>

class Matrix4x4
{
public:
	Matrix4x4()
	{
	}

	void setIdentity()
	{
		::memset(m_mat, 0, sizeof(float) * 16);
	}

	~Matrix4x4()
	{

	}

public:

	float m_mat[4][4] = {};
};