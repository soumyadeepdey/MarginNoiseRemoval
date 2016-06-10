cmake_minimum_required(VERSION 2.8)
project( DRD )
find_package( OpenCV REQUIRED )
add_executable( margin margindetection.cpp )
target_link_libraries( margin ${OpenCV_LIBS} )

