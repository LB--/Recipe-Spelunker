cmake_minimum_required(VERSION 3.0.1)

project(resplunk-server)

file(GLOB_RECURSE API_Headers "${PROJECT_SOURCE_DIR}/include/*.h*")
file(GLOB_RECURSE Impl_Headers "${PROJECT_SOURCE_DIR}/src/*.h*")
file(GLOB_RECURSE Impl_Sources "${PROJECT_SOURCE_DIR}/src/*.c*")
include_directories("${PROJECT_SOURCE_DIR}/include/" "${PROJECT_SOURCE_DIR}/src/")
link_directories("${PROJECT_SOURCE_DIR}/lib")

add_executable(resplunk-server ${Impl_Sources})
#target_link_libraries(resplunk-client ${Impl_Libraries})
