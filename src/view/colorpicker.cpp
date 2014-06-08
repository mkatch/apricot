#include "colorpicker.hpp"

#include <QPaintEvent>
#include <QResizeEvent>
#include <QPainter>
#include <QImage>
#include <QPixmap>
#include <QBitmap>
#include <QtMath>
#include <QVector2D>

ValueSlider::ValueSlider(
    const QString &title,
    int minimum, int maximum,
    QWidget *parent
) :
    QGroupBox(title, parent)
{
    slider = new QSlider(Qt::Horizontal, this);
    slider->setMinimum(minimum);
    slider->setMaximum(maximum);

    spinBox = new QSpinBox(this);
    spinBox->setMinimum(minimum);
    spinBox->setMaximum(maximum);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(slider);
    layout->addWidget(spinBox);
    setLayout(layout);
    setContentsMargins(-10, 5, -10, -10);

    connect(slider, SIGNAL(valueChanged(int)), spinBox, SLOT(setValue(int)));
    connect(spinBox, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(setValue(int)));
}

int ValueSlider::value() const
{
    return slider->value();
}

void ValueSlider::setValue(int value)
{
    slider->setValue(value);
    emit valueChanged(value);
}

const qreal ColorWheel::HUE_WHEEL_THICKNESS = 0.2;
const qreal ColorWheel::COLOR_PREVIEW_RADIUS = 0.65;

ColorWheel::ColorWheel(QWidget *parent) :
    QWidget(parent), hueWheel(new QImage()),
    m_hue(0), m_saturation(0), m_lightness(0),
    m_previousColor(Qt::black)
{
    QSizePolicy policy;
    policy.setHorizontalPolicy(QSizePolicy::MinimumExpanding);
    policy.setVerticalPolicy(QSizePolicy::MinimumExpanding);
    setSizePolicy(policy);
}

int ColorWheel::hue() const
{
    return m_hue;
}

int ColorWheel::saturation() const
{
    return m_saturation;
}

int ColorWheel::lightness() const
{
    return m_lightness;
}

QColor ColorWheel::activeColor() const
{
    QColor color;
    color.setHslF(hue() / 360.0, saturation() / 100.0, lightness() / 100.0);
    return color;
}

QColor ColorWheel::previousColor() const
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

void ColorWheel::renderHueWheelMask(QImage &image)
{
    int width = image.width(), height = image.height();

    QPainter painter(&image);
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

void ColorWheel::renderHueWheelBox(QImage &image)
{
    int width = image.width(), height = image.height();

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            qreal hue = angleNorm(x - width / 2.0, -(y - height / 2.0));

            QColor color;
            color.setHslF(hue, 1.0, 0.5);

            QRgb rgb = qRgb(color.red(), color.green(), color.blue());
            image.setPixel(x, y, rgb);
        }
    }
}

void ColorWheel::resizeEvent(QResizeEvent *event)
{
    int size = qMin(event->size().width(), event->size().height());

    hueWheel.reset(new QImage(size, size, QImage::Format_ARGB32));
    hueWheel->fill(Qt::transparent);
    QPainter painter(hueWheel.get());

    QImage proxy(size, size, QImage::Format_ARGB32);

    // Generate and draw new hue wheel alpha mask.
    proxy.fill(Qt::transparent);
    renderHueWheelMask(proxy);
    painter.drawImage(0, 0, proxy);

    // Generate and draw new hue box.
    proxy.fill(Qt::transparent);
    renderHueWheelBox(proxy);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.drawImage(0, 0, proxy);
}

void ColorWheel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2.0, height() / 2.0);

    // Draw color preview.
    qreal radius = qMin(width(), height()) / 2.0;

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
    painter.drawImage(-radius, -radius, *hueWheel);

    // Draw hue pointer.
    qreal size = huePointerRadius();
    painter.setPen(QColor(0, 0, 0, 255));
    painter.setBrush(QColor(255, 255, 255, 100));
    painter.drawEllipse(huePointerPosition(), size, size);
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

ColorPicker::ColorPicker(QWidget *parent) :
    QWidget(parent),
    wheel(new ColorWheel(this)),
    hueSlider(new ValueSlider("Hue", 0, 360, this)),
    saturationSlider(new ValueSlider("Saturation", 0, 100, this)),
    lightnessSlider(new ValueSlider("Lightness", 0, 100, this)),
    hexEdit(new QLineEdit(this))
{
    hexEdit->setMaximumWidth(75);
    hexEdit->setAlignment(Qt::AlignRight);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(wheel);
    layout->addWidget(hueSlider);
    layout->addWidget(saturationSlider);
    layout->addWidget(lightnessSlider);
    layout->addWidget(hexEdit, 0, Qt::AlignRight);
    setLayout(layout);

    connect(
        hueSlider, SIGNAL(valueChanged(int)),
        wheel, SLOT(setHue(int))
    );
    connect(
        saturationSlider, SIGNAL(valueChanged(int)),
        wheel, SLOT(setSaturation(int))
    );
    connect(
        lightnessSlider, SIGNAL(valueChanged(int)),
        wheel, SLOT(setLightness(int))
    );
    connect(
        wheel, SIGNAL(hueChanged(int)),
        hueSlider, SLOT(setValue(int))
    );
    connect(
        wheel, SIGNAL(saturationChanged(int)),
        saturationSlider, SLOT(setValue(int))
    );
    connect(
        wheel, SIGNAL(lightnessChanged(int)),
        lightnessSlider, SLOT(setValue(int))
    );
    connect(
        wheel, SIGNAL(activeColorChanged(QColor)),
        this, SLOT(setActiveColor(QColor))
    );
    connect(
        wheel, SIGNAL(activeColorChanged(QColor)),
        this, SIGNAL(activeColorChanged(QColor))
    );
    connect(
        wheel, SIGNAL(previousColorChanged(QColor)),
        this, SLOT(setPreviousColor(QColor))
    );
    connect(
        wheel, SIGNAL(previousColorChanged(QColor)),
        this, SIGNAL(previousColorChanged(QColor))
    );
    connect(
        hexEdit, SIGNAL(returnPressed()),
        this, SLOT(setNamedColor())
    );
}

QColor ColorPicker::activeColor() const
{
    return wheel->activeColor();
}

QColor ColorPicker::previousColor() const
{
    return wheel->previousColor();
}

void ColorPicker::setActiveColor(const QColor &color)
{
    wheel->setActiveColor(color.toHsl());
    hexEdit->setText(wheel->activeColor().name());
}

void ColorPicker::setPreviousColor(const QColor &color)
{
    wheel->setPreviousColor(color.toHsl());
}

void ColorPicker::applyColor()
{
    setPreviousColor(activeColor());
}

void ColorPicker::revertColor()
{
    setActiveColor(previousColor());
}

void ColorPicker::setNamedColor()
{
    QColor color;
    color.setNamedColor(hexEdit->text());
    if (!color.isValid())
        return;

    setActiveColor(color);
}
