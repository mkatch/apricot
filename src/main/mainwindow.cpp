#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QDockWidget>

#include "newprojectdialog.hpp"

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
    m_project(nullptr)
{
    ui->setupUi(this);
    dockHacking();
    connectViews();
    groupOnionSkinActions();
}

/*!
 * Destroys the widget.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setProject(Project *project, bool own)
{
    if (m_project == project)
        return;

    connect(project, SIGNAL(framesChanged()), ui->frameView, SLOT(setOnionSkinFrames()) );
    ui->animationView->setProject(project);

    if (m_project != nullptr && m_project->parent() == this)
        m_project->setParent(nullptr);
    if (ownProject && m_project != nullptr)
        delete m_project;

    m_project = project;
    ownProject = own;
    project->setParent(this);

    if (project->objectName().isEmpty())
        setWindowTitle("Apricot — untitled");
    else
        setWindowTitle("Apricot — " + QFileInfo(project->objectName()).fileName());
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
    // Qt Designer does not recognize slots of docked widgets, so we have to do it by hand.

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
    connect(
        ui->colorPicker, SIGNAL(colorChanged(QColor)),
        ui->toolbox, SLOT(setActiveColor(QColor))
    );
    connect(
        ui->toolbox, SIGNAL(activeColorChanged(QColor)),
        ui->colorPicker, SLOT(setColor(QColor))
    );
    ui->frameView->setTool(ui->toolbox->activeTool());
}

void MainWindow::groupOnionSkinActions()
{
    QActionGroup *backwardGroup = new QActionGroup(this);
    backwardGroup->addAction(ui->actionOnionSkinBackward0);
    backwardGroup->addAction(ui->actionOnionSkinBackward1);
    backwardGroup->addAction(ui->actionOnionSkinBackward2);
    backwardGroup->addAction(ui->actionOnionSkinBackward3);

    QActionGroup *forwardGroup = new QActionGroup(this);
    forwardGroup->addAction(ui->actionOnionSkinForward0);
    forwardGroup->addAction(ui->actionOnionSkinForward1);
    forwardGroup->addAction(ui->actionOnionSkinForward2);
    forwardGroup->addAction(ui->actionOnionSkinForward3);
}

QString MainWindow::projectDirectory() const
{
    return m_project == nullptr || m_project->objectName().isEmpty()
        ? QDir::homePath()
        : QFileInfo(m_project->objectName()).dir().absolutePath();
}

void MainWindow::handleOnionSkinActionToggled(bool checked)
{
    if (!checked)
        return;

    if (sender() == ui->actionOnionSkinBackward0)
        ui->frameView->setOnionSkinBackward(0);
    else if (sender() == ui->actionOnionSkinBackward1)
        ui->frameView->setOnionSkinBackward(1);
    else if (sender() == ui->actionOnionSkinBackward2)
        ui->frameView->setOnionSkinBackward(2);
    else if (sender() == ui->actionOnionSkinBackward0)
        ui->frameView->setOnionSkinBackward(3);
    else if (sender() == ui->actionOnionSkinForward0)
        ui->frameView->setOnionSkinForward(0);
    else if (sender() == ui->actionOnionSkinForward1)
        ui->frameView->setOnionSkinForward(1);
    else if (sender() == ui->actionOnionSkinForward2)
        ui->frameView->setOnionSkinForward(2);
    else if (sender() == ui->actionOnionSkinForward3)
        ui->frameView->setOnionSkinForward(3);
}

void MainWindow::handleOpenAction()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Open File",
        projectDirectory(),
        "Apricot Project (*.apr);;Image Files (*.gif *.png)"
    );
    if (fileName.isEmpty())
        return;

    setProject(Project::load(fileName));
}

void MainWindow::handleNewAction()
{
    NewProjectDialog dialog(this);
    if (dialog.exec() == QDialog::Rejected)
        return;

    Project *project = new Project;
    project->setSize(dialog.size());
    project->newFrame()->newLayer();
    setProject(project);
}

void MainWindow::handleSaveAsAction()
{
    if (project() == nullptr) {
        qWarning("MainWindow::handleSaveAsAction(): No project is set");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Save Project",
        projectDirectory(),
        "Apricot Project (*.apr)"
    );
    if (fileName.isEmpty())
        return;

    Project::save(fileName, project());
}

void MainWindow::handleSaveAction()
{
    if (project() == nullptr) {
        qWarning("MainWindow::handleSaveAction(): No project is set");
        return;
    }

    if (project()->objectName().isEmpty() || !project()->objectName().endsWith("apr"))
        return handleSaveAsAction();
    else
        Project::save(project()->objectName(), project());
}
