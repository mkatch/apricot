#include "colorpicker.hpp"

#include <QtGui>
#include <QtMath>
#include <QtWidgets>

#include <memory>
using std::unique_ptr;

class GroupedSlider : public QGroupBox
{
    Q_OBJECT
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(int minimum READ minimum WRITE setMinimum)
    Q_PROPERTY(int maximum READ maximum WRITE setMaximum)

public:
    explicit GroupedSlider(const QString &title, QWidget *parent = nullptr);

    int value() const;
    int minimum() const;
    int maximum() const;

    void setMinimum(int minimum);
    void setMaximum(int maximum);

public slots:
    void setValue(int value);

signals:
    void valueChanged(int value);

private:
    QSlider *slider;
    QSpinBox *spinBox;
};

GroupedSlider::GroupedSlider(const QString &title, QWidget *parent) :
    QGroupBox(title, parent)
{
    slider = new QSlider(Qt::Horizontal, this);
    spinBox = new QSpinBox(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(slider);
    layout->addWidget(spinBox);
    setLayout(layout);
    setContentsMargins(-10, 5, -10, -10);

    connect(slider, SIGNAL(valueChanged(int)), spinBox, SLOT(setValue(int)));
    connect(spinBox, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(setValue(int)));
}

inline int GroupedSlider::value() const
{
    return slider->value();
}

inline int GroupedSlider::minimum() const
{
    return slider->minimum();
}

inline int GroupedSlider::maximum() const
{
    return slider->maximum();
}

inline void GroupedSlider::setMinimum(int minimum)
{
    slider->setMinimum(minimum);
    spinBox->setMinimum(minimum);
}

inline void GroupedSlider::setMaximum(int maximum)
{
    slider->setMaximum(maximum);
    spinBox->setMaximum(maximum);
}

inline void GroupedSlider::setValue(int value)
{
    slider->setValue(value);
    emit valueChanged(value);
}

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
    static const int HUE_BOX_SIZE;

    void renderHueWheelBox();
    void renderHueWheelMask();
    void renderHueWheel();

    bool huePointerDragged;
    QPointF huePointerPosition() const;
    qreal huePointerRadius() const;

    unique_ptr<QImage> hueBox, hueMask, hueWheel;
    int m_hue, m_saturation, m_lightness;
    QColor m_previousColor;
};

const qreal ColorWheel::HUE_WHEEL_THICKNESS = 0.2;
const qreal ColorWheel::COLOR_PREVIEW_RADIUS = 0.65;
const int ColorWheel::HUE_BOX_SIZE = 16;

ColorWheel::ColorWheel(QWidget *parent) :
    QWidget(parent),
    hueBox(new QImage(HUE_BOX_SIZE, HUE_BOX_SIZE, QImage::Format_ARGB32)),
    hueMask(new QImage()),
    hueWheel(new QImage()),
    m_hue(0), m_saturation(0), m_lightness(0),
    m_previousColor(Qt::black)
{
    QSizePolicy policy;
    policy.setHorizontalPolicy(QSizePolicy::MinimumExpanding);
    policy.setVerticalPolicy(QSizePolicy::MinimumExpanding);
    setSizePolicy(policy);
}

inline int ColorWheel::hue() const
{
    return m_hue;
}

inline int ColorWheel::saturation() const
{
    return m_saturation;
}

inline int ColorWheel::lightness() const
{
    return m_lightness;
}

inline QColor ColorWheel::activeColor() const
{
    QColor color;
    color.setHslF(hue() / 360.0, saturation() / 100.0, lightness() / 100.0);
    return color;
}

inline QColor ColorWheel::previousColor() const
{
    return m_previousColor;
}

QSize ColorWheel::sizeHint() const
{
    return QSize(150, 150);
}

void ColorWheel::setHue(int hue)
{
    if (m_hue == hue)
        return;

    m_hue = hue;
    emit hueChanged(hue);
    emit activeColorChanged(activeColor());
    update();
}

void ColorWheel::setSaturation(int saturation)
{
    if (m_saturation == saturation)
        return;

    m_saturation = saturation;
    emit saturationChanged(saturation);
    emit activeColorChanged(activeColor());
    update();
}

void ColorWheel::setLightness(int lightness)
{
    if (m_lightness == lightness)
        return;

    m_lightness = lightness;
    emit lightnessChanged(lightness);
    emit activeColorChanged(activeColor());
    update();
}

