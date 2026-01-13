#pragma once

#include "types.h"

namespace Velkro::Renderer
{
	void Initialize();

	uint32_t LoadShaderFromFile(const char* vertexShaderFilePath, const char* fragShaderFilePath);

	uint32_t LoadTexture2D(const char* path, int& width, int& height, int& channels, bool linear);
}