workspace "OpenGL"
    configurations { "Debug", "Release" }

project "OpenGL"
    kind "ConsoleApp"
    language "C++"

    files { "src/**.h", "src/**.cpp" }
    includedirs { "src" }

    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin-int/%{cfg.buildcfg}"