# src/wine_bridge/CMakeLists.txt
# Wine bridge components

if(WINE_PLUGIN)
    include(${CMAKE_SOURCE_DIR}/cmake/WineCompiler.cmake)
    
    # Add subdirectories
    add_subdirectory(client)
    add_subdirectory(controller)
    add_subdirectory(tester)
    add_subdirectory(ft_client)
    add_subdirectory(ft_tester)
    add_subdirectory(views)
    
    # NSIS installer generation (if makensis is available)
    if(MAKENSIS_EXECUTABLE)
        # This would need to be customized based on the .nsi files
        # For now, just note that it needs to be built
        message(STATUS "NSIS installer support: enabled (makensis found)")
    else()
        message(STATUS "NSIS installer support: disabled (makensis not found)")
    endif()
endif()

