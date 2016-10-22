cmake_minimum_required(VERSION 2.8)
project(squares)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})

find_package(OpenCV)
FIND_PACKAGE(CURL)


include_directories(${OpenCV_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS})

add_executable(squares src/squares.cpp)
add_executable(squares_gui src/squares_gui.cpp)
add_executable(train src/train.cpp)
add_executable(tests src/test.cpp)

target_link_libraries(squares ${OpenCV_LIBS} ${CURL_LIBS})
target_link_libraries(squares curl)
target_link_libraries(squares ${wxWidgets_LIBRARIES})

target_link_libraries(squares_gui ${wxWidgets_LIBRARIES})
target_link_libraries(squares_gui ${OpenCV_LIBS} ${CURL_LIBS})
target_link_libraries(squares_gui curl)

target_link_libraries(train ${OpenCV_LIBS} ${CURL_LIBS})
target_link_libraries(train curl)

target_link_libraries(tests ${CURL_LIBS} ${OpenCV_LIBS})
target_link_libraries(tests curl)


