#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "IO.h"

namespace Velkro::Renderer
{
	void Initialize()
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			VLK_CORE_FATAL("Renderer failed to initialize.");
		}

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	uint32_t LoadShaderFromFile(const char* vertexShaderFilePath, const char* fragShaderFilePath)
	{
		std::string vertexShaderSourceStr = IO::GetFile(vertexShaderFilePath);
		std::string fragmentShaderSourceStr = IO::GetFile(fragShaderFilePath);

		const char* vertexShaderSource = vertexShaderSourceStr.c_str();
		const char* fragmentShaderSource = fragmentShaderSourceStr.c_str();

		uint32_t vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		GLint vertexShaderSuccess;
		GLchar vertexShaderInfoLog[512];

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShaderSuccess);

		if (!vertexShaderSuccess)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, vertexShaderInfoLog);

			VLK_CORE_ERROR("Vertex Shader compilation failed! \n{}\n", vertexShaderInfoLog);
		}

		uint32_t fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		GLint fragmentShaderSuccess;
		GLchar fragmentShaderInfoLog[512];

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentShaderSuccess);

		if (!fragmentShaderSuccess)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, fragmentShaderInfoLog);

			VLK_CORE_ERROR("Fragment Shader compilation failed! \n{}\n", fragmentShaderInfoLog);
		}

		uint32_t shaderProgram;
		shaderProgram = glCreateProgram();

		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		GLint shaderSuccess;
		GLchar shaderInfoLog[512];

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &shaderSuccess);

		if (!shaderSuccess)
		{
			glGetProgramInfoLog(shaderProgram, 512, NULL, shaderInfoLog);

			VLK_CORE_ERROR("Shader compilation failed! \n{}\n", shaderInfoLog);
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return shaderProgram;
	}

	uint32_t LoadTexture2D(const char* path, int& width, int& height, int& channels, bool linear)
	{
		stbi_set_flip_vertically_on_load(true);
		
		uint8_t* pixels = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);
		
		if (!pixels)
		{
			VLK_CORE_ERROR("Failed to load texture \"{}\", returning empty texture id.", path);

			return 0;
		}

		uint32_t textureID;
		
		glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, linear ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, linear ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(pixels);

		return textureID;
	}

	void ClearBuffer()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void UpdateViewport(int x, int y, int width, int height)
	{
		glViewport(x, y, width, height);
	}
}