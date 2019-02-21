cmake_minimum_required(VERSION 3.0.0)
project(SensorDetectionTesting VERSION 0.1.0)

include(CTest)
enable_testing()

add_library(Libs libraries/Average.cpp)
add_executable(SensorDetectionTesting main.cpp)
target_link_libraries(SensorDetectionTesting Libs)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
