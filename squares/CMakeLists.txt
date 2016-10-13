cmake_minimum_required(VERSION 2.8)
project( squares )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#aux_source_directory(. SRC_LIST)
#set(wxWidgets_CONFIGURATION mswu)
find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})

find_package( OpenCV )
FIND_PACKAGE(CURL)


include_directories( ${OpenCV_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS})

add_executable( squares src/squares.cpp )
add_executable(squares_api src/squares_api.cpp)
add_executable(squares_gui src/squares_gui.cpp)
#add_executable(squares ${SRC_LIST})
target_link_libraries( squares ${OpenCV_LIBS} ${CURL_LIBS})
target_link_libraries(squares curl)
target_link_libraries(squares ${wxWidgets_LIBRARIES})
target_link_libraries(squares_api ${wxWidgets_LIBRARIES})
target_link_libraries(squares_gui ${wxWidgets_LIBRARIES})


