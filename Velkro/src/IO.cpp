#include "IO.h"

namespace Velkro::IO
{
	std::string GetFile(std::string filePath)
	{
		std::ifstream file(filePath);

		if (!file.is_open())
		{
			VLK_CORE_ERROR("IO: Failed to open file {0}", filePath);
		}

		std::string fileStr;

		std::string line;
		while (std::getline(file, line))
		{
			fileStr += line + '\n';
		}

		// Close the file
		file.close();

		return fileStr;
	}
}