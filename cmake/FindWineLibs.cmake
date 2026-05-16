# FindWineLibs.cmake
# Multi-distribution Wine library path detection (matching configure.ac logic).
# Returns WINE32_FOUND and WINE64_FOUND to indicate if libraries are actually available.
#
# Detection approach: we probe a fixed list of known distro paths (directory existence).
# Override: set WINE_LIBS_PATH / WINE64_LIBS_PATH (e.g. -DWINE64_LIBS_PATH=/path) to use
# a custom path when the distro layout is not in the list.
#
# Downsides of directory-only probing:
#   - Does not verify that the libs are linkable (correct arch, ABI); a try_compile
#     would be more robust but adds build-time cost and cross-arch complexity.
#   - New distro layouts require adding paths here (or users pass override).
# Professional builds often use pkg-config or config scripts when available; Wine does
# not ship pkg-config for its Unix libs, so path probing + override is the common approach.

function(detect_wine_libs)
    if(UNIX AND NOT APPLE)
        # Detect 32-bit Wine libraries
        set(WINE_LIBS_DEFAULT "")
        set(WINE32_FOUND FALSE)
        
        # Arch Linux with wine32 from AUR
        if(EXISTS "/usr/lib32/wine/i386-unix")
            set(WINE_LIBS_DEFAULT "-L/usr/lib32/wine/i386-unix")
            set(WINE32_FOUND TRUE)
            message(STATUS "Found Arch Linux wine32 installation in /usr/lib32/wine/i386-unix")
        elseif(EXISTS "/usr/lib32/wine")
            set(WINE_LIBS_DEFAULT "-L/usr/lib32/wine")
            set(WINE32_FOUND TRUE)
            message(STATUS "Found Arch Linux wine32 installation in /usr/lib32/wine")
        # Modern Ubuntu/Debian (22.04+) - prefer explicit 32-bit unix dir
        elseif(EXISTS "/usr/lib/i386-linux-gnu/wine/i386-unix")
            set(WINE_LIBS_DEFAULT "-L/usr/lib/i386-linux-gnu/wine/i386-unix")
            set(WINE32_FOUND TRUE)
            message(STATUS "Found Debian/Ubuntu wine32 libraries in /usr/lib/i386-linux-gnu/wine/i386-unix")
        # Fallback for older Debian/Ubuntu layouts
        elseif(EXISTS "/usr/lib/i386-linux-gnu/wine")
            set(WINE_LIBS_DEFAULT "-L/usr/lib/i386-linux-gnu/wine")
            set(WINE32_FOUND TRUE)
            message(STATUS "Found Debian/Ubuntu wine32 libraries in /usr/lib/i386-linux-gnu/wine")
        # Fedora (43+) wine-wow64 layout
        elseif(EXISTS "/usr/lib/wine-wow64/wine")
            set(WINE_LIBS_DEFAULT "-L/usr/lib/wine-wow64/wine")
            set(WINE32_FOUND TRUE)
            message(STATUS "Found Fedora wine32 libraries in /usr/lib/wine-wow64/wine")
        # Modern Fedora (38+)
        elseif(EXISTS "/usr/lib/wine")
            # Check if this is actually 32-bit (could be 64-bit on some systems)
            # For now, assume it's 32-bit if no other path found
            set(WINE_LIBS_DEFAULT "-L/usr/lib/wine")
            set(WINE32_FOUND TRUE)
            message(STATUS "Found Fedora wine32 libraries in /usr/lib/wine")
        else()
            set(WINE_LIBS_DEFAULT "-L/usr/lib/wine")
            set(WINE32_FOUND FALSE)
            message(STATUS "32-bit Wine libraries not found - 32-bit components will not be built")
        endif()
        
        # Use user-provided path if set
        if(WINE_LIBS_PATH)
            set(WINE_LIBS_DEFAULT "-L${WINE_LIBS_PATH}")
            # If user provided path, assume it exists
            if(NOT WINE32_FOUND)
                set(WINE32_FOUND TRUE)
            endif()
        endif()
        
        # Set as cache variable for global access
        set(WINE_LIBS ${WINE_LIBS_DEFAULT} CACHE STRING "Wine 32-bit library path" FORCE)
        set(WINE32_FOUND ${WINE32_FOUND} CACHE BOOL "32-bit Wine libraries found" FORCE)
        
        # Detect 64-bit Wine libraries (always check - 64-bit is easier to install)
        set(WINE64_LIBS_DEFAULT "")
        set(WINE64_FOUND FALSE)
        
        # Modern Ubuntu/Debian (22.04+) - prefer explicit 64-bit unix dir
        if(EXISTS "/usr/lib/x86_64-linux-gnu/wine/x86_64-unix")
            set(WINE64_LIBS_DEFAULT "-L/usr/lib/x86_64-linux-gnu/wine/x86_64-unix")
            set(WINE64_FOUND TRUE)
            message(STATUS "Found Ubuntu/Debian wine64 libraries in /usr/lib/x86_64-linux-gnu/wine/x86_64-unix")
        # Fallback for older Debian/Ubuntu layouts
        elseif(EXISTS "/usr/lib/x86_64-linux-gnu/wine")
            set(WINE64_LIBS_DEFAULT "-L/usr/lib/x86_64-linux-gnu/wine")
            set(WINE64_FOUND TRUE)
            message(STATUS "Found Ubuntu/Debian wine64 libraries in /usr/lib/x86_64-linux-gnu/wine")
        # Arch Linux with wine64
        elseif(EXISTS "/usr/lib/wine/x86_64-unix")
            set(WINE64_LIBS_DEFAULT "-L/usr/lib/wine/x86_64-unix")
            set(WINE64_FOUND TRUE)
            message(STATUS "Found Arch Linux wine64 installation in /usr/lib/wine/x86_64-unix")
        # Fedora (43+) wine-wow64 layout
        elseif(EXISTS "/usr/lib64/wine-wow64/wine")
            set(WINE64_LIBS_DEFAULT "-L/usr/lib64/wine-wow64/wine")
            set(WINE64_FOUND TRUE)
            message(STATUS "Found Fedora wine64 libraries in /usr/lib64/wine-wow64/wine")
        # Modern Fedora (38+)
        elseif(EXISTS "/usr/lib64/wine")
            set(WINE64_LIBS_DEFAULT "-L/usr/lib64/wine")
            set(WINE64_FOUND TRUE)
            message(STATUS "Found Fedora wine64 libraries in /usr/lib64/wine")
        # Generic fallback - check if directory exists
        elseif(EXISTS "/usr/lib/wine")
            set(WINE64_LIBS_DEFAULT "-L/usr/lib/wine")
            set(WINE64_FOUND TRUE)
            message(STATUS "Using default wine64 path: /usr/lib/wine")
        else()
            set(WINE64_LIBS_DEFAULT "-L/usr/lib/x86_64-linux-gnu/wine")
            set(WINE64_FOUND FALSE)
            message(WARNING "64-bit Wine libraries not found - 64-bit components will not be built")
        endif()
        
        # Use user-provided path if set
        if(WINE64_LIBS_PATH)
            set(WINE64_LIBS_DEFAULT "-L${WINE64_LIBS_PATH}")
            # If user provided path, assume it exists
            if(NOT WINE64_FOUND)
                set(WINE64_FOUND TRUE)
            endif()
        endif()
        
        # Set as cache variable for global access
        set(WINE64_LIBS ${WINE64_LIBS_DEFAULT} CACHE STRING "Wine 64-bit library path" FORCE)
        set(WINE64_FOUND ${WINE64_FOUND} CACHE BOOL "64-bit Wine libraries found" FORCE)
    else()
        set(WINE_LIBS "-L/usr/lib/wine" CACHE STRING "Wine 32-bit library path" FORCE)
        set(WINE64_LIBS "-L/usr/lib/wine" CACHE STRING "Wine 64-bit library path" FORCE)
        set(WINE32_FOUND FALSE CACHE BOOL "32-bit Wine libraries found" FORCE)
        set(WINE64_FOUND FALSE CACHE BOOL "64-bit Wine libraries found" FORCE)
    endif()
