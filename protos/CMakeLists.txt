function(generate_wayland TARGET DIR)
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

    add_custom_target("${TARGET}.generate" DEPENDS ${OUTPUT_FILE})
    add_dependencies(${TARGET} "${TARGET}.generate")
endfunction()

add_library(generated_protocols INTERFACE)

set(GEN_DIR ${CMAKE_CURRENT_BINARY_DIR})
if(NOT WLALAT_ENABLE_CODEGEN)
    message("NOT WLALAT_ENABLE_CODEGEN")
    set(GEN_DIR ${CMAKE_CURRENT_SOURCE_DIR})
else()
    if(WLALAT_CODEGEN_IN_SOURCE)
        message("WLALAT_CODEGEN_IN_SOURCE")
        set(GEN_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    endif()
    file(GLOB PROTOS CONFIGURE_DEPENDS "*.xml")
    generate_wayland(generated_protocols ${GEN_DIR} ${PROTOS})
endif()

target_include_directories(generated_protocols INTERFACE ${GEN_DIR})
