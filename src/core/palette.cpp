#include "Palette.h"
#include "ui_palette.h"

#include <QColorDialog>


ColorListModel::ColorListModel(QObject *parent) :
    QAbstractListModel(parent)
{
}


int ColorListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return colorList.count();
}


Qt::ItemFlags ColorListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}


QVariant ColorListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount()) {
        return QVariant();
    }

    if(role == Qt::DisplayRole || role == Qt::EditRole || role == Qt::DecorationRole) {
        return *colorList.at(index.row());
    }

    return QVariant();
}


bool ColorListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole) {
        return false;
    }

    *colorList[index.row()] = value.value<QColor>();
    emit dataChanged(index, index);
    return true;
}


bool ColorListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; i++) {
        colorList.append(new QColor());
    }
    endInsertRows();
    return true;
}


bool ColorListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; i++) {
        delete colorList.at(row);
        colorList.removeAt(row);
    }
    endRemoveRows();
    return true;
}


ColorDialogDelegate::ColorDialogDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}


QWidget *ColorDialogDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QColorDialog *picker = new QColorDialog(parent);
    return picker;
}


void ColorDialogDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QColorDialog *picker = static_cast<QColorDialog *>(editor);

    QColor color = index.model()->data(index, Qt::EditRole).value<QColor>();
    picker->setCurrentColor(color);
}


void ColorDialogDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QColorDialog *picker = static_cast<QColorDialog *>(editor);
    model->setData(index, picker->currentColor(), Qt::EditRole);
}


Palette::Palette(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Palette),
    paletteModel(new ColorListModel(this))
{
    ui->setupUi(this);

    ui->paletteListView->setModel(paletteModel);

    ColorDialogDelegate *colorDialogDelegate = new ColorDialogDelegate(this);
    ui->paletteListView->setItemDelegate(colorDialogDelegate);
}


Palette::~Palette()
{
    delete ui;
}


void Palette::on_addButton_clicked()
{
    paletteModel->insertRow(paletteModel->rowCount());
}


void Palette::on_removeButton_clicked()
{
    QModelIndex index = ui->paletteListView->currentIndex();
    if (index.isValid())
        paletteModel->removeRow(index.row());
}
