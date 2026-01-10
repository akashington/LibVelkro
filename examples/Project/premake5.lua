workspace "Example"	
	language "C++"
	cppdialect "C++latest"

	configurations { "Debug", "Release" }

	startproject "Example"

include "../../"

project "Example"
	kind "ConsoleApp"

	targetdir "bin/%{cfg.architecture}-%{cfg.buildcfg}"
	objdir "bin-int/%{cfg.architecture}-%{cfg.buildcfg}"

	architecture "x64"

	links { "LibVelkro" }

	files { "src/**.h", "src/**.cpp" }
	
	includedirs
	{
		"../../../LibVelkro/include"
	}

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"