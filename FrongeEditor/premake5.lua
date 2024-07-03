project "FrongeEditor"
	filter "configurations:not Distribute"
		kind "ConsoleApp"

	filter "configurations:Distribute"
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

project "*"