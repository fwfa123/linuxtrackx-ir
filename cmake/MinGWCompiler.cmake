if(NOT MINGW_FOUND)
    message(FATAL_ERROR "MinGW toolchain not found; call detect_mingw() first.")
endif()

function(_mingw_tools_for_arch arch out_cc out_cxx out_res)
    if(arch STREQUAL "x64")
        set(${out_cc} ${MINGW64_GCC_EXECUTABLE} PARENT_SCOPE)
        set(${out_cxx} ${MINGW64_GPP_EXECUTABLE} PARENT_SCOPE)
        set(${out_res} ${MINGW64_WINDRES_EXECUTABLE} PARENT_SCOPE)
    else()
        set(${out_cc} ${MINGW32_GCC_EXECUTABLE} PARENT_SCOPE)
        set(${out_cxx} ${MINGW32_GPP_EXECUTABLE} PARENT_SCOPE)
        set(${out_res} ${MINGW32_WINDRES_EXECUTABLE} PARENT_SCOPE)
    endif()
endfunction()

function(_mingw_compile_source src obj arch)
    _mingw_tools_for_arch(${arch} MINGW_CC MINGW_CXX MINGW_RES)
    if(IS_ABSOLUTE ${src})
        set(src_path ${src})
    else()
        set(src_path ${CMAKE_CURRENT_SOURCE_DIR}/${src})
    endif()

    set(_inc
        -DHAVE_CONFIG_H
        -I${CMAKE_SOURCE_DIR}
        -I${CMAKE_SOURCE_DIR}/src
        -I${CMAKE_BINARY_DIR}
        -I${CMAKE_BINARY_DIR}/src
        -I${CMAKE_CURRENT_SOURCE_DIR}
        -I${CMAKE_CURRENT_SOURCE_DIR}/../..
    )

    if(src MATCHES "\\.rc$")
        add_custom_command(
            OUTPUT ${obj}
            COMMAND ${MINGW_RES} -o ${obj} -I${CMAKE_CURRENT_SOURCE_DIR} ${src_path}
            DEPENDS ${src_path}
            COMMENT "Compiling resource ${src} (${arch})"
        )
    elseif(src MATCHES "\\.cpp$")
        add_custom_command(
            OUTPUT ${obj}
            COMMAND ${MINGW_CXX} -c ${src_path} -g -O2 ${_inc} -o ${obj}
            DEPENDS ${src_path}
            COMMENT "Compiling C++ ${src} (${arch})"
        )
    else()
        add_custom_command(
            OUTPUT ${obj}
            COMMAND ${MINGW_CC} -c ${src_path} -g -O2 ${_inc} -o ${obj}
            DEPENDS ${src_path}
            COMMENT "Compiling C ${src} (${arch})"
        )
    endif()
endfunction()

function(add_mingw_executable target)
    set(oneValueArgs ARCH)
    set(multiValueArgs SOURCES LIBS)
    cmake_parse_arguments(MW "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    if(NOT MW_ARCH)
        set(MW_ARCH x86)
    endif()
    _mingw_tools_for_arch(${MW_ARCH} MINGW_CC MINGW_CXX MINGW_RES)

    set(objects)
    foreach(src ${MW_SOURCES})
        get_filename_component(src_name ${src} NAME_WE)
        set(obj ${CMAKE_CURRENT_BINARY_DIR}/${target}_${MW_ARCH}_${src_name}.o)
        list(APPEND objects ${obj})
        _mingw_compile_source(${src} ${obj} ${MW_ARCH})
    endforeach()

    set(_mingw_runtime_flags -static-libgcc -static-libstdc++)

    add_custom_target(${target} ALL
        COMMAND ${MINGW_CXX} ${_mingw_runtime_flags} -o ${CMAKE_CURRENT_BINARY_DIR}/${target}.exe ${objects} ${MW_LIBS}
        DEPENDS ${objects}
        COMMENT "Linking ${target}.exe (${MW_ARCH})"
    )
endfunction()

function(add_mingw_library target)
    set(oneValueArgs ARCH DEF)
    set(multiValueArgs SOURCES LIBS)
    cmake_parse_arguments(MW "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    if(NOT MW_ARCH)
        set(MW_ARCH x86)
    endif()
    _mingw_tools_for_arch(${MW_ARCH} MINGW_CC MINGW_CXX MINGW_RES)

    set(objects)
    foreach(src ${MW_SOURCES})
        get_filename_component(src_name ${src} NAME_WE)
        set(obj ${CMAKE_CURRENT_BINARY_DIR}/${target}_${MW_ARCH}_${src_name}.o)
        list(APPEND objects ${obj})
        _mingw_compile_source(${src} ${obj} ${MW_ARCH})
    endforeach()

    set(_def_arg)
    if(MW_DEF)
        if(IS_ABSOLUTE ${MW_DEF})
            set(_def_arg ${MW_DEF})
        else()
            set(_def_arg ${CMAKE_CURRENT_SOURCE_DIR}/${MW_DEF})
        endif()
    endif()

    set(_mingw_runtime_flags -static-libgcc -static-libstdc++)

    add_custom_target(${target} ALL
        COMMAND ${MINGW_CXX} ${_mingw_runtime_flags} -shared -Wl,--kill-at -o ${CMAKE_CURRENT_BINARY_DIR}/${target}.dll ${objects} ${_def_arg} ${MW_LIBS}
        DEPENDS ${objects}
        COMMENT "Linking ${target}.dll (${MW_ARCH})"
    )
endfunction()
