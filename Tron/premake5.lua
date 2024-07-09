project "Tron"
	filter "configurations:not Release"
		kind "ConsoleApp"

	filter "configurations:Release"
		kind "WindowedApp"

	filter {}

	dofile "../Fronge/Premake/globalprj.lua"
	dofile "../Fronge/Premake/defines.lua"

	files "Code/**"
	includedirs "../Fronge/Fronge/Source"
	targetdir ("../Output/" .. outputdir)
	objdir ("../Intermediate/" .. outputdir)

	links {
		"Fronge",
		"FrongeEntry" }

	postbuildcommands {
		("xcopy %[../Fronge/Output/" .. outputdir .. "/*.dll] %[%{cfg.buildtarget.directory}] /S /Y"),
		("xcopy %[../Fronge/Output/" .. outputdir .. "/*.pdb] %[%{cfg.buildtarget.directory}] /S /Y") }

	filter {}

	dofile "../External/vld/premake5.lua"

project "*"