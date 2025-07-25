#include <osgQOpenGL/osgQOpenGLWindow>
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


osgQOpenGLWindow::osgQOpenGLWindow(QWidget* parent)
    : QOpenGLWindow(QOpenGLWindow::NoPartialUpdate, nullptr)
{
    _widget = QWidget::createWindowContainer(this);
    _widget->setMouseTracking(true);
}

osgQOpenGLWindow::~osgQOpenGLWindow()
{
}

osgViewer::ViewerBase* osgQOpenGLWindow::getOsgViewer() const
{
    if (!m_renderer)
        return _viewer.get();
    return m_renderer->getViewer();
}

osg::GraphicsContext* osgQOpenGLWindow::getGraphicsContext() const
{
    if (!m_renderer)
        return nullptr;
    return m_renderer->getGraphicsContext();
}

osgViewer::GraphicsWindow* osgQOpenGLWindow::getGraphicsWindow() const
{
    if (!m_renderer)
        return nullptr;
    return m_renderer->getGraphicsWindow();
}

void osgQOpenGLWindow::setOsgViewer(osgViewer::ViewerBase* viewer)
{
    _viewer = viewer;
    if (m_renderer)
        m_renderer->setViewer(_viewer.get());
}

OpenThreads::ReadWriteMutex* osgQOpenGLWindow::mutex()
{
    return &_osgMutex;
}


void osgQOpenGLWindow::initializeGL()
{
    // Initializes OpenGL function resolution for the current context.
    initializeOpenGLFunctions();
    createRenderer();
    getOsgViewer()->realize();
    Q_EMIT initialized();
}

void osgQOpenGLWindow::resizeGL(int w, int h)
{
    Q_ASSERT(m_renderer);
    double pixelRatio = screen()->devicePixelRatio();
    m_renderer->resize(w, h, pixelRatio);
}

void osgQOpenGLWindow::paintGL()
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

void osgQOpenGLWindow::keyPressEvent(QKeyEvent* event)
{
    Q_ASSERT(m_renderer);
    // forward event to renderer
    m_renderer->keyPressEvent(event);
}

void osgQOpenGLWindow::keyReleaseEvent(QKeyEvent* event)
{
    Q_ASSERT(m_renderer);
    // forward event to renderer
    m_renderer->keyReleaseEvent(event);
}

void osgQOpenGLWindow::mousePressEvent(QMouseEvent* event)
{
    Q_ASSERT(m_renderer);
    // forward event to renderer
    m_renderer->mousePressEvent(event);
}

void osgQOpenGLWindow::mouseReleaseEvent(QMouseEvent* event)
{
    Q_ASSERT(m_renderer);
    // forward event to renderer
    m_renderer->mouseReleaseEvent(event);
}

void osgQOpenGLWindow::mouseDoubleClickEvent(QMouseEvent* event)
{
    Q_ASSERT(m_renderer);
    // forward event to renderer
    m_renderer->mouseDoubleClickEvent(event);
}

void osgQOpenGLWindow::mouseMoveEvent(QMouseEvent* event)
{
    Q_ASSERT(m_renderer);
    // forward event to renderer
    m_renderer->mouseMoveEvent(event);
}

void osgQOpenGLWindow::wheelEvent(QWheelEvent* event)
{
    Q_ASSERT(m_renderer);
    // forward event to renderer
    m_renderer->wheelEvent(event);
}

void osgQOpenGLWindow::setDefaultDisplaySettings()
{
    osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
    ds->setNvOptimusEnablement(1);
    ds->setStereo(false);
}

void osgQOpenGLWindow::createRenderer()
{
    // call this before creating a View...
    setDefaultDisplaySettings();

    m_renderer = new OSGRenderer(this);
    if (_viewer.valid())
        m_renderer->setViewer(_viewer.get());
    m_renderer->setTimerInterval(_timerIntervalMs);
    double pixelRatio = screen()->devicePixelRatio();
    m_renderer->setupOSG(width(), height(), pixelRatio);
    m_renderer->setRenderFunction(_renderFunction);
}

void osgQOpenGLWindow::setTimerInterval(int intervalMs)
{
    _timerIntervalMs = intervalMs;
    if (m_renderer)
        m_renderer->setTimerInterval(_timerIntervalMs);
}

void osgQOpenGLWindow::setRenderFunction(const std::function<void(double simulationTime)>& renderFunc)
{
    _renderFunction = renderFunc;
    if (m_renderer)
        m_renderer->setRenderFunction(_renderFunction);
}
