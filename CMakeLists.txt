cmake_minimum_required(VERSION 3.10)
project(TeenTalwar CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_executable(TeenTalwar ${SOURCES})
target_include_directories(TeenTalwar PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(TeenTalwar sfml-graphics sfml-window sfml-system sfml-audio)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
