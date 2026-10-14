cmake_minimum_required(VERSION 3.16)
project(ltr_installer_extractor LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

find_package(ZLIB REQUIRED)

set(MSPACK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/libmspack")

add_executable(ltr_installer_extractor
  main.c
  native_installer.c
  cfb.c
  sha256.c
  ${MSPACK_DIR}/system.c
  ${MSPACK_DIR}/cabd.c
  ${MSPACK_DIR}/lzxd.c
  ${MSPACK_DIR}/mszipd.c
  ${MSPACK_DIR}/qtmd.c
)

target_include_directories(ltr_installer_extractor PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${MSPACK_DIR}"
)
target_compile_definitions(ltr_installer_extractor PRIVATE
  _FILE_OFFSET_BITS=64
  _POSIX_C_SOURCE=200809L
  HAVE_FSEEKO=1
)
target_compile_options(ltr_installer_extractor PRIVATE
  -Wall -Wextra -Wpedantic
)
target_link_libraries(ltr_installer_extractor PRIVATE ZLIB::ZLIB)
