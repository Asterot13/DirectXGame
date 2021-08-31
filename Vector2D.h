#pragma once

class Vector2D
{
public:
	Vector2D() : m_x(0), m_y(0)
	{
	}

	Vector2D(float x, float y) : m_x(x), m_y(y)
	{
	}

	Vector2D(const Vector2D& vector) : m_x(vector.m_x), m_y(vector.m_y)
	{
	}

	static Vector2D lerp(const Vector2D& start, const Vector2D& end, float delta)
	{
		Vector2D v;
		v.m_x = start.m_x * (1.0f - delta) + end.m_x * (delta);
		v.m_y = start.m_y * (1.0f - delta) + end.m_y * (delta);

		return v;
	}

	Vector2D operator* (float num) const
	{
		return Vector2D(m_x * num, m_y * num);
	}

	Vector2D operator+ (const Vector2D vector) const
	{
		return Vector2D(m_x + vector.m_x, m_y + vector.m_y);
	}

	Vector2D operator- (const Vector2D vector) const
	{
		return Vector2D(m_x - vector.m_x, m_y - vector.m_y);
	}

	~Vector2D()
	{

	}

public:

	float m_x, m_y;
};