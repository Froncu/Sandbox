local relativepath = os.getcwd()

filter { "configurations:Debug", "platforms:x64" }
	libdirs "lib/Win64"
	postbuildcommands ("xcopy %[" .. relativepath .. "/bin/Win64] %[%{cfg.buildtarget.directory}] /S /Y")

filter { "configurations:Debug", "platforms:x86" }
	libdirs "lib/Win32"
	postbuildcommands ("xcopy %[" .. relativepath .. "/bin/Win32] %[%{cfg.buildtarget.directory}] /S /Y")

filter "configurations:Debug"
	links "vld"
	includedirs "include"

filter {}