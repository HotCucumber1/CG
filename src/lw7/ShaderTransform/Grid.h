#pragma once
#include "../../Common/Shader/AbstractMesh.h"

class Grid final : AbstractMesh
{
public:
	void ParameterInit(int stepsU, int stepsV);

	void Init() override;

	void Draw() const override;
};
