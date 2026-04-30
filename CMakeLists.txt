cmake_minimum_required(VERSION 3.16)

project(DragonSlayer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
list(FILTER SOURCES EXCLUDE REGEX "/build/")
list(FILTER SOURCES EXCLUDE REGEX "/Main Game/Main\\.cpp$")
list(FILTER SOURCES EXCLUDE REGEX "/GUI/.*\\.cpp$")
list(FILTER SOURCES EXCLUDE REGEX "/Main Game/GUI/GuiMain\\.cpp$")

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
list(FILTER HEADERS EXCLUDE REGEX "/build/")

set(INCLUDE_DIRS "")
foreach(header ${HEADERS})
    get_filename_component(header_dir "${header}" DIRECTORY)
    list(APPEND INCLUDE_DIRS "${header_dir}")
endforeach ()
list(REMOVE_DUPLICATES INCLUDE_DIRS)

add_executable(DragonSlayer ${SOURCES} "Main Game/Main.cpp")
add_executable(DragonSlayerGui WIN32 ${SOURCES} "GUI/GuiGame.cpp" "GUI/GuiMain.cpp")

target_include_directories(DragonSlayer PRIVATE ${INCLUDE_DIRS})
target_include_directories(DragonSlayerGui PRIVATE ${INCLUDE_DIRS})
