cmake_minimum_required (VERSION 3.8)
project("Minesweeper")
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# SFML
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

set(
	SOURCES_FILES
	"main.cpp"
	"GameLogic/game.cpp"
	"GameLogic/game.h"
)

add_executable (${PROJECT_NAME} WIN32 ${SOURCES_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-audio sfml-graphics)