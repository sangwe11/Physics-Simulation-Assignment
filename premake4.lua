solution "PhysicsAssignment"

  configurations { "Debug", "Release"  }
  platforms { "x32", "x64" }

  -- Build type specific defines and flags
  configuration { "Debug" }
    defines { "_DEBUG" }
    flags { "Symbols" }
  configuration {}

  configuration { "Release" }
    defines { "_RELEASE", "NDEBUG" }
    flags { "Optimize" }
  configuration {}

  -- Build output directories
  configuration { "Debug", "x32" }
    targetdir "bin/Debug/x86"
  configuration {}

  configuration { "Debug", "x64" }
    targetdir "bin/Debug/x64"
  configuration {}

  configuration { "Release", "x32" }
    targetdir "bin/Release/x86"
  configuration {}

  configuration { "Release", "x64" }
    targetdir "bin/Release/x64"
  configuration {}

  -- OS specific defines
  configuration { "windows" }
    defines { "WINDOWS" }
  configuration {}

  configuration { "linux" }
    defines { "LINUX" }
  configuration {}

  -- Project
	project "PhysicsAssignment"
		language "C++"
		kind "ConsoleApp"

		-- C++ on linux
		configuration { "linux" }
			buildoptions "-std=c++11"
    configuration {}

    -- Project sources
		files { "./src/**.h", "./src/**.cpp" }
		excludes { "./include/**", "./lib/**" }

    -- Project include and lib directories
		includedirs { "./include/" }
		libdirs { "./lib/" }

    -- Virtual paths for IDE support
    vpaths {
              ["Header Files/*"] = { "**.h", "**.hpp"},
              ["Source Files/*"] = { "**.c", "**.cpp", }
           }

    -- Library linking
    configuration { "windows", "x32" }
  		links { "SDL2/x86/SDL2", "SDL2/x86/SDL2main", "SDL2/x86/SDL2_image", "assimp/x86/assimp", "opengl32", "GL/x86/glew32" }
    configuration {}

	configuration { "windows", "x32", "Debug" }
		links { "PhysX/x86/PhysX3DEBUG_x86", "PhysX/x86/PhysX3CommonDEBUG_x86", "PhysX/x86/PhysX3ExtensionsDEBUG", "PhysX/x86/PhysX3CharacterKinematicDEBUG_x86", "PhysX/x86/PhysXVisualDebuggerSDKDEBUG", "PhysX/x86/PhysX3CookingDEBUG_x86", "PhysX/x86/PhysX3GpuDEBUG_x86", "PhysX/x86/PxTaskDEBUG" }
	configuration {}

	configuration { "windows", "x32", "Release" }
		links { "PhysX/x86/PhysX3_x86", "PhysX/x86/PhysX3Common_x86", "PhysX/x86/PhysX3Extensions", "PhysX/x86/PhysX3CharacterKinematic_x86", "PhysX/x86/PhysXVisualDebuggerSDK", "PhysX/x86/PhysX3Cooking_x86", "PhysX/x86/PhysX3Gpu_x86", "PhysX/x86/PxTask" }
	configuration {}

    configuration { "windows", "x64" }
  		links { "SDL2/x64/SDL2", "SDL2/x64/SDL2main", "SDL2/x64/SDL2_image", "assimp/x64/assimp", "opengl32", "GL/x64/glew32" }
    configuration {}

	configuration { "windows", "x64", "Debug" }
		links { "PhysX/x64/PhysX3DEBUG_x64", "PhysX/x64/PhysX3CommonDEBUG_x64", "PhysX/x64/PhysX3ExtensionsDEBUG", "PhysX/x64/PhysX3CharacterKinematicDEBUG_x64", "PhysX/x64/PhysXVisualDebuggerSDKDEBUG", "PhysX/x64/PhysX3CookingDEBUG_x64", "PhysX/x64/PhysX3GpuDEBUG_x64", "PhysX/x64/PxTaskDEBUG" }
	configuration {}

	configuration { "windows", "x64", "Release" }
		links { "PhysX/x64/PhysX3_x64", "PhysX/x64/PhysX3Common_x64", "PhysX/x64/PhysX3Extensions", "PhysX/x64/PhysX3CharacterKinematic_x64", "PhysX/x64/PhysXVisualDebuggerSDK", "PhysX/x64/PhysX3Cooking_x64", "PhysX/x64/PhysX3Gpu_x64", "PhysX/x64/PxTask" }
	configuration {}

  	configuration { "linux" }
  		links { "SDL2", "SDL2main", "SDL2_image", "assimp", "GL", "GLEW" }
    configuration {}

    -- Windows DLL
    -- Post build commands
    configuration { "windows", "x32", "Debug" }
			postbuildcommands { "xcopy windows\\dll\\x86 bin\\Debug\\x86 /d /s /e /h /i /Y" }
    configuration { "windows", "x32", "Release" }
			postbuildcommands { "xcopy windows\\dll\\x86 bin\\Release\\x86 /d /s /e /h /i /Y" }
    configuration { "windows", "x64", "Debug" }
			postbuildcommands { "xcopy windows\\dll\\x64 bin\\Debug\\x64 /d /s /e /h /i /Y" }
    configuration { "windows", "x64", "Release" }
			postbuildcommands { "xcopy windows\\dll\\x64 bin\\Release\\x64 /d /s /e /h /i /Y" }
		configuration {}
