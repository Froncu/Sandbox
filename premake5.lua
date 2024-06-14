if _ACTION == "clean" then
   os.rmdir ".vs"
   os.rmdir "Output"
   os.rmdir "Intermediate"

   os.remove "**.sln"
   os.remove "**.vcxproj"
   os.remove "**.filters"
   os.remove "**.user"
end

workspace "Fronge"
	include "Fronge"
	include "FrongeEditor"

	startproject "FrongeEditor"