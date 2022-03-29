cmake_minimum_required(VERSION 3.11)

project(ctf-tool)

find_package (Threads)

file(GLOB SOURCE_FILES src/*.cpp src/operations/*.cpp)
file(GLOB HEADER_FILES include/ctf-tool/*.h include/ctf-tool/operations/*.h)

add_library(ctf-tool ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(ctf-tool PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(ctf-tool Threads::Threads)

