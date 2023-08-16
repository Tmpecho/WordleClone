cmake_minimum_required(VERSION 3.26)
project(WordleClone)

set(CMAKE_CXX_STANDARD 17)

add_executable(WordleClone main.cpp
        WordFileReader.cpp
        WordFileReader.h
        TextFormatter.cpp
        TextFormatter.h
)
