cmake_minimum_required(VERSION 3.5)

project(WavAnalyzer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(raylib)
    endif()
endif()

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE
	src/main.cpp
	src/types.cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC src)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

if (APPLE)
	target_link_libraries(${PROJECT_NAME} PRIVATE "-framework IOKit")
	target_link_libraries(${PROJECT_NAME} PRIVATE "-framework Cocoa")
	target_link_libraries(${PROJECT_NAME} PRIVATE "-framework OpenGL")
endif()

# include(cmake/CPM.cmake)
