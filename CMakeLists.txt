cmake_minimum_required(VERSION 3.23)

project(WLalat)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(target_cxx_version)

add_library(wlalat INTERFACE)
target_include_directories(wlalat INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(ExampleCodeGenDriver ExampleCodeGenDriver.cc)
target_cxx_version(ExampleCodeGenDriver 23)
target_link_libraries(ExampleCodeGenDriver PRIVATE wlalat)

function(generate_wayland TARGET)
    set(DIR "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}_include")
    file(MAKE_DIRECTORY ${DIR})

    foreach(PROTOCOL_FILE ${ARGN})
        set(INPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${PROTOCOL_FILE})
        cmake_path(GET PROTOCOL_FILE FILENAME PROTOCOL_FILE_NAME)
        set(OUTPUT_FILE "${DIR}/${PROTOCOL_FILE_NAME}.hh")

        add_custom_command(
            OUTPUT ${OUTPUT_FILE}
            DEPENDS "$<TARGET_FILE:ExampleCodeGenDriver>" "${INPUT_FILE}"
            COMMAND "$<TARGET_FILE:ExampleCodeGenDriver>" "${INPUT_FILE}" "${OUTPUT_FILE}"
        )
        list(APPEND OUTPUT_FILES ${OUTPUT_FILE})
    endforeach()


    add_library(${TARGET} INTERFACE)
    target_include_directories(${TARGET} INTERFACE ${DIR})
    add_custom_target("${TARGET}.generate"
        DEPENDS ${OUTPUT_FILES}
    )
    add_dependencies(${TARGET} "${TARGET}.generate")
endfunction()

generate_wayland(
  gena_wl
    protos/wayland.xml
)

add_executable(Example Example.cc)
target_cxx_version(Example 23)
target_link_libraries(Example PRIVATE wlalat gena_wl)

add_executable(Parser Parser.cc)
target_cxx_version(Parser 23)
target_link_libraries(Parser PRIVATE wlalat)
