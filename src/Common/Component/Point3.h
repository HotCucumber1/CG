#pragma once

template <class T>
class Point3
{
public:
	Point3(T x0, T y0, T z0)
		: m_x(x0)
		, m_y(y0)
		, m_z(z0)
	{
	}

	Point3()
		: Point3(0, 0, 0)
	{
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

private:
	T m_x;
	T m_y;
	T m_z;
};

using Point3d = Point3<double>;
using Point3i = Point3<int>;
