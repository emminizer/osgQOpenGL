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
}

osgQOpenGLWidget::~osgQOpenGLWidget()
{
}

osgViewer::Viewer* osgQOpenGLWidget::getOsgViewer() const
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

void osgQOpenGLWidget::setOsgViewer(osgViewer::Viewer* viewer)
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
    if (_isFirstFrame) {
        _isFirstFrame = false;
        m_renderer->getGraphicsContext()->setDefaultFboId(defaultFramebufferObject());
    }
    m_renderer->renderFrame();
}

void osgQOpenGLWidget::keyPressEvent(QKeyEvent* event)
{
    Q_ASSERT(m_renderer);

    if(event->key() == Qt::Key_F)
    {
        static QSize g;
        static QMargins sMargins;

        if(parent() && parent()->isWidgetType())
        {
            QMainWindow* _mainwindow = dynamic_cast<QMainWindow*>(parent());

            if(_mainwindow)
            {
                g = size();

                if(layout())
                    sMargins = layout()->contentsMargins();

                bool ok = true;

                // select screen
                if(qApp->screens().size() > 1)
                {
                    QMap<QString, QScreen*> screens;
                    int screenNumber = 0;

                    for(QScreen* screen : qApp->screens())
                    {
                        QString name = screen->name();

                        if(name.isEmpty())
                        {
                            name = tr("Screen %1").arg(screenNumber);
                        }

                        name += " (" + QString::number(screen->size().width()) + "x" + QString::number(
                                    screen->size().height()) + ")";
                        screens[name] = screen;
                        ++screenNumber;
                    }

                    QString selected = QInputDialog::getItem(this,
                                                             tr("Choose fullscreen target screen"), tr("Screen"), screens.keys(), 0, false,
                                                             &ok);

                    if(ok && !selected.isEmpty())
                    {
                        context()->setScreen(screens[selected]);
                        move(screens[selected]->geometry().x(), screens[selected]->geometry().y());
                        resize(screens[selected]->geometry().width(),
                               screens[selected]->geometry().height());
                    }
                }

                if(ok)
                {
                    // in fullscreen mode, a thiner (1px) border around
                    // viewer widget
                    if(layout())
                        layout()->setContentsMargins(1, 1, 1, 1);

                    setParent(0);
                    showFullScreen();
                }
            }
        }
        else
        {
            showNormal();
            setMinimumSize(g);
            QMainWindow* _mainwindow = dynamic_cast<QMainWindow*>(parent());
            if(_mainwindow){
                _mainwindow->setCentralWidget(this);
            }

            if(layout())
                layout()->setContentsMargins(sMargins);

            qApp->processEvents();
            setMinimumSize(QSize(1, 1));
        }
    }
    else // not 'F' key
    {
        // forward event to renderer
        m_renderer->keyPressEvent(event);
    }
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
    QScreen* screen = windowHandle()
                      && windowHandle()->screen() ? windowHandle()->screen() :
                      qApp->screens().front();
    m_renderer->setupOSG(width(), height(), screen->devicePixelRatio());
}

void osgQOpenGLWidget::setTimerInterval(int intervalMs)
{
  _timerIntervalMs = intervalMs;
  if (m_renderer)
    m_renderer->setTimerInterval(_timerIntervalMs);
}
