cmake_minimum_required(VERSION 3.10)

project(tsp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "-O3" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS "-O3" CACHE STRING "" FORCE)
message(${CMAKE_C_FLAGS})
message(${CMAKE_CXX_FLAGS})

add_executable(tsp tsp.cpp solvers.cpp tsp.h solvers.h)

target_include_directories(tsp PRIVATE .)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(tsp PRIVATE Threads::Threads)
