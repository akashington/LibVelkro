#pragma once

#include "Types.h"

namespace Velkro::Renderer
{
	void Initialize();

	uint32_t LoadShaderFromFile(const char* vertexShaderFilePath, const char* fragShaderFilePath);

	uint32_t LoadTexture2D(const char* path, int& width, int& height, int& channels, bool linear);

	void ClearBuffer();

	void UpdateViewport(int x, int y, int width, int height);
}