cmake_minimum_required(VERSION 3.5)

project(WavAnalyzer)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(CMAKE_CXX_STANDARD 17)

add_executable(WavAnalyzer)
target_sources(WavAnalyzer PRIVATE
	src/main.cpp

	src/Track/Track.cpp
	src/Track/Track.h
	src/Track/play.cpp
	src/Track/track_duration.cpp
	src/Track/RawTrack.cpp
	src/Track/RawTrack.h
	src/Track/read.cpp
	src/Track/read.h
	src/Track/FourCC.cpp
	src/Track/FourCC.h

	src/OutputStream/OutputStream.cpp
	src/OutputStream/OutputStream.h

	src/Note.cpp
	src/Note.h

	src/types.cpp
	src/types.h

	src/log.hpp

	src/test/test.cpp
	src/test/test.h
)
set_target_properties(WavAnalyzer PROPERTIES COMPILE_FLAGS "-Wall -Wextra -Wpedantic -g3")
target_include_directories(WavAnalyzer PUBLIC src)

include(cmake/CPM.cmake)

CPMAddPackage(
    NAME portaudio
    GITHUB_REPOSITORY PortAudio/portaudio
    VERSION 19.7.0
)
target_include_directories(WavAnalyzer PUBLIC ${portaudio_SOURCE_DIR}/include)
target_link_libraries(WavAnalyzer portaudio)

CPMAddPackage(
    NAME fftw3
    URL "http://fftw.org/fftw-3.3.10.tar.gz"
    VERSION 3.3.10
)
target_include_directories(WavAnalyzer PUBLIC ${fftw3_SOURCE_DIR}/api)
target_link_libraries(WavAnalyzer fftw3)
