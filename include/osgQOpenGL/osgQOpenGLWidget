#ifndef OSGQOPENGLWIDGET_H
#define OSGQOPENGLWIDGET_H

#ifdef __APPLE__
#   define __glext_h_
#   include <QtGui/qopengl.h>
#   undef __glext_h_
#   include <QtGui/qopenglext.h>
#endif

#include <functional>
#include <osgQOpenGL/Export>
#include <OpenThreads/ReadWriteMutex>
#include <osg/observer_ptr>

#ifdef WIN32
//#define __gl_h_
#include <osg/GL>
#endif

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QReadWriteLock>

class OSGRenderer;

namespace osg { class GraphicsContext; }
namespace osgViewer
{
    class GraphicsWindow;
    class ViewerBase;
}

class OSGQOPENGL_EXPORT osgQOpenGLWidget : public QOpenGLWidget,
    protected QOpenGLFunctions
{
    Q_OBJECT

protected:
    // Owned and deleted through Qt parent
    OSGRenderer* m_renderer {nullptr};
    bool _osgWantsToRenderFrame{true};
    OpenThreads::ReadWriteMutex _osgMutex;
	bool _isFirstFrame {true};
    int _timerIntervalMs {10};
    osg::observer_ptr<osgViewer::ViewerBase> _viewer;
    std::function<void(double simulationTime)> _renderFunction;

    friend class OSGRenderer;

public:
    osgQOpenGLWidget(QWidget* parent = nullptr);
    virtual ~osgQOpenGLWidget();

    //! Sets the timer interval for updating the graphics, in ms
    void setTimerInterval(int intervalMs);

    /** Get osgViewer View */
    osgViewer::ViewerBase* getOsgViewer() const;
    /** Retrieve the graphics context */
    osg::GraphicsContext* getGraphicsContext() const;
    /** Retrieve the graphics window */
    osgViewer::GraphicsWindow* getGraphicsWindow() const;

    /** Sets the osgViewer that will be used when creating the rendering context. Call at initialization only. */
    void setOsgViewer(osgViewer::ViewerBase* viewer);

    //! get mutex
    virtual OpenThreads::ReadWriteMutex* mutex();

    //! override this to change default size or aspect ratio
    virtual QSize sizeHint() const override { return QSize(640,480); }

    /** Set an overriding render function. By default this calls the viewer's frame() */
    void setRenderFunction(const std::function<void(double simulationTime)>& renderFunc);

Q_SIGNALS:
    void initialized();
    void aboutToRenderFirstFrame();

protected:

    //! call createRender. If overloaded, this method must send initialized signal at end
    virtual void initializeGL() override;

    virtual void resizeGL(int w, int h) override;

    //! lock scene graph and call osgViewer::frame()
    virtual void paintGL() override;

    //! called before creating renderer
    virtual void setDefaultDisplaySettings();

    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void keyReleaseEvent(QKeyEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;

    void createRenderer();

private:
};

#endif // OSGQOPENGLWIDGET_H
