-- Premake Project Structure
workspace "Velkro"
	configurations { "Development", "Distribution" }
	language "C++"
	cppdialect "C++latest"
	startproject "Project"

include "vendor/glfw"
include "vendor/glad"

project "Velkro"
	kind "StaticLib"

	location "%{prj.name}"

	targetdir "bin/%{prj.name}/%{cfg.architecture}-%{cfg.buildcfg}"
	objdir "bin-int/%{prj.name}/%{cfg.architecture}-%{cfg.buildcfg}"

	architecture "x64"    

	links { "GLFW", "Glad" }

	files { "%{prj.name}/include/**.h", "%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp" }

	includedirs { "%{prj.name}/include", "vendor/glfw/include", "vendor/glad/include", "vendor/uuid_v4", "vendor/glm" }

	filter "configurations:Development"
		defines { "VLK_DEVELOPMENT" }
		symbols "On"

	filter "configurations:Distribution"
		optimize "On"

project "Project"
	kind "ConsoleApp"

	location "%{prj.name}"

	targetdir "bin/%{prj.name}/%{cfg.architecture}-%{cfg.buildcfg}"
	objdir "bin-int/%{prj.name}/%{cfg.architecture}-%{cfg.buildcfg}"

	architecture "x64"

	links { "Velkro" }

	files { "%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp" }
	
	includedirs { "Velkro/include", "vendor/glfw/include", "vendor/glad/include", "vendor/uuid_v4", "vendor/glm" }

	filter "configurations:Development"
		symbols "On"

	filter "configurations:Distribution"
		optimize "On"