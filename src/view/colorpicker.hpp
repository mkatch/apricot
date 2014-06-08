#ifndef VIEW_COLORPICKER_HPP
#define VIEW_COLORPICKER_HPP

#include <QWidget>
#include <QSlider>
#include <QSpinBox>
#include <QGroupBox>
#include <QBoxLayout>
#include <QLineEdit>

#include <memory>
using std::unique_ptr;

class ValueSlider : public QGroupBox
{
    Q_OBJECT
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)

public:
    explicit ValueSlider(
        const QString &title,
        int minimum, int maximum,
        QWidget *parent = nullptr
    );

    int value() const;

public slots:
    void setValue(int value);

signals:
    void valueChanged(int value);

private:
    QSlider *slider;
    QSpinBox *spinBox;
};

class ColorWheel : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QSize sizeHint READ sizeHint)
    Q_PROPERTY(
        int hue
        READ hue WRITE setHue NOTIFY hueChanged
    )
    Q_PROPERTY(
        int saturation
        READ saturation WRITE setSaturation NOTIFY saturationChanged
    )
    Q_PROPERTY(
        int lightness
        READ lightness WRITE setLightness NOTIFY lightnessChanged
    )
    Q_PROPERTY(
        QColor activeColor
        READ activeColor WRITE setActiveColor NOTIFY activeColorChanged
    )
    Q_PROPERTY(
        QColor previousColor
        READ previousColor WRITE setPreviousColor NOTIFY previousColorChanged
    )

public:
    explicit ColorWheel(QWidget *parent = nullptr);

    int hue() const;
    int saturation() const;
    int lightness() const;

    QColor activeColor() const;
    QColor previousColor() const;

    QSize sizeHint() const;

protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

public slots:
    void setHue(int hue);
    void setSaturation(int saturation);
    void setLightness(int lightness);

    void setActiveColor(const QColor &color);
    void setPreviousColor(const QColor &color);

signals:
    void hueChanged(int value);
    void saturationChanged(int value);
    void lightnessChanged(int value);

    void activeColorChanged(const QColor &color);
    void previousColorChanged(const QColor &color);

private:
    static const qreal HUE_WHEEL_THICKNESS;
    static const qreal COLOR_PREVIEW_RADIUS;

    static void renderHueWheelMask(QImage &image);
    static void renderHueWheelBox(QImage &image);

    bool huePointerDragged;
    QPointF huePointerPosition() const;
    qreal huePointerRadius() const;

    unique_ptr<QImage> hueWheel;
    int m_hue, m_saturation, m_lightness;
    QColor m_previousColor;
};

class ColorPicker : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(
        QColor activeColor
        READ activeColor WRITE setActiveColor NOTIFY activeColorChanged
    )
    Q_PROPERTY(
        QColor previousColor
        READ previousColor WRITE setPreviousColor NOTIFY previousColorChanged
    )

public:
    explicit ColorPicker(QWidget *parent = 0);

    QColor activeColor() const;
    QColor previousColor() const;

public slots:
    void setActiveColor(const QColor &color);
    void setPreviousColor(const QColor &color);

    void applyColor();
    void revertColor();

private slots:
    void setNamedColor();

signals:
    void activeColorChanged(const QColor &color);
    void previousColorChanged(const QColor &color);

private:
    ColorWheel *wheel;
    ValueSlider *hueSlider, *saturationSlider, *lightnessSlider;
    QLineEdit *hexEdit;
};

#endif // VIEW_COLORPICKER_HPP
