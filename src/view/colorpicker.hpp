#ifndef VIEW_COLORPICKER_HPP
#define VIEW_COLORPICKER_HPP

#include <QWidget>
#include <QResizeEvent>

class QLineEdit;
class GroupedSlider;
class ColorWheel;

class ColorPicker : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    explicit ColorPicker(QWidget *parent = 0);

    QColor color() const;

public slots:
    void setColor(const QColor &color);

    void applyColor();
    void revertColor();

private slots:
    void setNamedColor();

signals:
    void colorChanged(const QColor &color);

private:
    ColorWheel *wheel;
    GroupedSlider *hueSlider;
    GroupedSlider *saturationSlider;
    GroupedSlider *lightnessSlider;
    QLineEdit *hexEdit;
};

#endif // VIEW_COLORPICKER_HPP
