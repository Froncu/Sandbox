cmake_minimum_required(VERSION 3.15)
project(sandbox)

add_subdirectory(fronge)

add_executable(${PROJECT_NAME}
   source/sandbox.cpp)

target_include_directories(${PROJECT_NAME}
   PRIVATE source)

target_link_libraries(${PROJECT_NAME}
   PRIVATE $<$<BOOL:${MINGW}>:mingw32>)

target_link_libraries(${PROJECT_NAME}
   PRIVATE fronge)

set_target_properties(${PROJECT_NAME} PROPERTIES
   CXX_STANDARD 23
   CXX_STANDARD_REQUIRED TRUE)

target_compile_options(${PROJECT_NAME}
   PRIVATE $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic -Werror>
   PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>)

add_custom_command(TARGET ${PROJECT_NAME}
   POST_BUILD COMMAND
   ${CMAKE_COMMAND} -E copy_if_different
   $<TARGET_FILE:fronge_library>
   $<TARGET_FILE_DIR:${PROJECT_NAME}>)

add_custom_command(TARGET ${PROJECT_NAME}
   POST_BUILD COMMAND
   ${CMAKE_COMMAND} -E copy_directory_if_different
   ${CMAKE_SOURCE_DIR}/resources
   $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)