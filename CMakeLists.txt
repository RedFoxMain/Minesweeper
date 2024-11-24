cmake_minimum_required (VERSION 3.8)
set(CXX_STANDARD 17)

project ("Minesweeper")
set(PROJECT_NAME "Minesweeper")

# SFML
set(SFML_DIR "${CMAKE_SOURCE_DIR}/include/SFML-2.6.1/lib/cmake/SFML")
set(SFML_STATIC_LIBRARIES TRUE)
include_directories(${SFML_INCLUDE_DIR})
find_package(SFML 2.6.1 COMPONENTS system window graphics audio network REQUIRED)

# SFGUI
set(SFGUI_STATIC TRUE)
set(SFGUI_DIR "${PROJECT_SOURCE_DIR}/include/SFGUI-0.4.0/cmake/Modules")
set(SFGUI_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include/SFGUI-0.4.0/include")
find_package(SFGUI REQUIRED)
include_directories(${SFGUI_INCLUDE_DIR})

set(
	PROJECT_FILES
	"src/main.cpp"
	"src/game.cpp" 
	"src/game.h"
)

#add_executable("Minesweeper" WIN32 ${PROJECT_FILES})
add_executable(${PROJECT_NAME} ${PROJECT_FILES})
target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
target_link_libraries(${PROJECT_NAME} ${SFGUI_LIBRARIES})