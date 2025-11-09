# WineCompiler.cmake
# Custom functions for compiling with winegcc/wineg++

# Validate required executables
if(NOT WINEGCC_EXECUTABLE)
    message(FATAL_ERROR "WINEGCC_EXECUTABLE not set. Wine support requires winegcc.")
endif()
if(NOT WINEGPP_EXECUTABLE)
    message(FATAL_ERROR "WINEGPP_EXECUTABLE not set. Wine support requires wineg++.")
endif()

# Ensure Wine library paths are set (they should be set by FindWineLibs.cmake)
# Provide defaults if not set
if(NOT DEFINED WINE_LIBS)
    set(WINE_LIBS "-L/usr/lib/wine" CACHE STRING "Wine 32-bit library path")
    message(WARNING "WINE_LIBS not set, using default: ${WINE_LIBS}")
endif()
if(NOT DEFINED WINE64_LIBS)
    set(WINE64_LIBS "-L/usr/lib/x86_64-linux-gnu/wine" CACHE STRING "Wine 64-bit library path")
    message(WARNING "WINE64_LIBS not set, using default: ${WINE64_LIBS}")
endif()

# Helper function to compile a single source file
function(wine_compile_source src obj is_64bit)
    # Determine source file path (handle relative paths)
    if(IS_ABSOLUTE ${src})
        set(src_path ${src})
    elseif(src MATCHES "^\\.\\./")
        # Relative path from current source dir
        get_filename_component(src_abs ${CMAKE_CURRENT_SOURCE_DIR}/${src} ABSOLUTE)
        set(src_path ${src_abs})
    else()
        set(src_path ${CMAKE_CURRENT_SOURCE_DIR}/${src})
    endif()
    
    # Handle resource files (.rc)
    if(src MATCHES "\\.rc$")
        if(NOT WRC_EXECUTABLE)
            message(FATAL_ERROR "WRC_EXECUTABLE not set. Resource file compilation requires wrc.")
        endif()
        add_custom_command(
            OUTPUT ${obj}
            COMMAND ${WRC_EXECUTABLE} -o ${obj} -I${CMAKE_CURRENT_SOURCE_DIR} ${src_path}
            DEPENDS ${src_path}
            COMMENT "Compiling resource file ${src} with wrc"
        )
    # Handle C++ files
    elseif(src MATCHES "\\.cpp$")
        if(is_64bit)
            add_custom_command(
                OUTPUT ${obj}
                COMMAND ${WINEGPP_EXECUTABLE} -c ${src_path}
                    -fPIC
                    -g
                    -DHAVE_CONFIG_H
                    -I${CMAKE_SOURCE_DIR}
                    -I${CMAKE_SOURCE_DIR}/src
                    -I${CMAKE_BINARY_DIR}
                    -I${CMAKE_BINARY_DIR}/src
                    -I${CMAKE_CURRENT_SOURCE_DIR}
                    -I${CMAKE_CURRENT_SOURCE_DIR}/../..
                    -o ${obj}
                DEPENDS ${src_path}
                COMMENT "Compiling ${src} with wineg++ (64-bit)"
            )
        else()
            add_custom_command(
                OUTPUT ${obj}
                COMMAND ${WINEGPP_EXECUTABLE} -c ${src_path}
                    -m32
                    -g
                    -DHAVE_CONFIG_H
                    -I${CMAKE_SOURCE_DIR}
                    -I${CMAKE_SOURCE_DIR}/src
                    -I${CMAKE_BINARY_DIR}
                    -I${CMAKE_BINARY_DIR}/src
                    -I${CMAKE_CURRENT_SOURCE_DIR}
                    -I${CMAKE_CURRENT_SOURCE_DIR}/../..
                    -o ${obj}
                DEPENDS ${src_path}
                COMMENT "Compiling ${src} with wineg++"
            )
        endif()
    # Handle C files
    else()
        if(is_64bit)
            add_custom_command(
                OUTPUT ${obj}
                COMMAND ${WINEGCC_EXECUTABLE} -c ${src_path}
                    -fPIC
                    -g
                    -DHAVE_CONFIG_H
                    -I${CMAKE_SOURCE_DIR}
                    -I${CMAKE_SOURCE_DIR}/src
                    -I${CMAKE_BINARY_DIR}
                    -I${CMAKE_BINARY_DIR}/src
                    -I${CMAKE_CURRENT_SOURCE_DIR}
                    -I${CMAKE_CURRENT_SOURCE_DIR}/../..
                    -o ${obj}
                DEPENDS ${src_path}
                COMMENT "Compiling ${src} with winegcc (64-bit)"
            )
        else()
            add_custom_command(
                OUTPUT ${obj}
                COMMAND ${WINEGCC_EXECUTABLE} -c ${src_path}
                    -m32
                    -g
                    -DHAVE_CONFIG_H
                    -I${CMAKE_SOURCE_DIR}
                    -I${CMAKE_SOURCE_DIR}/src
                    -I${CMAKE_BINARY_DIR}
                    -I${CMAKE_BINARY_DIR}/src
                    -I${CMAKE_CURRENT_SOURCE_DIR}
                    -I${CMAKE_CURRENT_SOURCE_DIR}/../..
                    -o ${obj}
                DEPENDS ${src_path}
                COMMENT "Compiling ${src} with winegcc"
            )
        endif()
    endif()
endfunction()

