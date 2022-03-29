cmake_minimum_required(VERSION 3.11)

project(CTFTool)

add_executable(CTFTool CTFTool.cpp)

add_subdirectory(ctf-tool)
add_subdirectory(tests)

target_link_libraries(CTFTool ctf-tool)