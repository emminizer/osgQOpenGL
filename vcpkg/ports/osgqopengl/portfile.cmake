vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO emminizer/osgQOpenGL
    HEAD_REF main
    REF 689a7e4
    SHA512 783b4f940e2611bf91d7dfacf85c4670e5ca392c7e2222445203bed060a2e295f969bf1851aefd8211f94b6bd34ab3b9f46149a53e4961acfcbb2b9e90e2705a
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
