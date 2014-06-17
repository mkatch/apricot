#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QDebug>
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
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dockHacking();
    connectViews();
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
    connect(m_project, SIGNAL(framesChanged()), ui->frameView, SLOT(setOnionSkinFrames()) );
    ui->animationView->setProject(project);
}

void MainWindow::dockHacking()
{
    QMainWindow *innerWindow = new QMainWindow;
    innerWindow->setCentralWidget(ui->frameView);
    innerWindow->addDockWidget(this->dockWidgetArea(ui->animationDock), ui->animationDock);
    this->setCentralWidget(innerWindow);
}

void MainWindow::connectViews()
{
    connect(
        ui->animationView, SIGNAL(activeFrameChanged(AnimationFrame*)),
        ui->frameView, SLOT(setFrame(AnimationFrame*))
    );
    connect(
        ui->animationView, SIGNAL(activeFrameChanged(AnimationFrame*)),
        ui->layerView, SLOT(setFrame(AnimationFrame*))
    );
    connect(
        ui->layerView, SIGNAL(activeLayerChanged(Layer*)),
        ui->frameView, SLOT(setActiveLayer(Layer*))
    );
    connect(
        ui->toolbox, SIGNAL(activeToolChanged(Tool*)),
        ui->frameView, SLOT(setTool(Tool*))
    );
    connect(
        ui->toolbox, SIGNAL(penSizeChanged(int)),
        ui->frameView, SLOT(setPenSize(int))
    );
    connect(
        ui->toolbox, SIGNAL(activeColorChanged(QColor)),
        ui->frameView, SLOT(setPenColor(QColor))
    );

    ui->frameView->setTool(ui->toolbox->activeTool());
}

void MainWindow::handleOnionSkinActionToggled(bool checked)
{
    if (!checked)
        return;

    if (sender() == ui->actionOnionSkinBackward0)
        ;
    else if (sender() == ui->actionOnionSkinBackward1)
        ;
    else if (sender() == ui->actionOnionSkinBackward2)
        ;
    else if (sender() == ui->actionOnionSkinForward0)
        ;
    else if (sender() == ui->actionOnionSkinForward1)
        ;
    else if (sender() == ui->actionOnionSkinForward2)
        ;
    else if (sender() == ui->actionOnionSkinForward3)
        ;
}

void MainWindow::handleWindowActionToggled(bool checked)
{
}
