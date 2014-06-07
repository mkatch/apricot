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

    ToolActionArea *actionArea();

    AnimationView *animationView();

signals:
    void projectChanged();

private:
    Ui::MainWindow *ui;

    Project *m_project;
    ToolActionArea *m_actionArea;
    AnimationView *m_animationView;
};

inline Project *MainWindow::project()
{
    return m_project;
}

inline ToolActionArea *MainWindow::actionArea()
{
    return m_actionArea;
}

inline AnimationView *MainWindow::animationView()
{
    return m_animationView;
}

#endif // MAINWINDOW_HPP
