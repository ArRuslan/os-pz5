cmake_minimum_required(VERSION 3.28)
project(os_pz5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUNICODE -D_UNICODE")

add_executable(os_pz5 main.cpp)
