# CMake script to fix help.qhc file paths after installation
# This fixes the relative path in help.qhc to point to help.qch in the same directory

set(HELP_QHC_FILE "${HELP_QHC_FILE}")
set(HELP_QCH_FILE "${HELP_QCH_FILE}")

if(NOT EXISTS "${HELP_QHC_FILE}")
    message(WARNING "help.qhc file not found: ${HELP_QHC_FILE} - skipping path fix")
    return()
endif()

if(NOT EXISTS "${HELP_QCH_FILE}")
    message(WARNING "help.qch file not found: ${HELP_QCH_FILE} - skipping path fix")
    return()
endif()

# Find sqlite3 command
find_program(SQLITE3_EXECUTABLE sqlite3)

if(SQLITE3_EXECUTABLE)
    # Fix the path in help.qhc using sqlite3
    # The NamespaceTable contains the path to help.qch
    # We need to update it to use just 'help.qch' (same directory)
    # During installation, the file should be writable
    execute_process(
        COMMAND ${SQLITE3_EXECUTABLE} "${HELP_QHC_FILE}" 
            "UPDATE NamespaceTable SET FilePath='help.qch' WHERE FilePath LIKE '%help.qch'"
        OUTPUT_QUIET
        ERROR_QUIET
        RESULT_VARIABLE sqlite_result
    )
    
    if("${sqlite_result}" STREQUAL "0")
        message(STATUS "Fixed help.qhc path reference to help.qch")
    else()
        message(WARNING "Failed to update help.qhc paths using sqlite3 (result: ${sqlite_result})")
        message(WARNING "The help.qhc file may need manual path correction")
    endif()
else()
    message(WARNING "sqlite3 not found - cannot fix help.qhc paths automatically")
    message(WARNING "The help.qhc file contains a path that may not work after installation")
    message(WARNING "Install sqlite3 package and reinstall to fix help system paths")
endif()

