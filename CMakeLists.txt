cmake_minimum_required(VERSION 3.5)

project(WavAnalyzer)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(CMAKE_CXX_STANDARD 17)

add_executable(WavAnalyzer)
target_sources(WavAnalyzer PRIVATE
	src/main.cpp
)
set_target_properties(WavAnalyzer PROPERTIES COMPILE_FLAGS "-Wall -Wextra -Wpedantic -g3")
target_include_directories(WavAnalyzer PUBLIC src)
