cmake_minimum_required(VERSION 3.14)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS ${SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS ${INCLUDE_DIR}/*.h)
file(GLOB_RECURSE VENDOR_FILES ${VENDOR_DIR}/*.c*)

include_directories(.)
include_directories(${VENDOR_DIR})

find_package(glfw3 3.3 REQUIRED)

add_executable(Painter ${SOURCE_FILES} ${VENDOR_FILES})
set_property(TARGET Painter PROPERTY CXX_STANDARD 23)

target_link_libraries(Painter glfw)