SET(TARGET_SRC
    osgQtWidgets.cpp
)

IF(CMAKE_COMPILER_IS_GNUCXX)
    # Remove -pedantic flag as it barfs on Qt headers
   STRING(REGEX REPLACE "-pedantic" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
ENDIF()

if ( QT4_FOUND )
    SET(TARGET_EXTERNAL_LIBRARIES ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTOPENGL_LIBRARY} ${QT_QTWEBKIT_LIBRARY})
    INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR} ${QT_QTCORE_INCLUDE_DIR} ${QT_QTOPENGL_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR} ${QT_QTWEBKIT_INCLUDE_DIR} )
endif( QT4_FOUND )

SET(TARGET_LIBRARIES  ${OPENSCENEGRAPH_LIBRARIES})

#### end var setup  ###

# Optionally find Web components for browser examples
if(TARGET Qt6::Widgets)
    find_package(Qt6 COMPONENTS WebEngineWidgets QUIET)
elseif(TARGET Qt5::Widgets)
    find_package(Qt5 COMPONENTS WebKitWidgets QUIET)
endif()

IF ( TARGET Qt6::WebEngineWidgets )
    SET ( EXAMPLE_NAME osgQt6Widgets )
    SET ( BUILD_EXAMPLE TRUE )
ELSEIF ( TARGET Qt5::WebKitWidgets )
    SET ( EXAMPLE_NAME osgQt5Widgets )
    SET ( BUILD_EXAMPLE TRUE )
ELSEIF ( QT4_FOUND )
    SET ( EXAMPLE_NAME osgQtWidgets )
    SET ( BUILD_EXAMPLE TRUE )
ELSE()
    SET ( BUILD_EXAMPLE FALSE )
ENDIF()

IF ( BUILD_EXAMPLE )
    SETUP_EXAMPLE(${EXAMPLE_NAME})

    IF ( TARGET Qt6::WebEngineWidgets )
        target_link_libraries(example_${EXAMPLE_NAME} Qt6::OpenGL Qt6::OpenGLWidgets Qt6::WebEngineWidgets)
    ELSEIF ( TARGET Qt5::WebKitWidgets )
        target_link_libraries(example_${EXAMPLE_NAME} Qt5::OpenGL Qt5::WebKitWidgets)
    ENDIF()
ENDIF()
