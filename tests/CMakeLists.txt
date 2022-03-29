cmake_minimum_required(VERSION 3.11)

project(CTFTool-Tests)

add_executable(tests tests.cpp)
target_link_libraries(tests ctf-tool)