#pragma once
#include "../../Common/BaseWindow/BaseWindow.h"
#include "../../Common/Component/Color.h"
#include "../../Common/Component/Vector3.h"

#include <array>
#include <vector>

class Shape final : public BaseWindow
{
public:
	static constexpr int TRIANGLE_VERTICES = 3;
	static constexpr int QUAD_VERTICES = 4;

	struct FacesData
	{
		std::vector<Vector3d> vertices;
		std::vector<std::array<int, TRIANGLE_VERTICES>> triangleFaces;
		std::vector<std::array<int, QUAD_VERTICES>> quadFaces;
	};

	Shape(
		int width,
		int height,
		const std::string& title);

private:
	void Draw(int width, int height) override;

	void DrawTriangles(Color color, float alpha = 1) const;

	void DrawQuads(Color color, float alpha = 1) const;

	void DrawEdges(Color color, float alpha = 1) const;

	void DrawFaces(GLenum mode) const;

	void OnCursorPos(double x, double y) override;

	void OnMouseButton(int button, int action, int mods) override;

private:
	double m_rotationX = 0;
	double m_rotationY = 0;
	double m_lastMouseX = 0;
	double m_lastMouseY = 0;
	bool m_isMousePressed = false;
	FacesData m_facesData;
};
