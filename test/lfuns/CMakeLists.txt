set(PROJECT_ROOT ${CMAKE_CURRENT_LIST_DIR}/../..)

set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
if (APPLE)
    list(APPEND CMAKE_INSTALL_RPATH @loader_path/../lib)
elseif(UNIX)
    list(APPEND CMAKE_INSTALL_RPATH $ORIGIN/../lib)
endif()

add_executable(tgt_exe_test_lfuns)

set_property(TARGET tgt_exe_test_lfuns PROPERTY OUTPUT_NAME test_lfuns)

target_compile_definitions(
    tgt_exe_test_lfuns
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)

target_compile_features(
    tgt_exe_test_lfuns
    PRIVATE
        c_std_23
)

target_compile_options(
    tgt_exe_test_lfuns
    PRIVATE
        -Wall
        -Wextra
        -pedantic
        $<$<CONFIG:Debug>:-g>
        $<$<CONFIG:Debug>:-O0>
        $<$<CONFIG:Release>:-Werror>
)

target_include_directories(
    tgt_exe_test_lfuns
    PRIVATE
        ${PROJECT_ROOT}/include
)

target_link_libraries(
    tgt_exe_test_lfuns
    PRIVATE
        criterion
        tgt_lib_lfuns
)

target_sources(
    tgt_exe_test_lfuns
    PRIVATE
        ${PROJECT_ROOT}/test/lfuns/test_lfuns.c
)

install(TARGETS tgt_exe_test_lfuns)
