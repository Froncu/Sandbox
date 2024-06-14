project "FrongeEditor"
	filter "configurations:not Distribute"
		kind "ConsoleApp"

	filter "configurations:Distribute"
		kind "WindowedApp"

	filter {}

	dofile "../Fronge/Premake/globalprj.lua"
	dofile "../Fronge/Premake/defines.lua"

	files "Code/**"
	targetdir ("../Output/" .. outputdir)
	objdir ("../Intermediate/" .. outputdir)

	includedirs "../Fronge/Fronge/Source"
	links "Fronge"
	links "FrongeEntry"
	postbuildcommands {
		("xcopy %[../Fronge/Output/" .. outputdir .. "/*.dll] %[%{cfg.buildtarget.directory}] /S /Y"),
		("xcopy %[../Fronge/Output/" .. outputdir .. "/*.pdb] %[%{cfg.buildtarget.directory}] /S /Y") }

project "*"