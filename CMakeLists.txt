cmake_minimum_required (VERSION 3.8)
project("Minesweeper")

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(
	SOURCES_FILES
	"src/main.cpp"
	"src/game.cpp"
	"src/game.h"
)

# Find SFML
set( SFML_DIR "${CMAKE_SOURCE_DIR}/external/SFML/lib/cmake/SFML" )
set( SFML_STATIC_LIBRARIES TRUE )
include_directories( ${SFML_INCLUDE_DIR} )
find_package( SFML COMPONENTS audio system graphics REQUIRED )

add_executable(${PROJECT_NAME} WIN32 ${SOURCES_FILES})

# Link SFML
target_link_libraries( ${PROJECT_NAME} sfml-audio sfml-graphics sfml-system )