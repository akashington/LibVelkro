include "vendor/glfw"
include "vendor/glad"

project "LibVelkro"
	kind "StaticLib"

	language "C++"
	cppdialect "C++latest"
	
	architecture "x64"

	targetdir "bin/%{prj.name}/%{cfg.architecture}-%{cfg.buildcfg}"
	objdir "bin-int/%{prj.name}/%{cfg.architecture}-%{cfg.buildcfg}"	

	links { "GLFW", "Glad" }

	files { "include/**.h", "src/**.h", "src/**.cpp" }

	includedirs { "include", "vendor/glfw/include", "vendor/glad/include", "vendor/uuid_v4", "vendor/glm", "vendor/stb" }

	configurations { "Debug", "Release" }

	filter "configurations:Debug"
		defines { "VLK_CONFIG_DEBUG" }
		symbols "On"

	filter "configurations:Release"
		optimize "On"