void ColorWheel::setActiveColor(const QColor &color)
{
    if (color == activeColor())
        return;

    setHue(qRound(color.hslHueF() * 360.0));
    setSaturation(qRound(color.hslSaturationF() * 100.0));
    setLightness(qRound(color.lightnessF() * 100.0));
}

void ColorWheel::setPreviousColor(const QColor &color)
{
    if (color == m_previousColor)
        return;

    m_previousColor = color;
    emit previousColorChanged(color);
    update();
}

static const qreal pi = 4.0 * qAtan2(1.0, 1.0);

static inline qreal angleNorm(qreal x, qreal y)
{
    qreal theta = qAtan2(y, x);
    if (theta < 0.0)
        theta += 2.0 * pi;
    return theta / (2.0 * pi);
}

void ColorWheel::renderHueWheelMask()
{
    int width = hueMask->width(), height = hueMask->height();

    QPainter painter(hueMask.get());
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width / 2.0, height / 2.0);

    qreal radius = qMin(width, height) / 2.0;
    qreal innerRadius = radius * (1.0 - HUE_WHEEL_THICKNESS);

    QPainterPath path;
    path.addEllipse(-radius, -radius, radius * 2.0, radius * 2.0);
    path.addEllipse(-innerRadius, -innerRadius, innerRadius * 2.0, innerRadius * 2.0);

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.drawPath(path);
}

void ColorWheel::renderHueWheelBox()
{
    int width = hueBox->width(), height = hueBox->height();

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            qreal hue = angleNorm(x - width / 2.0, -(y - height / 2.0));

            QColor color;
            color.setHslF(hue, saturation() / 100.0, lightness() / 100.0);

            QRgb rgb = qRgb(color.red(), color.green(), color.blue());
            hueBox->setPixel(x, y, rgb);
        }
    }
}

void ColorWheel::resizeEvent(QResizeEvent *event)
{
    int size = qMin(event->size().width(), event->size().height());

    hueWheel.reset(new QImage(size, size, QImage::Format_ARGB32));
    hueMask.reset(new QImage(size, size, QImage::Format_ARGB32));
    hueMask->fill(Qt::transparent);
    renderHueWheelMask();
}

void ColorWheel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2.0, height() / 2.0);

    // Draw color preview.
    int size = qMin(width(), height());
    qreal radius = size / 2.0;

    qreal previewRadius = radius * COLOR_PREVIEW_RADIUS;
    QRect arcRect(
        -previewRadius, -previewRadius,
        previewRadius * 2.0, previewRadius * 2.0
    );

    painter.setPen(Qt::NoPen);
    painter.setBrush(activeColor());
    painter.drawChord(arcRect, 0, 180 * 16);

    painter.setPen(Qt::NoPen);
    painter.setBrush(previousColor());
    painter.drawChord(arcRect, 180 * 16, 180 * 16);

    // Draw hue wheel.
    hueWheel->fill(Qt::transparent);
    QPainter wheelPainter(hueWheel.get());
    wheelPainter.drawImage(0, 0, *hueMask);
    wheelPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    renderHueWheelBox();
    wheelPainter.drawImage(0, 0, hueBox->scaled(
        size, size,
        Qt::IgnoreAspectRatio, Qt::SmoothTransformation
    ));

    painter.drawImage(-radius, -radius, *hueWheel);

    // Draw hue pointer.
    qreal pradius = huePointerRadius();
    painter.setPen(QColor(0, 0, 0, 255));
    painter.setBrush(QColor(255, 255, 255, 100));
    painter.drawEllipse(huePointerPosition(), pradius, pradius);
}

void ColorWheel::mousePressEvent(QMouseEvent *event)
{
    QVector2D mousePointer(event->pos());
    mousePointer -= QVector2D(width(), height()) / 2.0;
    qreal distance = mousePointer.length();
    qreal outerRadius = qMin(width(), height()) / 2.0;
    qreal innerRadius = outerRadius * (1.0 - HUE_WHEEL_THICKNESS);

    if (innerRadius <= distance && distance <= outerRadius) {
        setHue(qRound(360.0 * angleNorm(mousePointer.x(), -mousePointer.y())));
        huePointerDragged = true;
    }
}

void ColorWheel::mouseMoveEvent(QMouseEvent *event)
{
    if (!huePointerDragged)
        return;

    qreal x = event->pos().x() - width() / 2.0;
    qreal y = event->pos().y() - height() / 2.0;
    setHue(qRound(360.0 * angleNorm(x, -y)));
}

void ColorWheel::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    huePointerDragged = false;
}

