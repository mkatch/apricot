#include "toolbox.hpp"

#include <QBoxLayout>
#include <QSpinBox>
#include <QPushButton>
#include <QButtonGroup>
#include <QPainter>

#include <ApricotTools>

const int PenPreview::MINIMUM_PEN_SIZE = 1;
const int PenPreview::MAXIMUM_PEN_SIZE = 10;

PenPreview::PenPreview(QWidget *parent) :
    QWidget(parent), m_penSize(MINIMUM_PEN_SIZE)
{
    // Do nothing.
}

void PenPreview::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(penColor());

    int size = qMin(width(), height());
    qreal r = (penSize() * size / 2.0) / MAXIMUM_PEN_SIZE;
    painter.drawEllipse(QPointF(width() / 2.0, height() / 2.0), r, r);
}

void PenPreview::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;

    setPenSize(penSize() + numSteps);
    event->accept();
}

PenPicker::PenPicker(QWidget *parent) :
    QGroupBox("Pen size", parent),
    preview(new PenPreview(this)),
    spinBox(new QSpinBox(this))
{
    spinBox->setMinimum(PenPreview::MINIMUM_PEN_SIZE);
    spinBox->setMaximum(PenPreview::MAXIMUM_PEN_SIZE);
    spinBox->setValue(PenPreview::MINIMUM_PEN_SIZE);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(preview);
    layout->addWidget(spinBox);
    setLayout(layout);

    QMargins margins = contentsMargins();
    margins.setLeft(-10);
    margins.setRight(-10);
    setContentsMargins(margins);

    QSizePolicy sizePolicy;
    sizePolicy.setHorizontalPolicy(QSizePolicy::Preferred);
    sizePolicy.setVerticalPolicy(QSizePolicy::Maximum);
    setSizePolicy(sizePolicy);

    connect(spinBox, SIGNAL(valueChanged(int)), preview, SLOT(setPenSize(int)));
    connect(preview, SIGNAL(penSizeChanged(int)), spinBox, SLOT(setValue(int)));

    connect(preview, SIGNAL(penSizeChanged(int)), this, SIGNAL(penSizeChanged(int)));
    connect(preview, SIGNAL(penColorChanged(QColor)), this, SIGNAL(penColorChanged(QColor)));
}

const qreal ColorPreview::PREVIEW_RECT_RATIO = 2.0 / 3.0;

ColorPreview::ColorPreview(QWidget *parent) :
    QWidget(parent),
    foregroundClickInitiated(false), backgroundClickInitiated(false)
{
    QSizePolicy sizePolicy;
    sizePolicy.setHorizontalPolicy(QSizePolicy::Preferred);
    sizePolicy.setVerticalPolicy(QSizePolicy::Fixed);
    setSizePolicy(sizePolicy);
}

void ColorPreview::mousePressEvent(QMouseEvent *event)
{
    if (foregroundPreviewRect().contains(event->pos())) {
        emit foregroundRectPressed();
        foregroundClickInitiated = true;
        event->accept();
        return;
    }

    if (backgroundPreviewRect().contains(event->pos())) {
        emit backgroundRectPressed();
        backgroundClickInitiated = true;
        event->accept();
        return;
    }

    QWidget::mousePressEvent(event);
}

void ColorPreview::mouseReleaseEvent(QMouseEvent *event)
{
    if (foregroundPreviewRect().contains(event->pos())) {
        emit foregroundRectReleased();
        if (foregroundClickInitiated) {
            foregroundClickInitiated = false;
            emit foregroundRectClicked();
        }
        event->accept();
        return;
    }

    if (backgroundPreviewRect().contains(event->pos())) {
        emit backgroundRectReleased();
        if (backgroundClickInitiated) {
            backgroundClickInitiated = false;
            emit backgroundRectClicked();
        }
        event->accept();
        return;
    }

    QWidget::mouseReleaseEvent(event);
}

void ColorPreview::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setPen(Qt::gray);

    // Draw background rectangle.
    painter.setBrush(backgroundColor());
    painter.drawRect(backgroundPreviewRect());

    // Draw foreground rectangle.
    painter.setBrush(foregroundColor());
    painter.drawRect(foregroundPreviewRect());
}

class ToolboxButton : public QPushButton
{
    Q_OBJECT

public:
    explicit ToolboxButton(Tool *tool, const QString &text, QWidget *parent = nullptr);

    Tool *tool();

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

private:
    Tool *m_tool;
};

inline Tool *ToolboxButton::tool()
{
    return m_tool;
}

