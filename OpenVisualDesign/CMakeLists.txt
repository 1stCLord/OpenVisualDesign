# CMakeList.txt : CMake project for OpenVisualDesign, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

find_package(imgui CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Clang CONFIG REQUIRED)

# Add source to this project's executable.
file( GLOB_RECURSE OVD_SOURCES *.cpp )
file( GLOB_RECURSE OVD_SOURCES_C *.c )
file( GLOB_RECURSE OVD_HEADERS *.hpp *.h )

# Add source to this project's executable.
add_executable (OpenVisualDesign  ${OVD_SOURCES} ${OVD_SOURCES_C} ${OVD_HEADERS})

target_include_directories(OpenVisualDesign PRIVATE ${CMAKE_CURRENT_LIST_DIR})
# TODO: Add tests and install targets if needed.
target_link_libraries(OpenVisualDesign PRIVATE libclang imgui::imgui SDL2::SDL2 SDL2::SDL2main ${OPENGL_LIBRARIES})