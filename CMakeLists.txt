cmake_minimum_required(VERSION 3.10)

project(media_indexer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(media_indexer
    test.cpp
    media_scanner.cpp
)