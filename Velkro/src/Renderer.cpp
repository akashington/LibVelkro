#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "IO.h"

namespace Velkro::Renderer
{
	void Initialize()
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			VLK_CORE_FATAL("Renderer failed to initialize.");
		}

		VLK_CORE_INFO("Renderer initialized!");
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
}