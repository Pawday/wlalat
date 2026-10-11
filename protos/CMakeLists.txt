function(generate_wayland TARGET)
    set(DIR "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}_include")
    file(MAKE_DIRECTORY ${DIR})
    set(OUTPUT_FILE "${DIR}/${TARGET}.hh")

    foreach(PROTOCOL_FILE ${ARGN})
        list(APPEND INPUT_FILES ${PROTOCOL_FILE})
    endforeach()

    list(APPEND ARGS ${INPUT_FILES})
    list(APPEND ARGS ${OUTPUT_FILE})

    add_custom_command(
        OUTPUT ${OUTPUT_FILE}
        DEPENDS "$<TARGET_FILE:ExampleCodeGenDriver>" ${INPUT_FILES}
        COMMAND "$<TARGET_FILE:ExampleCodeGenDriver>" ${ARGS}
    )

    add_library(${TARGET} INTERFACE)
    target_include_directories(${TARGET} INTERFACE ${DIR})
    add_custom_target("${TARGET}.generate"
        DEPENDS ${OUTPUT_FILE}
    )
    add_dependencies(${TARGET} "${TARGET}.generate")
endfunction()

file(GLOB PROTOS CONFIGURE_DEPENDS "*.xml")
generate_wayland(gena_wl ${PROTOS})
