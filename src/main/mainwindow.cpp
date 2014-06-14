#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QDockWidget>

/*!
 * \class MainWindow
 * \inmodule main
 *
 * \brief The main window of the application.
 */

// Properties

/*!
 * \property MainWindow::project
 * \brief The displayed Project.
 */

// Methods

/*!
 * Constructs MainWindow with parent widget \a parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_frameView(new AnimationFrameView),
    m_animationView(new AnimationView),
    m_layerView(new LayerView),
    m_toolbox(new Toolbox(this))
{
    ui->setupUi(this);

    setCentralWidget(frameView());

    QDockWidget *dockableAnimationView = new QDockWidget;
    dockableAnimationView->setWidget(animationView());
    dockableAnimationView->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    this->addDockWidget(Qt::BottomDockWidgetArea, dockableAnimationView);

    QDockWidget *dockableLayerView = new QDockWidget;
    dockableLayerView->setWidget(layerView());
    dockableLayerView->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    this->addDockWidget(Qt::RightDockWidgetArea, dockableLayerView);

    toolbox()->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, toolbox());

    connect(
        toolbox(), SIGNAL(activeToolChanged(Tool*)),
        frameView(), SLOT(setTool(Tool*))
    );

    // The order of connections is important!
    connect(
        m_animationView, SIGNAL(activeFrameChanged(AnimationFrame*)),
        m_frameView, SLOT(setFrame(AnimationFrame*))
    );
    connect(
        m_animationView, SIGNAL(activeFrameChanged(AnimationFrame*)),
        m_layerView, SLOT(setFrame(AnimationFrame*))
    );
    connect(
        m_animationView, SIGNAL(onionSkinFramesChanged(QList<AnimationFrame*>)),
        m_frameView, SLOT(setOnionSkinFrames(QList<AnimationFrame*>))
    );
    connect(
        m_layerView, SIGNAL(activeLayerChanged(Layer*)),
        m_frameView, SLOT(setActiveLayer(Layer*))
    );

}

/*!
 * Destroys the widget.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setProject(Project *project)
{
    if (m_project == project)
        return;

    m_project = project;
    animationView()->setProject(project);
    frameView()->setFrame(project->frame(0));
    layerView()->setFrame(project->frame(0));
}

/*!
 * \fn MainWindow::frameView()
 * \brief Returns the AnimationFrameView child.
 */

/*!
 * \fn MainWindow::animationView()
 * \brief Returns the AnimationView child.
 */

/*!
 * \fn MainWindow::layerView()
 * \brief Returns the LayerView child.
 */
