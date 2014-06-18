#include "newprojectdialog.hpp"
#include "ui_newprojectdialog.h"

NewProjectDialog::NewProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewProjectDialog)
{
    ui->setupUi(this);
}

NewProjectDialog::~NewProjectDialog()
{
    delete ui;
}

QSize NewProjectDialog::projectSize() const
{
    return QSize(ui->widthSpinBox->value(), ui->heightSpinBox->value());
}
