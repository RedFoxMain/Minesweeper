cmake_minimum_required (VERSION 3.8)
project("Minesweeper")

set(
	SOURCES_FILES
	"main.cpp"
	"game.cpp"
	"game.h"
)

# Find SFML
set( SFML_DIR "${CMAKE_SOURCE_DIR}/external/SFML/lib/cmake/SFML" )
set( SFML_STATIC_LIBRARIES TRUE )
include_directories( ${SFML_INCLUDE_DIR} )
find_package( SFML COMPONENTS audio system graphics REQUIRED )

add_executable(${PROJECT_NAME} WIN32 ${SOURCES_FILES})

add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${CMAKE_SOURCE_DIR}/external/SFML/bin/openal32.dll"
	$<TARGET_FILE_DIR:${PROJECT_NAME}>
)

# Link SFML
target_link_libraries( ${PROJECT_NAME} sfml-audio sfml-graphics sfml-system )