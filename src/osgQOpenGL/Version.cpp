#include <string>
#include <stdio.h>
#include <osgQOpenGL/Version>

extern "C" {

const char* osgQOpenGLGetVersion()
{
    static char osgqopengl_version[256];
    static int osgqopengl_version_init = 1;
    if (osgqopengl_version_init)
    {
        sprintf(osgqopengl_version,"%d.%d.%d build %d",
            OSGQOPENGL_MAJOR_VERSION,
            OSGQOPENGL_MINOR_VERSION,
            OSGQOPENGL_PATCH_VERSION,
            OSGQOPENGL_SOVERSION);

        osgqopengl_version_init = 0;
    }

    return osgqopengl_version;
}

const char* osgQOpenGLGetSOVersion()
{
    static char osgqopengl_soversion[32];
    static int osgqopengl_soversion_init = 1;
    if (osgqopengl_soversion_init)
    {
        sprintf(osgqopengl_soversion,"%d", OSGQOPENGL_SOVERSION);
        osgqopengl_soversion_init = 0;
    }

    return osgqopengl_soversion;
}

const char* osgQOpenGLGetLibraryName()
{
    return "osgQOpenGL OpenSceneGraph QOpenGL Library";
}

}
