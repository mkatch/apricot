#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

#include <ApricotCore>
#include <ApricotView>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(Project *project READ project WRITE setProject NOTIFY projectChanged)

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Project *project();
    void setProject(Project *project);

signals:
    void projectChanged();

private:
    Ui::MainWindow *ui;

    Project *m_project;

    void dockHacking();
    void connectViews();

private slots:
    void handleOnionSkinActionToggled(bool checked);
    void handleWindowActionToggled(bool checked);
};

inline Project *MainWindow::project()
{
    return m_project;
}

#endif // MAINWINDOW_HPP
