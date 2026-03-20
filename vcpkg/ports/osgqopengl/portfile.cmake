vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO emminizer/osgQOpenGL
    HEAD_REF main
    REF 01c9ec1
    SHA512 8f6fd89597d59ef304b976318cd7c2dd52343763d93434284359ab2dcec9e37653bcc73701c20cd448f93533f379205286c8c7132e6ec7ce6ca1d0709ea76586
    PATCHES
)

# Use this instead of vcpkg_from_github() when testing locally
#get_filename_component(SOURCE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../.." ABSOLUTE)

# If the 'qt5' feature was enabled, we tell CMake to desire Qt5
set(QT_VERSION_TO_USE 6) # Default to 6
if("qt5" IN_LIST FEATURES)
    set(QT_VERSION_TO_USE 5)
endif()

# Configure
vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DDESIRED_QT_VERSION=${QT_VERSION_TO_USE}
        -DBUILD_OSG_EXAMPLES=OFF
        -DOSG_MSVC_VERSIONED_DLL=OFF
)

# Build/install
vcpkg_cmake_install()
vcpkg_cmake_config_fixup(
    PACKAGE_NAME osgQOpenGL
    CONFIG_PATH share/cmake/osgQOpenGL
)

# Fix up results for vcpkg
file(REMOVE_RECURSE
    "${CURRENT_PACKAGES_DIR}/debug/include"
    "${CURRENT_PACKAGES_DIR}/debug/share"
)

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE.txt")
file(INSTALL "${CMAKE_CURRENT_LIST_DIR}/usage" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}")
