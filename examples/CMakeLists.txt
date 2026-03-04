#######################################################
# this are setting used in SETUP_EXAMPLE macro
#######################################################
SET(TARGET_DEFAULT_PREFIX "example_")
SET(TARGET_DEFAULT_LABEL_PREFIX "Examples")

IF(DYNAMIC_OPENSCENEGRAPH)

# Examples compatible with osgQOpenGL library
ADD_SUBDIRECTORY(osgviewerQt)
ADD_SUBDIRECTORY(osgqfont)

# TODO: These examples require osgQt specific features not available in osgQOpenGL:
# - osgQtWidgets: requires QGraphicsViewAdapter, QWidgetImage (advanced Qt-OSG integration)
# - osgQtBrowser: requires QWebViewImage, QGraphicsViewAdapter (web browser in OSG)
# These would need significant refactoring to work with osgQOpenGL
# ADD_SUBDIRECTORY(osgQtWidgets)
# ADD_SUBDIRECTORY(osgQtBrowser)

ENDIF()
