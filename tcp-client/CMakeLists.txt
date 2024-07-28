cmake_minimum_required(VERSION 3.28)
project(tcp-client)

set(CMAKE_CXX_STANDARD 17)

add_executable(client)
target_sources(client
        PRIVATE
            main.cpp)