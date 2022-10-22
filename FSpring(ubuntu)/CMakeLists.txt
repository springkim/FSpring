cmake_minimum_required(VERSION 3.8)
project(FSpring)
#Support OpenMP
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES main.cpp)
#3rdparty
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/3rdparty/include/)
LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/3rdparty/staticlib/)

add_executable(FSpring ${SOURCE_FILES})


target_link_libraries(FSpring nana X11 stdc++fs pthread fontconfig Xft opencv_world avcodec avformat avutil-ffmpeg swscale dl libwebp z png IlmImf ipp_iw ippicv ittnotify libprotobuf jpeg tiff jasper)