endfunction()

# Wine Windows API headers (windows.h, windef.h) — required for winegcc/wrc.
# winegcc normally adds these when libwine-dev is installed; we pass -I explicitly
# so builds work on Debian/Ubuntu/Fedora layouts and fail clearly when headers are absent.
function(detect_wine_headers)
    set(_dirs "")
    set(_found FALSE)

    # Probe known distro layouts (path -> extra -I dirs for wine/debug.h etc.)
    set(_layouts
        "/usr/include/wine/windows.h|/usr/include/wine"
        "/usr/include/wine/windows/windows.h|/usr/include/wine/windows;/usr/include/wine"
        "/usr/include/wine/wine/windows/windows.h|/usr/include/wine/wine/windows;/usr/include/wine/wine"
    )
    foreach(_entry IN LISTS _layouts)
        string(REPLACE "|" ";" _parts "${_entry}")
        list(GET _parts 0 _marker)
        list(REMOVE_AT _parts 0)
        if(EXISTS "${_marker}")
            set(_found TRUE)
            foreach(_d IN LISTS _parts)
                list(APPEND _dirs "${_d}")
            endforeach()
        endif()
    endforeach()

    if(_dirs)
        list(REMOVE_DUPLICATES _dirs)
    endif()

    set(WINE_HEADERS_FOUND ${_found} CACHE BOOL "Wine windows.h headers found" FORCE)
    set(WINE_INCLUDE_DIRS "${_dirs}" CACHE STRING "Wine header include directories" FORCE)

    set(_flags "")
    foreach(_d IN LISTS _dirs)
        list(APPEND _flags "-I${_d}")
    endforeach()
    set(WINE_INCLUDE_FLAGS "${_flags}" CACHE STRING "Wine header -I flags for winegcc/wrc" FORCE)

    if(_found)
        message(STATUS "Wine headers: ${WINE_INCLUDE_DIRS}")
    else()
        message(STATUS "Wine headers: not found (install libwine-dev / wine-devel)")
    endif()
endfunction()

