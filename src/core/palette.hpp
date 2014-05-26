#ifndef PALETTE_H
#define PALETTE_H

#include <QWidget>
#include <QAbstractListModel>
#include <QStyledItemDelegate>
#include <QColor>
#include <QVector>


namespace Ui {
class Palette;
}


class ColorListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ColorListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

private:
    QVector<QColor *> colorList;
};


class ColorDialogDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ColorDialogDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
};


class Palette : public QWidget
{
    Q_OBJECT

public:
    explicit Palette(QWidget *parent = nullptr);
    ~Palette();

private slots:
    void on_addButton_clicked();
    void on_removeButton_clicked();

private:
    Ui::Palette *ui;

    ColorListModel *paletteModel;
};


#endif // PALETTE_H
