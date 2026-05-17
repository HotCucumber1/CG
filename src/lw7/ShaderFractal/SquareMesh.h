#pragma once
#include "../../Common/Shader/AbstractMesh.h"

class SquareMesh final : AbstractMesh
{
public:
	void Init() override;

	void Draw() const override;
};
