cmake_minimum_required (VERSION 3.8)
project("Minesweeper")

set(
	SOURCES_FILES
	"main.cpp"
	"game.cpp"
	"game.h"
)
configure_file( "config.h.in" ${CMAKE_BINARY_DIR}/config.h )
include_directories( ${CMAKE_BINARY_DIR} )

# Find SFML
set( SFML_DIR "${CMAKE_SOURCE_DIR}/external/SFML/lib/cmake/SFML" )
set( SFML_STATIC_LIBRARIES TRUE )
include_directories( ${SFML_INCLUDE_DIR} )
find_package( SFML COMPONENTS audio system graphics REQUIRED )

add_executable(${PROJECT_NAME} WIN32 ${SOURCES_FILES})

# Link SFML
target_link_libraries( ${PROJECT_NAME} sfml-audio sfml-graphics sfml-system )