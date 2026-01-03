# 默认构建类型
if(NOT DEFINED BUILD_TYPE)
    set(BUILD_TYPE "debug")
endif()

# Debug配置
if(BUILD_TYPE STREQUAL "debug")
    set(SCAN_DIR "${CMAKE_SOURCE_DIR}/src")
    set(OUTPUT_NAME "sucecode_dbg")
    set(IGNORE_PATTERNS ".*test/.*" ".*third_party/.*")
    set(INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/src" "${CMAKE_SOURCE_DIR}/include")
    set(EXTRA_FLAGS "-g" "-Wall" "-Wextra")
    set(LINK_EXTRA_FLAGS "")
    set(AUTO_RUN OFF)

# Release配置
elseif(BUILD_TYPE STREQUAL "release")
    set(SCAN_DIR "${CMAKE_SOURCE_DIR}/src")
    set(OUTPUT_NAME "sucecode")
    set(IGNORE_PATTERNS ".*test/.*")
    set(INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/src" "${CMAKE_SOURCE_DIR}/include")
    set(EXTRA_FLAGS "-O3" "-DNDEBUG" "-march=native")
    set(LINK_EXTRA_FLAGS "-s") # 剥离符号（Linux/macOS）
    set(AUTO_RUN OFF)
endif()