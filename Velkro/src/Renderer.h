#pragma once

#include "types.h"

namespace Velkro::Renderer
{
	void Initialize();

	uint32_t LoadShaderFromFile(const char* vertexShaderFilePath, const char* fragShaderFilePath);
}