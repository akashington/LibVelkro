project "Glad"
    kind "StaticLib"
    language "C"
    architecture "x64"

    targetdir ("../../bin/%{prj.name}/%{cfg.architecture}-%{cfg.buildcfg}")
    objdir ("../../bin-int/%{prj.name}/%{cfg.architecture}-%{cfg.buildcfg}")

    files { "src/**.c", "include/glad/**.h", "include/KHR/**.h" }

    includedirs { "include" }