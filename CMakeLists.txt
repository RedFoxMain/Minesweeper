cmake_minimum_required (VERSION 3.8)
set(CXX_STANDARD 17)

project ("Minesweeper")

set(SFML_DIR "${CMAKE_SOURCE_DIR}/include/SFML-2.6.1/lib/cmake/SFML")
set(SFML_STATIC_LIBRARIES TRUE)
include_directories(${SFML_INCLUDE_DIR})
find_package(SFML 2.6.1 COMPONENTS system window graphics audio network REQUIRED)

set(
	PROJECT_FILES
	"src/main.cpp"
	"src/game.cpp" 
	"src/game.h"
)

add_executable("Minesweeper" ${PROJECT_FILES})
target_link_libraries("Minesweeper" sfml-system sfml-window sfml-graphics sfml-audio sfml-network)