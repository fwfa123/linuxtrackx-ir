# FindWineLibs.cmake
# Multi-distribution Wine library path detection (matching configure.ac logic)

function(detect_wine_libs)
    if(UNIX AND NOT APPLE)
        # Detect 32-bit Wine libraries
        set(WINE_LIBS_DEFAULT "")
        
        # Arch Linux with wine32 from AUR
        if(EXISTS "/usr/lib32/wine/i386-unix")
            set(WINE_LIBS_DEFAULT "-L/usr/lib32/wine/i386-unix")
            message(STATUS "Found Arch Linux wine32 installation in /usr/lib32/wine/i386-unix")
        elseif(EXISTS "/usr/lib32/wine")
            set(WINE_LIBS_DEFAULT "-L/usr/lib32/wine")
            message(STATUS "Found Arch Linux wine32 installation in /usr/lib32/wine")
        # Modern Ubuntu/Debian (22.04+)
        elseif(EXISTS "/usr/lib/i386-linux-gnu/wine")
            set(WINE_LIBS_DEFAULT "-L/usr/lib/i386-linux-gnu/wine")
            message(STATUS "Found Debian/Ubuntu wine32 libraries in /usr/lib/i386-linux-gnu/wine")
        # Modern Fedora (38+)
        elseif(EXISTS "/usr/lib/wine")
            set(WINE_LIBS_DEFAULT "-L/usr/lib/wine")
            message(STATUS "Found Fedora wine32 libraries in /usr/lib/wine")
        else()
            set(WINE_LIBS_DEFAULT "-L/usr/lib/wine")
            message(STATUS "Using default wine32 path: /usr/lib/wine")
        endif()
        
        # Use user-provided path if set
        if(WINE_LIBS_PATH)
            set(WINE_LIBS_DEFAULT "-L${WINE_LIBS_PATH}")
        endif()
        
        # Set as cache variable for global access
        set(WINE_LIBS ${WINE_LIBS_DEFAULT} CACHE STRING "Wine 32-bit library path" FORCE)
        
        # Detect 64-bit Wine libraries
        set(WINE64_LIBS_DEFAULT "")
        
        # Modern Ubuntu/Debian (22.04+)
        if(EXISTS "/usr/lib/x86_64-linux-gnu/wine")
            set(WINE64_LIBS_DEFAULT "-L/usr/lib/x86_64-linux-gnu/wine")
            message(STATUS "Found Ubuntu/Debian wine64 libraries in /usr/lib/x86_64-linux-gnu/wine")
        # Arch Linux with wine64
        elseif(EXISTS "/usr/lib/wine/x86_64-unix")
            set(WINE64_LIBS_DEFAULT "-L/usr/lib/wine/x86_64-unix")
            message(STATUS "Found Arch Linux wine64 installation in /usr/lib/wine/x86_64-unix")
        # Modern Fedora (38+)
        elseif(EXISTS "/usr/lib64/wine")
            set(WINE64_LIBS_DEFAULT "-L/usr/lib64/wine")
            message(STATUS "Found Fedora wine64 libraries in /usr/lib64/wine")
        # Generic fallback
        else()
            set(WINE64_LIBS_DEFAULT "-L/usr/lib/x86_64-linux-gnu/wine")
            message(STATUS "Using default wine64 path: /usr/lib/x86_64-linux-gnu/wine")
        endif()
        
        # Use user-provided path if set
        if(WINE64_LIBS_PATH)
            set(WINE64_LIBS_DEFAULT "-L${WINE64_LIBS_PATH}")
        endif()
        
        # Set as cache variable for global access
        set(WINE64_LIBS ${WINE64_LIBS_DEFAULT} CACHE STRING "Wine 64-bit library path" FORCE)
    else()
        set(WINE_LIBS "-L/usr/lib/wine" CACHE STRING "Wine 32-bit library path" FORCE)
        set(WINE64_LIBS "-L/usr/lib/wine" CACHE STRING "Wine 64-bit library path" FORCE)
    endif()
endfunction()

