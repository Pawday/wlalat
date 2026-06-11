function(generate_wayland TARGET)
    set(DIR "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}_include")
    file(MAKE_DIRECTORY ${DIR})

    foreach(PROTOCOL_FILE ${ARGN})
        set(INPUT_FILE ${PROTOCOL_FILE})
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

file(GLOB PROTOS "*.xml")
generate_wayland(gena_wl ${PROTOS})
