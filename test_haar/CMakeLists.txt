cmake_minimum_required(VERSION 2.8)
project(test_haar)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenCV)

include_directories(${OpenCV_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS})

add_executable(test_haar src/test_haar.cpp)

target_link_libraries(test_haar ${OpenCV_LIBS})
