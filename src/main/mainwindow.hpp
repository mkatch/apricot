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

    AnimationFrameView *frameView();
    AnimationView *animationView();
    LayerView *layerView();

signals:
    void projectChanged();

private:
    Ui::MainWindow *ui;

    Project *m_project;
    AnimationFrameView *m_frameView;
    AnimationView *m_animationView;
    LayerView *m_layerView;

};

inline Project *MainWindow::project()
{
    return m_project;
}

inline AnimationFrameView *MainWindow::frameView()
{
    return m_frameView;
}

inline AnimationView *MainWindow::animationView()
{
    return m_animationView;
}

inline LayerView *MainWindow::layerView()
{
    return m_layerView;
}

#endif // MAINWINDOW_HPP