void ColorWheel::wheelEvent(QWheelEvent *event)
{
    int newHue = hue() + event->delta() / 8 / 15;
    if (newHue > 360)
        newHue %= 360;
    if (newHue < 0)
        newHue += 360;
    setHue(newHue);

    event->accept();
}

QPointF ColorWheel::huePointerPosition() const
{
    qreal theta = 2.0 * 4.0 * qAtan2(1.0, 1.0) * hue() / 360.0;
    qreal radius = qMin(width(), height()) / 2.0;
    qreal distance = radius * (1.0 - HUE_WHEEL_THICKNESS / 2.0);
    return QPointF(distance * qCos(theta), distance * -qSin(theta));
}

qreal ColorWheel::huePointerRadius() const
{
    return qMin(width(), height()) * 0.5 * HUE_WHEEL_THICKNESS / 2.0 * 0.75;
}

/*!
 * \class ColorPicker
 * \inmodule view
 *
 * \brief A widget for choosing colors.
 */

/*!
 * \fn ColorPicker::activeColorChanged(const QColor &color)
 * \brief This signal is emmited whenever currently active color changes.
 */

/*!
 * \fn ColorPicker::previousColorChanged(const QColor &color)
 * \brief This signal is emmited whenever previously choosen color changes.
 */

/*!
 * \brief Constructs the picker with given \a parent widget.
 */
ColorPicker::ColorPicker(QWidget *parent) :
    QWidget(parent),
    wheel(new ColorWheel(this)),
    hueSlider(new GroupedSlider("Hue", this)),
    saturationSlider(new GroupedSlider("Saturation", this)),
    lightnessSlider(new GroupedSlider("Lightness", this)),
    hexEdit(new QLineEdit(this))
{
    hueSlider->setMinimum(0);
    hueSlider->setMaximum(360);

    saturationSlider->setMinimum(0);
    saturationSlider->setMaximum(100);

    lightnessSlider->setMinimum(0);
    lightnessSlider->setMaximum(100);

    QFont hexFont("Monospace");
    hexFont.setStyleHint(QFont::TypeWriter);
    hexEdit->setFont(hexFont);
    hexEdit->setMaximumWidth(75);
    hexEdit->setAlignment(Qt::AlignRight);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(wheel);
    layout->addWidget(hueSlider);
    layout->addWidget(saturationSlider);
    layout->addWidget(lightnessSlider);
    layout->addWidget(hexEdit, 0, Qt::AlignRight);
    setLayout(layout);

    connect(hueSlider, SIGNAL(valueChanged(int)), wheel, SLOT(setHue(int)));
    connect(saturationSlider, SIGNAL(valueChanged(int)), wheel, SLOT(setSaturation(int)));
    connect(lightnessSlider, SIGNAL(valueChanged(int)), wheel, SLOT(setLightness(int)));
    connect(wheel, SIGNAL(hueChanged(int)), hueSlider, SLOT(setValue(int)));
    connect(wheel, SIGNAL(saturationChanged(int)), saturationSlider, SLOT(setValue(int)));
    connect(wheel, SIGNAL(lightnessChanged(int)), lightnessSlider, SLOT(setValue(int)));
    connect(wheel, SIGNAL(activeColorChanged(QColor)), this, SLOT(setColor(QColor)));
    connect(wheel, SIGNAL(activeColorChanged(QColor)), this, SIGNAL(colorChanged(QColor)));
    connect(hexEdit, SIGNAL(returnPressed()), this, SLOT(setNamedColor()));
}

/*!
 * \property ColorPicker::color
 * \brief Currently active color.
 */
QColor ColorPicker::color() const
{
    return wheel->activeColor();
}

void ColorPicker::setColor(const QColor &color)
{
    wheel->setActiveColor(color.toHsl());
    hexEdit->setText(wheel->activeColor().name());
}

/*!
 * \brief Sets previously choosen color to currently active color.
 */
void ColorPicker::applyColor()
{
    wheel->setPreviousColor(color());
}

/*!
 * \brief Sets currently active color to previously choosen color.
 */
void ColorPicker::revertColor()
{
    setColor(wheel->previousColor());
}

/*!
 * \brief Sets active color basing on the hex line edit.
 */
void ColorPicker::setNamedColor()
{
    QColor color;
    color.setNamedColor(hexEdit->text());
    if (!color.isValid())
        return;

    setColor(color);
}

#include "colorpicker.moc"
#include "moc_colorpicker.cpp"
