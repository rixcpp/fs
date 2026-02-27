# ====================================================================
# Rix - FS Module
# ====================================================================
# Purpose:
#   Build configuration for the fs module.
#
#   This module provides filesystem utilities for Rix:
#     - path helpers
#     - file exists/size helpers
#     - directory helpers
#     - small file ops helpers
#
# Design:
#   - Header-only by default (INTERFACE library)
#   - If any .cpp files exist under src/, we switch to a STATIC lib
#
# Targets:
#   - rix_fs      : The actual library target (STATIC or INTERFACE)
#   - rix::fs     : Namespaced alias for consumers
#
# Installation:
#   - Installs headers under <prefix>/include/rix/...
#   - Contributes to the umbrella export-set `RixTargets`
# ====================================================================

cmake_minimum_required(VERSION 3.20)
project(rix_fs VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

file(GLOB_RECURSE RIX_FS_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

option(RIX_FS_ENABLE_SANITIZERS "Enable address/UB sanitizers for fs" OFF)
option(RIX_FS_BUILD_TESTS "Build fs tests" ON)
option(RIX_FS_BUILD_EXAMPLES "Build fs examples" ON)

function(rix_fs_apply_warnings tgt scope)
  if (NOT TARGET ${tgt})
    return()
  endif()

  if (MSVC)
    target_compile_options(${tgt} ${scope} /W4 /permissive-)
  else()
    target_compile_options(${tgt} ${scope} -Wall -Wextra -Wshadow -Wpedantic)
  endif()
endfunction()

function(rix_fs_apply_sanitizers tgt scope)
  if (NOT RIX_FS_ENABLE_SANITIZERS OR NOT TARGET ${tgt})
    return()
  endif()

  if (MSVC)
    message(WARNING "[fs] Sanitizers are not enabled on MSVC in this module.")
    return()
  endif()

  target_compile_options(${tgt} ${scope}
    -O1 -g -fno-omit-frame-pointer -fsanitize=address,undefined
  )
  target_link_options(${tgt} ${scope}
    -fsanitize=address,undefined
  )
endfunction()

if (RIX_FS_SOURCES)
  message(STATUS "[fs] Building STATIC library (sources found).")

  add_library(rix_fs STATIC ${RIX_FS_SOURCES})
  add_library(rix::fs ALIAS rix_fs)

  target_compile_features(rix_fs PUBLIC cxx_std_20)

  target_include_directories(rix_fs
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )

  rix_fs_apply_warnings(rix_fs PUBLIC)
  rix_fs_apply_sanitizers(rix_fs PRIVATE)

  set_target_properties(rix_fs PROPERTIES
    OUTPUT_NAME rix_fs
    VERSION ${PROJECT_VERSION}
    SOVERSION 0
    EXPORT_NAME fs
  )

  install(TARGETS rix_fs
    EXPORT RixTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )
else()
  message(STATUS "[fs] Building HEADER-ONLY library (no sources).")

  add_library(rix_fs INTERFACE)
  add_library(rix::fs ALIAS rix_fs)

  target_compile_features(rix_fs INTERFACE cxx_std_20)

  target_include_directories(rix_fs INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )

  rix_fs_apply_warnings(rix_fs INTERFACE)
  rix_fs_apply_sanitizers(rix_fs INTERFACE)

  install(TARGETS rix_fs
    EXPORT RixTargets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )
endif()

install(
  DIRECTORY include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
)

if (RIX_FS_BUILD_TESTS)
  enable_testing()

  add_executable(rix_fs_tests
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_basic.cpp
  )
  target_link_libraries(rix_fs_tests PRIVATE rix_fs)

  rix_fs_apply_warnings(rix_fs_tests PRIVATE)
  rix_fs_apply_sanitizers(rix_fs_tests PRIVATE)

  add_test(NAME rix_fs_tests COMMAND rix_fs_tests)
endif()

if (RIX_FS_BUILD_EXAMPLES)
  function(rix_fs_add_example exe_name src_file)
    add_executable(${exe_name} ${src_file})
    target_link_libraries(${exe_name} PRIVATE rix_fs)
    rix_fs_apply_warnings(${exe_name} PRIVATE)
    rix_fs_apply_sanitizers(${exe_name} PRIVATE)
  endfunction()

  # Keep this list small at first; add more as the module grows.
  set(RIX_FS_EXAMPLES
    exists_and_size
    read_text
    write_text
    list_dir
  )

  foreach(ex ${RIX_FS_EXAMPLES})
    rix_fs_add_example(rix_fs_${ex}
      ${CMAKE_CURRENT_SOURCE_DIR}/examples/${ex}.cpp
    )
  endforeach()
endif()

message(STATUS "------------------------------------------------------")
message(STATUS "rix::fs configured (${PROJECT_VERSION})")
if (RIX_FS_SOURCES)
  message(STATUS "Mode: STATIC / sources found")
else()
  message(STATUS "Mode: HEADER-ONLY / no sources")
endif()
message(STATUS "Sanitizers enabled: ${RIX_FS_ENABLE_SANITIZERS}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Binary dir: ${CMAKE_BINARY_DIR}")
message(STATUS "------------------------------------------------------")
