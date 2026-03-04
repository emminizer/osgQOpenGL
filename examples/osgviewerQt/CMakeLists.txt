SET( TARGET_SRC
    osgviewerQt.cpp
)

SET(TARGET_LIBRARIES_VARS
    OSG_LIBRARY
    OSGUTIL_LIBRARY
    OSGVIEWER_LIBRARY
    OSGGA_LIBRARY
    OSGDB_LIBRARY
    OPENTHREADS_LIBRARY
)


SET (EXAMPLE_NAME osgviewerQt )

SETUP_EXAMPLE( ${EXAMPLE_NAME} )

if(TARGET Qt6::Widgets)
    set(TARGET_LIBRARIES Qt6::Widgets Qt6::OpenGL Qt6::OpenGLWidgets)
elseif(TARGET Qt5::Widgets)
    set(TARGET_LIBRARIES Qt5::Widgets Qt5::OpenGL)
endif()

target_link_libraries(example_${EXAMPLE_NAME} osgQOpenGL)
