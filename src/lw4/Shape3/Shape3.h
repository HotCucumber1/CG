#pragma once
#include "../../Common/BaseWindow/BaseWindow.h"
#include "../../Common/Component/Color.h"
#include "MobiusStrip.h"

class Shape3 final : public BaseWindow
{
public:
	Shape3(
		int width,
		int height,
		const std::string& title);

private:
	void Draw(int width, int height) override;

	void OnCursorPos(double x, double y) override;

	void OnMouseButton(int button, int action, int mods) override;

	void DrawQuads(float alpha = 1) const;

	void DrawEdges(Color color) const;

private:
	double m_rotationX = 0;
	double m_rotationY = 0;
	double m_lastMouseX = 0;
	double m_lastMouseY = 0;
	bool m_isMousePressed = false;
	MobiusStrip m_facesData;
};
