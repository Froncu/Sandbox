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
		kind "ConsoleApp"

		dofile "Fronge/Premake/globalprj.lua"
		dofile "Fronge/Premake/defines.lua"

		files "Code/**"

		includedirs "Fronge/Source"
		links "Fronge"
		postbuildcommands {
			("{COPYFILE} %[Fronge/Output/" .. outputdir .. "/*.dll] %[%{cfg.buildtarget.directory}]"),
			("{COPYFILE} %[Fronge/Output/" .. outputdir .. "/*.pdb] %[%{cfg.buildtarget.directory}]") }

	project "*"

	startproject "FrongeEditor"