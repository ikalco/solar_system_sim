cmake_minimum_required(VERSION 3.27)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

add_compile_options(
	-Wall
	-Wextra
)

project(solar_system_sim)

file(GLOB_RECURSE SRCFILES src/*.c)
add_executable(solar_system_sim ${SRCFILES})

target_link_libraries(solar_system_sim PRIVATE m)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

target_include_directories(solar_system_sim PUBLIC "src/")
target_include_directories(solar_system_sim
    PUBLIC ${SDL2_INCLUDE_DIRS}
	PUBLIC ${SDL2TTF_INCLUDE_DIRS}
)

target_link_libraries(solar_system_sim PRIVATE SDL2 SDL2_ttf)
