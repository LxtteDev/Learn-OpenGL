workspace "OpenGL"
    configurations { "Debug", "Release" }

project "OpenGL"
    kind "ConsoleApp"
    language "C++"

    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin-int/%{cfg.buildcfg}"

    defines {
        -- "_GLFW_WAYLAND"
        "_GLFW_X11"
    }

    includedirs {
        "src",
        "vendor/glad/include",
        "vendor/glfw/include"
    }
    
    files {
        "src/**.h",
        "src/**.cpp",

        "vendor/glad/include/**.h",
        "vendor/glad/src/**.c",
        
        "vendor/glfw/include/**.h",
        "vendor/glfw/src/**.h",
        "vendor/glfw/src/**.c"
    }  