cmake_minimum_required(VERSION 3.18)
project(SuceCode)

# ===================== 基础配置 =====================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(VERSION_FILE ${CMAKE_SOURCE_DIR}/src/sucecode/version)

# ===================== 加载模块 =====================
include(cmake/config.cmake)    # 加载debug/release配置
include(cmake/version_update.cmake) # 版本更新

# ===================== 版本解析 =====================
# 读取版本文件
if(NOT EXISTS ${VERSION_FILE})
    file(WRITE ${VERSION_FILE} "#define SUCECODE_VERSION \"1.0.0\"")
endif()
file(READ ${VERSION_FILE} VERSION_CONTENT)
string(REGEX MATCH "\"([0-9.]+)\"" _ ${VERSION_CONTENT})
set(SUCECODE_VERSION ${CMAKE_MATCH_1})
string(REPLACE "." ";" VERSION_PARTS ${SUCECODE_VERSION})
list(GET VERSION_PARTS 0 SUCECODE_VERSION_MAJOR)
list(GET VERSION_PARTS 1 SUCECODE_VERSION_MINOR)
list(GET VERSION_PARTS 2 SUCECODE_VERSION_PATCH)

# ===================== 源文件处理 =====================
# 收集源文件
file(GLOB_RECURSE SOURCES 
    ${SCAN_DIR}/*.cpp
    ${SCAN_DIR}/*.h
    ${SCAN_DIR}/*.hpp
)

# 过滤忽略文件
foreach(IGNORE_PATTERN ${IGNORE_PATTERNS})
    list(FILTER SOURCES EXCLUDE REGEX "${IGNORE_PATTERN}")
endforeach()

# ===================== 构建目标 =====================
# 可执行文件
add_executable(${OUTPUT_NAME}
    ${SOURCES}
    ${CMAKE_SOURCE_DIR}/src/res.rc # 设置图标
)

# 包含目录
target_include_directories(${OUTPUT_NAME} 
    PRIVATE ${INCLUDE_DIRS}
    PRIVATE ${CMAKE_BINARY_DIR}/src
)

# 编译/链接选项
target_compile_options(${OUTPUT_NAME} PRIVATE ${EXTRA_FLAGS})
target_link_options(${OUTPUT_NAME} PRIVATE ${LINK_EXTRA_FLAGS})

# ===================== 辅助目标 =====================
# 清理缓存（替代--clear）
add_custom_target(
    clear_cache
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/bin
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/CMakeCache.txt
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMENT "Clearing build cache and binaries"
)

# 运行程序（替代--run）
add_custom_target(
    run
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${OUTPUT_NAME}
    DEPENDS ${OUTPUT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running ${OUTPUT_NAME}..."
)

# 构建+运行（debug默认）
add_custom_target(
    build_and_run
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ${OUTPUT_NAME}
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target run
    COMMENT "Building and running ${OUTPUT_NAME}"
)

# 构建所有配置
add_custom_target(
    build_all
    COMMAND ${CMAKE_COMMAND} -S ${CMAKE_SOURCE_DIR} -B ${CMAKE_BINARY_DIR}/debug -DBUILD_TYPE=debug
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/debug
    COMMAND ${CMAKE_COMMAND} -S ${CMAKE_SOURCE_DIR} -B ${CMAKE_BINARY_DIR}/release -DBUILD_TYPE=release
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/release
    COMMENT "Building debug and release configurations"
)

# ===================== 状态输出 =====================
message(STATUS "=====================================")
message(STATUS "Project: ${PROJECT_NAME} v${SUCECODE_VERSION}")
message(STATUS "Build type: ${BUILD_TYPE}")
message(STATUS "Output: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${OUTPUT_NAME}")
message(STATUS "=====================================")