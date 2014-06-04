#include "mainwindow.hpp"
#include "ui_mainwindow.h"

/*!
  \class MainWindow
  \inmodule main
  \brief The Main Window class.
 */

/*!
  Constructor.
  \a parent parent widget.
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

/*!
  Destructor.
 */
MainWindow::~MainWindow()
{
    delete ui;
}
