cmake_minimum_required (VERSION 3.8)
project ("Minesweeper")

set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/thirdparty/SFML-3.0.0")
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 3.0.0 COMPONENTS Audio System Graphics REQUIRED)

add_executable(${PROJECT_NAME} WIN32 "src/main.cpp")
# Link SFML
target_include_directories(${PROJECT_NAME} PUBLIC ${SFML_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} SFML::Audio SFML::Graphics SFML::System)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)