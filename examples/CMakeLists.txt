#######################################################
# this are setting used in SETUP_EXAMPLE macro
#######################################################
SET(TARGET_DEFAULT_PREFIX "example_")
SET(TARGET_DEFAULT_LABEL_PREFIX "Examples")

IF(DYNAMIC_OPENSCENEGRAPH)

# Examples compatible with osgQOpenGL library
ADD_SUBDIRECTORY(osgviewerQt)
ADD_SUBDIRECTORY(osgqfont)

ENDIF()

