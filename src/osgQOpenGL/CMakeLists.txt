# _TARGET_LIBRARIES is used in the Config.cmake.in to set proper library targets
set(_TARGET_LIBRARIES Qt::Widgets Qt::OpenGL)
if(TARGET Qt6::OpenGL)
    set(QT_VERSION_MAJOR 6)
    list(APPEND _TARGET_LIBRARIES Qt::OpenGLWidgets)
elseif(TARGET Qt5::OpenGL)
    set(QT_VERSION_MAJOR 5)
else()
    return()
endif()


include(GenerateExportHeader)

option(OSGQOPENGL_APPEND_QT_VERSION "Append the Qt major version to the library name (e.g., mylib-qt6)" OFF)
set(LIB_NAME osgQOpenGL)
if(OSGQOPENGL_APPEND_QT_VERSION)
    set(LIB_NAME ${LIB_NAME}-qt${QT_VERSION_MAJOR})
endif()


set(HEADER_INSTALL_DIR osgQOpenGL)
set(CMAKE_INCLUDE_CURRENT_DIR yes)
set(HEADER_PATH ../../include/osgQOpenGL)

set(SOURCE_H
    ${HEADER_PATH}/osgQOpenGLWidget.h
    ${HEADER_PATH}/osgQOpenGLWindow.h
    ${HEADER_PATH}/OSGRenderer.h
)
set(SOURCE_EXTENSIONLESS
    ${HEADER_PATH}/osgQOpenGLWidget
    ${HEADER_PATH}/osgQOpenGLWindow
    ${HEADER_PATH}/OSGRenderer
    ${HEADER_PATH}/Version
)

qt_wrap_cpp(SOURCES_H_MOC ${SOURCE_H})

set(TARGET_H
    ${SOURCE_H}
    ${SOURCE_EXTENSIONLESS}
    ${CMAKE_CURRENT_BINARY_DIR}/Export
    ${OSGQOPENGL_VERSION_HEADER}
)

set(TARGET_SRC
    osgQOpenGLWidget.cpp
    osgQOpenGLWindow.cpp
    OSGRenderer.cpp
    Version.cpp
    ${SOURCES_H_MOC}
    ${OPENSCENEGRAPH_VERSIONINFO_RC}
)

set(TARGET_LIBRARIES_VARS
    OSG_LIBRARY
    OSGUTIL_LIBRARY
    OSGVIEWER_LIBRARY
    OSGGA_LIBRARY
    OSGDB_LIBRARY
    OPENTHREADS_LIBRARY
)

SETUP_LIBRARY(${LIB_NAME})
target_include_directories(${LIB_NAME} PUBLIC $<INSTALL_INTERFACE:include>)

# Link to Qt; specify _QT_COMPONENTS for the configure_file() below for osgOpenGLConfig.cmake
if(QT_VERSION_MAJOR EQUAL 5)
    set(_QT_COMPONENTS "Widgets OpenGL")
    target_link_libraries(${LIB_NAME} PUBLIC Qt5::Widgets Qt5::OpenGL)
else()
    set(_QT_COMPONENTS "Widgets OpenGL OpenGLWidgets")
    target_link_libraries(${LIB_NAME} PUBLIC Qt6::Widgets Qt6::OpenGL Qt6::OpenGLWidgets)
endif()

generate_export_header(${LIB_NAME} BASE_NAME osgQOpenGL EXPORT_FILE_NAME "Export")

# Add an alias target for the latest version
if(OSGQOPENGL_APPEND_QT_VERSION)
    add_library(osgQOpenGL ALIAS ${LIB_NAME})
endif()
# Add an internal alias for osgQt::osgQOpenGL for consistency
add_library(osgQt::osgQOpenGL ALIAS ${LIB_NAME})

install(TARGETS ${LIB_NAME}
    EXPORT ${LIB_NAME}-targets
)

install(EXPORT ${LIB_NAME}-targets
    FILE ${LIB_NAME}Targets.cmake
    NAMESPACE osgQt::
    DESTINATION "share/cmake/${LIB_NAME}"
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    "osgQOpenGLConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME}Config.cmake"
    INSTALL_DESTINATION "share/cmake/${LIB_NAME}"
)

write_basic_package_version_file(
    "${LIB_NAME}ConfigVersion.cmake"
    VERSION ${OSGQOPENGL_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME}ConfigVersion.cmake"
    DESTINATION "share/cmake/${LIB_NAME}"
)
