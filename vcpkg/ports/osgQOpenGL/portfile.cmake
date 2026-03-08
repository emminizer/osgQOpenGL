#set(LOCAL_SOURCE_PATH "E:/Github/yanzixiang/osgQOpenGL")

if(DEFINED LOCAL_SOURCE_PATH)
    set(SOURCE_PATH "${LOCAL_SOURCE_PATH}")
    if(NOT EXISTS ${SOURCE_PATH})
        message(FATAL_ERROR "Local source path does not exist: ${SOURCE_PATH}")
    endif()
    message(STATUS "Using local source path from ENV: ${SOURCE_PATH}")
else()
    # Use GitHub source if no local path specified
    message(STATUS "Using GitHub source")
    vcpkg_from_github(
        OUT_SOURCE_PATH SOURCE_PATH
        REPO yanzixiang/osgQOpenGL
        REF 3e75d46
        SHA512 1eb7a401a894ceb9b2292da1cd9626f2c74335a4e1f0702fd14aac9a1a047c63a11093576197bd128926f06b2497725026f1ecc1ac8fa320eb8f8b50086bf8d5
        HEAD_REF example_and_vcpkg
    )
endif()

vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    OPTIONS
        -DOSGQOPENGL_APPEND_QT_VERSION=OFF
        -DCMAKE_PREFIX_PATH=${CURRENT_INSTALLED_DIR}/share/qt6
        -DBUILD_OSG_EXAMPLES=OFF
        -DOSG_MSVC_VERSIONED_DLL=OFF
)

vcpkg_cmake_install()

# Fixup config
vcpkg_cmake_config_fixup(
    PACKAGE_NAME osgQOpenGL
    CONFIG_PATH share/cmake/osgQOpenGL
)

file(REMOVE_RECURSE
    ${CURRENT_PACKAGES_DIR}/debug/include
    ${CURRENT_PACKAGES_DIR}/debug/share
)

file(INSTALL ${SOURCE_PATH}/LICENSE.txt DESTINATION ${CURRENT_PACKAGES_DIR}/share/${PORT} RENAME copyright)
