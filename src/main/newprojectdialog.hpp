#ifndef NEWPROJECTDIALOG_HPP
#define NEWPROJECTDIALOG_HPP

#include <QDialog>

namespace Ui {
class NewProjectDialog;
}

class NewProjectDialog : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QSize projectSize READ projectSize)
public:
    explicit NewProjectDialog(QWidget *parent = 0);
    ~NewProjectDialog();

    QSize projectSize() const;

private:
    Ui::NewProjectDialog *ui;
};

#endif // NEWPROJECTDIALOG_HPP
