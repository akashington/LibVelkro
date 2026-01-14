#pragma once

#include <format>
#include <iostream>
#include <string>

#ifdef VLK_CONFIG_DEBUG
#undef VLK_CONFIG_DEBUG
#define VLK_CONFIG_DEBUG true
#else
#define VLK_CONFIG_DEBUG false
#endif

namespace Velkro::Log
{
	enum Level
	{
		DEBUG, INFO, WARN, ERROR, FATAL
	};

	template<typename... Args>
	void Print(bool core, Level level, std::format_string<Args...> fmt, Args... args)
	{
		if ((level == DEBUG && VLK_CONFIG_DEBUG) || level != DEBUG)
		{
			switch (level)
			{
			case DEBUG:
				std::cout << std::format("[\x1b[36mdebug\033[0m] {}", (core) ? "CORE: " : "CLIENT: ");
				break;
			case INFO:
				std::cout << std::format("[\x1b[32minfo\033[0m] {}", (core) ? "CORE: " : "CLIENT: ");
				break;
			case WARN:
				std::cout << std::format("[\x1b[33mwarn\033[0m] {}", (core) ? "CORE: " : "CLIENT: ");
				break;
			case ERROR:
				std::cout << std::format("[\x1b[31merror\033[0m] {}", (core) ? "CORE: " : "CLIENT: ");
				break;
			case FATAL:
				std::cout << std::format("[\x1b[41mfatal\033[0m] {}", (core) ? "CORE: " : "CLIENT: ");
				break;
			}

			std::cout << std::format(fmt, std::forward<Args>(args)...) << '\n';
		}
	}
}

#define VLK_DEBUG(...)     Velkro::Log::Print(false, Velkro::Log::DEBUG, __VA_ARGS__)
#define VLK_INFO(...)      Velkro::Log::Print(false, Velkro::Log::INFO, __VA_ARGS__)
#define VLK_WARN(...)      Velkro::Log::Print(false, Velkro::Log::WARN, __VA_ARGS__)
#define VLK_ERROR(...)     Velkro::Log::Print(false, Velkro::Log::ERROR, __VA_ARGS__)
#define VLK_FATAL(...)     Velkro::Log::Print(false, Velkro::Log::FATAL, __VA_ARGS__)

#define VLK_CORE_DEBUG(...)     Velkro::Log::Print(true, Velkro::Log::DEBUG, __VA_ARGS__)
#define VLK_CORE_INFO(...)      Velkro::Log::Print(true, Velkro::Log::INFO, __VA_ARGS__)
#define VLK_CORE_WARN(...)      Velkro::Log::Print(true, Velkro::Log::WARN, __VA_ARGS__)
#define VLK_CORE_ERROR(...)     Velkro::Log::Print(true, Velkro::Log::ERROR, __VA_ARGS__)
#define VLK_CORE_FATAL(...)     Velkro::Log::Print(true, Velkro::Log::FATAL, __VA_ARGS__)