function(add_wine_executable target)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs SOURCES)
    cmake_parse_arguments(WINE_EXE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
    # Create object files - use target name prefix to avoid conflicts
    set(OBJECTS)
    foreach(src ${WINE_EXE_SOURCES})
        get_filename_component(src_name ${src} NAME_WE)
        # Prefix object file name with target to avoid conflicts when same source used in multiple targets
        set(obj ${CMAKE_CURRENT_BINARY_DIR}/${target}_${src_name}.o)
        list(APPEND OBJECTS ${obj})
        wine_compile_source(${src} ${obj} FALSE)
    endforeach()
    
    # Link executable
    add_custom_target(${target} ALL
        COMMAND ${WINEGCC_EXECUTABLE} ${WINE_LIBS} -m32 -Wall -Wextra -g
            -o ${CMAKE_CURRENT_BINARY_DIR}/${target}.exe.so
            ${OBJECTS}
        DEPENDS ${OBJECTS}
        COMMENT "Linking ${target} with winegcc"
    )
    
    set_target_properties(${target} PROPERTIES
        OUTPUT_NAME "${target}.exe.so"
    )
endfunction()

function(add_wine_library target)
    set(options)
    set(oneValueArgs SPEC)
    set(multiValueArgs SOURCES)
    cmake_parse_arguments(WINE_LIB "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
    # Create object files
    set(OBJECTS)
    set(SPEC_DEPS)
    
    # Handle SPEC file if provided
    if(WINE_LIB_SPEC)
        # Determine SPEC file path
        if(IS_ABSOLUTE ${WINE_LIB_SPEC})
            set(spec_path ${WINE_LIB_SPEC})
        else()
            set(spec_path ${CMAKE_CURRENT_SOURCE_DIR}/${WINE_LIB_SPEC})
        endif()
        list(APPEND SPEC_DEPS ${spec_path})
    else()
        # Try to find spec file with same name as target
        set(spec_path ${CMAKE_CURRENT_SOURCE_DIR}/${target}.spec)
        if(EXISTS ${spec_path})
            list(APPEND SPEC_DEPS ${spec_path})
        endif()
    endif()
    
    foreach(src ${WINE_LIB_SOURCES})
        get_filename_component(src_name ${src} NAME_WE)
        # Prefix object file name with target to avoid conflicts when same source used in multiple targets
        set(obj ${CMAKE_CURRENT_BINARY_DIR}/${target}_${src_name}.o)
        list(APPEND OBJECTS ${obj})
        wine_compile_source(${src} ${obj} FALSE)
    endforeach()
    
    # Link shared library
    # winegcc automatically finds .spec files in the current directory
    # We include it as a dependency so it's available during linking
    add_custom_target(${target} ALL
        COMMAND ${WINEGCC_EXECUTABLE} ${WINE_LIBS} -m32 -Wall -Wextra -g
            -shared
            -o ${CMAKE_CURRENT_BINARY_DIR}/${target}.dll.so
            ${OBJECTS}
            -ldl
        DEPENDS ${OBJECTS} ${SPEC_DEPS}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Linking ${target} with winegcc"
    )
    
    set_target_properties(${target} PROPERTIES
        OUTPUT_NAME "${target}.dll.so"
    )
endfunction()

function(add_wine64_library target)
    set(options)
    set(oneValueArgs SPEC)
    set(multiValueArgs SOURCES)
    cmake_parse_arguments(WINE_LIB "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
    # Create object files (64-bit, no -m32)
    set(OBJECTS)
    set(SPEC_DEPS)
    
    # Handle SPEC file if provided
    if(WINE_LIB_SPEC)
        # Determine SPEC file path
        if(IS_ABSOLUTE ${WINE_LIB_SPEC})
            set(spec_path ${WINE_LIB_SPEC})
        else()
            set(spec_path ${CMAKE_CURRENT_SOURCE_DIR}/${WINE_LIB_SPEC})
        endif()
        list(APPEND SPEC_DEPS ${spec_path})
    else()
        # Try to find spec file with same name as target
        set(spec_path ${CMAKE_CURRENT_SOURCE_DIR}/${target}.spec)
        if(EXISTS ${spec_path})
            list(APPEND SPEC_DEPS ${spec_path})
        endif()
    endif()
    
    foreach(src ${WINE_LIB_SOURCES})
        get_filename_component(src_name ${src} NAME_WE)
        # Prefix object file name with target to avoid conflicts when same source used in multiple targets
        set(obj ${CMAKE_CURRENT_BINARY_DIR}/${target}64_${src_name}.o)
        list(APPEND OBJECTS ${obj})
        wine_compile_source(${src} ${obj} TRUE)
    endforeach()
    
    # Link shared library (64-bit)
    # winegcc automatically finds .spec files in the current directory
    # We include it as a dependency so it's available during linking
    add_custom_target(${target}64 ALL
        COMMAND ${WINEGCC_EXECUTABLE} ${WINE64_LIBS} -Wall -Wextra -g
            -shared
            -o ${CMAKE_CURRENT_BINARY_DIR}/${target}64.dll.so
            ${OBJECTS}
            -ldl
        DEPENDS ${OBJECTS} ${SPEC_DEPS}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Linking ${target}64 with winegcc (64-bit)"
    )
    
    set_target_properties(${target}64 PROPERTIES
        OUTPUT_NAME "${target}64.dll.so"
    )
endfunction()

