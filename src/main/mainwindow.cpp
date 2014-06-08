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
    m_actionArea(new ToolActionArea),
    m_animationView(new AnimationView)
{
    ui->setupUi(this);

    setCentralWidget(actionArea());

    QDockWidget *dockableAnimationView = new QDockWidget;
    dockableAnimationView->setWidget(animationView());
    this->addDockWidget(Qt::BottomDockWidgetArea, dockableAnimationView);
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
}

/*!
 * \fn MainWindow::actionArea()
 * \brief Returns the ToolActionArea child.
 */

/*!
 * \fn MainWindow::animationView()
 * \brief Returns the AnimationView child.
 */
