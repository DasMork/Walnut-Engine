workspace "Walnut-Engine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Engine/Extern/GLFW/include"
IncludeDir["GLAD"] = "Engine/Extern/GLAD/include"
IncludeDir["ImGui"] = "Engine/Extern/ImGui"
IncludeDir["glm"] = "Engine/Extern/glm"

startproject "Sandbox"

group "Dependencies"
	include "Engine/Extern/GLFW"
	include "Engine/Extern/GLAD"
	include "Engine/Extern/ImGui"

group ""


project "Engine"
	location "Engine"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "wnpch.h"
	pchsource "Engine/src/wnpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/Extern/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLFW",
		"GLAD",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"WN_PLATFORM_WINDOWS",
			"WN_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		defines "WN_DEBUG"
		defines "WN_ENABLE_ASSERTS"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "WN_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "WN_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Engine/Extern/spdlog/include",
		"%{IncludeDir.glm}",
		"Engine/src",
		"Engine/Extern"
	}

	links
	{
		"Engine"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"WN_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "WN_DEBUG"
		defines "WN_ENABLE_ASSERTS"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "WN_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "WN_DIST"
		runtime "Release"
		optimize "On"