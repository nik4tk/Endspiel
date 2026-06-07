cmake_minimum_required(VERSION 3.10)

project(Endspiel)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(Endspiel
    src/main.cpp
    src/board.cpp
    src/movegen.cpp
    src/eval.cpp
    src/search.cpp
    include/move.h
    include/board.h
    include/movegen.h
    include/eval.h
    include/search.h
 "src/performanceTest.cpp" "include/performanceTest.h" "include/hash.h" "src/hash.cpp"
        include/tuning.h)