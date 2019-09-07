cmake_minimum_required(VERSION 2.8)
project( facenet )

SET(CMAKE_C_COMPILER arm-buildroot-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER arm-buildroot-linux-gnueabihf-c++)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

aux_source_directory(./src/ SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

include_directories( ../ncnn/build/src )
include_directories( ../ncnn/src )

target_link_libraries(facenet ${PROJECT_SOURCE_DIR}/../ncnn/build/src/libncnn.a)
target_link_libraries(facenet -lopencv_core)
target_link_libraries(facenet -lopencv_imgproc)
target_link_libraries(facenet -lopencv_videoio)
target_link_libraries(facenet -lopencv_imgcodecs)
