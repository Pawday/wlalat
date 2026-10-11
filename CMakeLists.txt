cmake_minimum_required(VERSION 3.23)

project(WLalat)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(target_cxx_version)
include(CMakeDependentOption)

add_library(wlalat INTERFACE)
target_include_directories(wlalat INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(HAS_EXPAT OFF)
find_library(WLALAT_EXPAT_LIBRARY expat)
if(WLALAT_EXPAT_LIBRARY)
    set(HAS_EXPAT ON)
endif()
cmake_dependent_option(WLALAT_ENABLE_EXPAT_PARSER_DRIVER "" ON HAS_EXPAT OFF)
if(WLALAT_ENABLE_EXPAT_PARSER_DRIVER)
    add_executable(Parser Parser.cc)
    target_cxx_version(Parser 23)
    target_link_libraries(Parser PRIVATE wlalat ${WLALAT_EXPAT_LIBRARY})
endif()

add_executable(ExampleCodeGenDriver ExampleCodeGenDriver.cc)
target_cxx_version(ExampleCodeGenDriver 23)
target_link_libraries(ExampleCodeGenDriver PRIVATE wlalat)
add_subdirectory(protos)

add_executable(Example Example.cc)
target_cxx_version(Example 23)
target_link_libraries(Example PRIVATE wlalat gena_wl)

add_executable(XML XML.cc)
target_cxx_version(XML 23)
target_link_libraries(XML PRIVATE wlalat)
