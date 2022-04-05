# CMakeList.txt : CMake project for OpenVisualDesign, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
file( GLOB_RECURSE PPPARSE_SOURCES *.cpp )
file( GLOB_RECURSE PPPARSE_SOURCES_C *.c )
file( GLOB_RECURSE PPPARSE_HEADERS *.hpp *.h )

# Add source to this project's executable.
add_library (PPParse  ${PPPARSE_SOURCES} ${PARSEPP_SOURCES_C} ${PARSEPP_HEADERS})

target_include_directories(PPParse PUBLIC ${CMAKE_CURRENT_LIST_DIR})
# TODO: Add tests and install targets if needed.
target_link_libraries(PPParse PRIVATE )