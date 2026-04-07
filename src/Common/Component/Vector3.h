#pragma once

template <class T>
class Vector3
{
public:
	Vector3(T x0, T y0, T z0)
		: m_x(x0)
		, m_y(y0)
		, m_z(z0)
	{
	}

	Vector3()
		: Vector3(0, 0, 0)
	{
	}

	T GetLength() const
	{
		return static_cast<T>(sqrt(m_x * m_x + m_y * m_y + m_z * m_z));
	}

	T GetX() const
	{
		return m_x;
	}

	T GetY() const
	{
		return m_y;
	}

	T GetZ() const
	{
		return m_z;
	}

	void Normalize()
	{
		double invLength = 1 / GetLength();
		m_x *= invLength;
		m_y *= invLength;
		m_z *= invLength;
	}

private:
	T m_x;
	T m_y;
	T m_z;
};

/*
Векторное произведение векторов A и B равно определителю матрицы:
	  |i  j  k |
AxB = |Ax Ay Az|
	  |Bx By Bz|
*/
template <class T>
Vector3<T> Cross(const Vector3<T>& a, const Vector3<T>& b)
{
	return Vector3<T>(
		a.GetY() * b.GetZ() - a.GetZ() * b.GetY(),
		a.GetZ() * b.GetX() - a.GetX() * b.GetZ(),
		a.GetX() * b.GetY() - a.GetY() * b.GetX());
}

using Vector3d = Vector3<double>;
using Vector3i = Vector3<int>;
