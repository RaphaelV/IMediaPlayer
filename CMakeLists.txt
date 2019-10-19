cmake_minimum_required(VERSION 3.5)

project(IMediaPlayer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "*.cpp" "*.h")
add_executable(IMediaPlayer ${SOURCES})
