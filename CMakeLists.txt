cmake_minimum_required(VERSION 3.15)
project(Sandbox)

add_subdirectory(External/Fronge)

if (NOT MSVC)
   message(FATAL_ERROR "This project currently can be build only using Microsoft Visual C++ (MSVC).")
endif ()

add_executable(Sandbox
   Source/Sandbox.cpp)

target_include_directories(Sandbox PRIVATE
   Source)

target_link_libraries(Sandbox PRIVATE
   Fronge
   FrongeEntry)

set_target_properties(Sandbox PROPERTIES
   CXX_STANDARD 20
   CXX_STANDARD_REQUIRED TRUE)

target_compile_options(Sandbox PRIVATE
   /W4
   /WX)

add_custom_command(TARGET Sandbox
   POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_if_different
   $<TARGET_FILE_DIR:Fronge>/Fronge.dll
   $<TARGET_FILE_DIR:Sandbox>)

add_custom_command(TARGET Sandbox
   POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
   ${CMAKE_SOURCE_DIR}/Data
   $<TARGET_FILE_DIR:Sandbox>/Data)