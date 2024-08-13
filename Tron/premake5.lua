project "Tron"
	filter "configurations:not Release"
		kind "ConsoleApp"

	filter "configurations:Release"
		kind "WindowedApp"

	filter {}

	dofile "../External/Fronge/Premake/globalprj.lua"
	dofile "../External/Fronge/Premake/defines.lua"

	files "Source/**"
	includedirs { "Source", "../External/Fronge/Fronge/Source" }
	targetdir ("../Output/" .. outputdir)
	objdir ("../Intermediate/" .. outputdir)

	links {
		"Fronge",
		"FrongeEntry" }

	postbuildcommands {
		("xcopy %[../External/Fronge/Output/" .. outputdir .. "/*.dll] %[%{cfg.buildtarget.directory}] /S /Y"),
		("xcopy %[../External/Fronge/Output/" .. outputdir .. "/*.pdb] %[%{cfg.buildtarget.directory}] /S /Y"),
		"mkdir %[%{cfg.buildtarget.directory}Data]",
		"xcopy %[Data/.] %[%{cfg.buildtarget.directory}Data] /S /Y" }

	dofile "../External/vld/premake5.lua"

project "*"