#include <osgQOpenGL/osgQOpenGLWidget>
#include <osgQOpenGL/OSGRenderer>

#include <osgViewer/Viewer>
#include <osg/GL>

#include <QApplication>
#include <QKeyEvent>
#include <QInputDialog>
#include <QLayout>
#include <QMainWindow>
#include <QScreen>
#include <QWindow>

osgQOpenGLWidget::osgQOpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
    setMouseTracking(true);
    // Focus policy required in order to process OSG keypresses
    setFocusPolicy(Qt::StrongFocus);
}

osgQOpenGLWidget::~osgQOpenGLWidget()
{
}

osgViewer::ViewerBase* osgQOpenGLWidget::getOsgViewer() const
{
    if (!m_renderer)
        return _viewer.get();
    return m_renderer->getViewer();
}

osg::GraphicsContext* osgQOpenGLWidget::getGraphicsContext() const
{
    if (!m_renderer)
        return nullptr;
    return m_renderer->getGraphicsContext();
}

osgViewer::GraphicsWindow* osgQOpenGLWidget::getGraphicsWindow() const
{
    if (!m_renderer)
        return nullptr;
    return m_renderer->getGraphicsWindow();
}

void osgQOpenGLWidget::setOsgViewer(osgViewer::ViewerBase* viewer)
{
    _viewer = viewer;
    if (m_renderer)
        m_renderer->setViewer(_viewer.get());
}

OpenThreads::ReadWriteMutex* osgQOpenGLWidget::mutex()
{
    return &_osgMutex;
}


void osgQOpenGLWidget::initializeGL()
{
    // Initializes OpenGL function resolution for the current context.
    initializeOpenGLFunctions();
    createRenderer();
    getOsgViewer()->realize();
    Q_EMIT initialized();
}

void osgQOpenGLWidget::resizeGL(int w, int h)
{
    Q_ASSERT(m_renderer);
    QScreen* screen = windowHandle()
                      && windowHandle()->screen() ? windowHandle()->screen() :
                      qApp->screens().front();
    m_renderer->resize(w, h, screen->devicePixelRatio());
}

void osgQOpenGLWidget::paintGL()
{
    OpenThreads::ScopedReadLock locker(_osgMutex);

    // Must constantly set the default FBO ID, because Qt can change it on resize or other events
    m_renderer->getGraphicsContext()->setDefaultFboId(defaultFramebufferObject());

    if (_isFirstFrame) {
        _isFirstFrame = false;

        Q_EMIT aboutToRenderFirstFrame();
    }
    m_renderer->renderFrame();
}

void osgQOpenGLWidget::keyPressEvent(QKeyEvent* event)
{
    Q_ASSERT(m_renderer);
    // forward event to renderer
    m_renderer->keyPressEvent(event);
}

void osgQOpenGLWidget::keyReleaseEvent(QKeyEvent* event)
{
    Q_ASSERT(m_renderer);
    // forward event to renderer
    m_renderer->keyReleaseEvent(event);
}

void osgQOpenGLWidget::mousePressEvent(QMouseEvent* event)
{
    Q_ASSERT(m_renderer);
    // forward event to renderer
    m_renderer->mousePressEvent(event);
}

void osgQOpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    Q_ASSERT(m_renderer);
    // forward event to renderer
    m_renderer->mouseReleaseEvent(event);
}

void osgQOpenGLWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    Q_ASSERT(m_renderer);
    // forward event to renderer
    m_renderer->mouseDoubleClickEvent(event);
}

void osgQOpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
    Q_ASSERT(m_renderer);
    // forward event to renderer
    m_renderer->mouseMoveEvent(event);
}

void osgQOpenGLWidget::wheelEvent(QWheelEvent* event)
{
    Q_ASSERT(m_renderer);
    // forward event to renderer
    m_renderer->wheelEvent(event);
}

void osgQOpenGLWidget::setDefaultDisplaySettings()
{
    osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
    ds->setNvOptimusEnablement(1);
    ds->setStereo(false);
}

void osgQOpenGLWidget::createRenderer()
{
    // call this before creating a View...
    setDefaultDisplaySettings();
    m_renderer = new OSGRenderer(this);
    if (_viewer.valid())
        m_renderer->setViewer(_viewer.get());
    m_renderer->setTimerInterval(_timerIntervalMs);
    QScreen* screen = windowHandle()
                      && windowHandle()->screen() ? windowHandle()->screen() :
                      qApp->screens().front();
    m_renderer->setupOSG(width(), height(), screen->devicePixelRatio());
    m_renderer->setRenderFunction(_renderFunction);
}

void osgQOpenGLWidget::setTimerInterval(int intervalMs)
{
    _timerIntervalMs = intervalMs;
    if (m_renderer)
        m_renderer->setTimerInterval(_timerIntervalMs);
}

void osgQOpenGLWidget::setRenderFunction(const std::function<void(double simulationTime)>& renderFunc)
{
    _renderFunction = renderFunc;
    if (m_renderer)
        m_renderer->setRenderFunction(_renderFunction);
}