ToolboxButton::ToolboxButton(Tool *tool, const QString &text, QWidget *parent) :
    QPushButton(parent), m_tool(tool)
{
    setCheckable(true);
    setFlat(true);
    setToolTip(text);
}

void ToolboxButton::enterEvent(QEvent *event)
{
    QPushButton::enterEvent(event);
    setFlat(false);
}

void ToolboxButton::leaveEvent(QEvent *event)
{
    QPushButton::leaveEvent(event);
    setFlat(true);
}

Toolbox::Toolbox(QWidget *parent) :
    QDockWidget(parent)
{
    QWidget *content = new QWidget(this);
    setWidget(content);

    Tool *dragTool = new DragTool(this);
    Tool *lineTool = new LineTool(this);
    Tool *rectTool = new RectangleTool(this);
    Tool *ellipseTool = new EllipseTool(this);
    Tool *pencilTool = new PencilTool(this);

    QGroupBox *toolBox = new QGroupBox("Tools", content);
    QMargins toolMargins = toolBox->contentsMargins();
    toolMargins.setLeft(-10);
    toolMargins.setRight(-10);
    toolBox->setContentsMargins(toolMargins);

    ToolboxButton *dragButton = new ToolboxButton(dragTool, "Drag view", toolBox);
    ToolboxButton *lineButton = new ToolboxButton(lineTool, "Line tool", toolBox);
    ToolboxButton *rectButton = new ToolboxButton(rectTool, "Rectangle tool", toolBox);
    ToolboxButton *ellipseButton = new ToolboxButton(ellipseTool, "Ellipse tool", toolBox);
    ToolboxButton *pencilButton = new ToolboxButton(pencilTool, "Pencil tool", toolBox);

    QButtonGroup *toolButtons = new QButtonGroup(content);
    toolButtons->addButton(dragButton);
    toolButtons->addButton(lineButton);
    toolButtons->addButton(rectButton);
    toolButtons->addButton(ellipseButton);
    toolButtons->addButton(pencilButton);

    QGridLayout *toolLayout = new QGridLayout(toolBox);
    toolLayout->addWidget(dragButton, 0, 0);
    toolLayout->addWidget(lineButton, 0, 1);
    toolLayout->addWidget(rectButton, 1, 0);
    toolLayout->addWidget(ellipseButton, 1, 1);
    toolLayout->addWidget(pencilButton, 2, 0);
    toolBox->setLayout(toolLayout);

    penPicker = new PenPicker(content);
    colorPreview = new ColorPreview(content);

    QVBoxLayout *layout = new QVBoxLayout(content);
    layout->addWidget(toolBox);
    layout->addWidget(penPicker);
    layout->addWidget(colorPreview);
    layout->addStretch();
    content->setLayout(layout);

    connect(penPicker, SIGNAL(penColorChanged(QColor)), colorPreview, SLOT(setForegroundColor(QColor)));
    connect(colorPreview, SIGNAL(foregroundColorChanged(QColor)), penPicker, SLOT(setPenColor(QColor)));

    connect(penPicker, SIGNAL(penSizeChanged(int)), this, SIGNAL(penSizeChanged(int)));
    connect(colorPreview, SIGNAL(foregroundColorChanged(QColor)), this, SIGNAL(foregroundColorChanged(QColor)));
    connect(colorPreview, SIGNAL(backgroundColorChanged(QColor)), this, SIGNAL(backgroundColorChanged(QColor)));

    connect(colorPreview, SIGNAL(foregroundRectClicked()), this, SIGNAL(foregroundRectClicked()));
    connect(colorPreview, SIGNAL(foregroundRectPressed()), this, SIGNAL(foregroundRectPressed()));
    connect(colorPreview, SIGNAL(foregroundRectReleased()), this, SIGNAL(foregroundRectReleased()));
    connect(colorPreview, SIGNAL(backgroundRectClicked()), this, SIGNAL(backgroundRectClicked()));
    connect(colorPreview, SIGNAL(backgroundRectPressed()), this, SIGNAL(backgroundRectPressed()));
    connect(colorPreview, SIGNAL(backgroundRectReleased()), this, SIGNAL(backgroundRectReleased()));

    connect(toolButtons, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(onToolToggled(QAbstractButton*)));
    dragButton->toggle();
    m_activeTool = dragTool;
}

void Toolbox::onToolToggled(QAbstractButton *button)
{
    ToolboxButton *toolButton = static_cast<ToolboxButton *>(button);

    if (toolButton->tool() == m_activeTool)
        return;

    m_activeTool = toolButton->tool();
    emit activeToolChanged(toolButton->tool());
}

#include "toolbox.moc"
#include "moc_toolbox.cpp"
