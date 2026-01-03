# 版本更新函数（核心）
function(update_version level jump)
    # 解析当前版本
    file(READ ${VERSION_FILE} VERSION_CONTENT)
    string(REGEX MATCH "\"([0-9.]+)\"" _ ${VERSION_CONTENT})
    set(CURRENT_VERSION ${CMAKE_MATCH_1})
    string(REPLACE "." ";" VERSION_PARTS ${CURRENT_VERSION})
    list(GET VERSION_PARTS 0 MAJOR)
    list(GET VERSION_PARTS 1 MINOR)
    list(GET VERSION_PARTS 2 PATCH)

    # 默认步长1
    if(NOT jump)
        set(jump 1)
    endif()

    # 更新版本号
    if(level STREQUAL "major")
        math(EXPR NEW_MAJOR "${MAJOR} + ${jump}")
        set(NEW_MINOR 0)
        set(NEW_PATCH 0)
    elseif(level STREQUAL "minor")
        set(NEW_MAJOR ${MAJOR})
        math(EXPR NEW_MINOR "${MINOR} + ${jump}")
        set(NEW_PATCH 0)
    else() # patch（默认）
        set(NEW_MAJOR ${MAJOR})
        set(NEW_MINOR ${MINOR})
        math(EXPR NEW_PATCH "${PATCH} + ${jump}")
    endif()

    # 生成新版本
    set(NEW_VERSION "${NEW_MAJOR}.${NEW_MINOR}.${NEW_PATCH}")
    file(WRITE ${VERSION_FILE} "#define SUCECODE_VERSION \"${NEW_VERSION}\"")
    
    # 输出日志
    message(STATUS "Version updated: ${CURRENT_VERSION} → ${NEW_VERSION}")
endfunction()

# 版本更新目标（支持参数：major/minor/patch，可选步长）
add_custom_target(
    update_version_major
    COMMAND ${CMAKE_COMMAND} -DVERSION_LEVEL=major -P ${CMAKE_CURRENT_LIST_FILE}
    COMMENT "Updating MAJOR version (X.0.0)"
)
add_custom_target(
    update_version_minor
    COMMAND ${CMAKE_COMMAND} -DVERSION_LEVEL=minor -P ${CMAKE_CURRENT_LIST_FILE}
    COMMENT "Updating MINOR version (X.Y.0)"
)
add_custom_target(
    update_version_patch
    COMMAND ${CMAKE_COMMAND} -DVERSION_LEVEL=patch -P ${CMAKE_CURRENT_LIST_FILE}
    COMMENT "Updating PATCH version (X.Y.Z)"
)

# 直接执行版本更新（当调用该脚本时）
if(DEFINED VERSION_LEVEL)
    update_version(${VERSION_LEVEL} ${VERSION_JUMP})
endif()