cmake_minimum_required(VERSION 3.15)
project(sandbox)

add_subdirectory(fronge)

if (NOT MSVC)
   message(FATAL_ERROR "This project currently can be build only using Microsoft Visual C++ (MSVC).")
endif ()

add_executable(${PROJECT_NAME}
   source/sandbox.cpp)

target_include_directories(${PROJECT_NAME}
   PRIVATE source)

target_link_libraries(${PROJECT_NAME}
   PRIVATE fronge
   PRIVATE fronge_entry)

set_target_properties(${PROJECT_NAME} PROPERTIES
   CXX_STANDARD 20
   CXX_STANDARD_REQUIRED TRUE)

target_compile_options(${PROJECT_NAME}
   PRIVATE /W4
   PRIVATE /WX)

add_custom_command(TARGET ${PROJECT_NAME}
   POST_BUILD COMMAND
   ${CMAKE_COMMAND} -E copy_if_different
   $<TARGET_FILE_DIR:fronge>/fronge.dll
   $<TARGET_FILE_DIR:${PROJECT_NAME}>)

add_custom_command(TARGET ${PROJECT_NAME}
   POST_BUILD COMMAND
   ${CMAKE_COMMAND} -E copy_directory_if_different
   ${CMAKE_SOURCE_DIR}/resources
   $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)