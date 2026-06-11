cmake_minimum_required(VERSION 3.23)

project(WLalat)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(target_cxx_version)

add_library(wlalat INTERFACE)
target_include_directories(wlalat INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(ExampleCodeGenDriver ExampleCodeGenDriver.cc)
target_cxx_version(ExampleCodeGenDriver 23)
target_link_libraries(ExampleCodeGenDriver PRIVATE wlalat)
add_subdirectory(protos)

add_executable(Example Example.cc)
target_cxx_version(Example 23)
target_link_libraries(Example PRIVATE wlalat gena_wl)

add_executable(Parser Parser.cc)
target_cxx_version(Parser 23)
target_link_libraries(Parser PRIVATE wlalat)
