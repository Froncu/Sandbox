if _ACTION == "clean" then
   os.rmdir ".vs"
   os.rmdir "Output"
   os.rmdir "Intermediate"

   os.remove "*.sln"
   os.remove "*.vcxproj"
   os.remove "*.filters"
   os.remove "*.user"
end

workspace "Fronge"
	include "Fronge"

	project "FrongeEditor"
		filter "configurations:not Distribute"
			kind "ConsoleApp"

		filter "configurations:Distribute"
			kind "WindowedApp"

		filter {}

		dofile "Fronge/Premake/globalprj.lua"
		dofile "Fronge/Premake/defines.lua"

		files "Code/**"

		includedirs "Fronge/Fronge/Source"
		links "Fronge"
		links "FrongeEntry"
		postbuildcommands {
			("xcopy %[Fronge/Fronge/Output/" .. outputdir .. "/*.dll] %[%{cfg.buildtarget.directory}] /S /Y"),
			("xcopy %[Fronge/Fronge/Output/" .. outputdir .. "/*.pdb] %[%{cfg.buildtarget.directory}] /S /Y") }

	project "*"

	startproject "FrongeEditor"