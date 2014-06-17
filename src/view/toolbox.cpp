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
    selectedRect(Selected::Primary),
    m_primaryColor(Qt::black),
    m_secondaryColor(Qt::white)
{
    QSizePolicy sizePolicy;
    sizePolicy.setHorizontalPolicy(QSizePolicy::Preferred);
    sizePolicy.setVerticalPolicy(QSizePolicy::Fixed);
    setSizePolicy(sizePolicy);
}

void ColorPreview::mousePressEvent(QMouseEvent *event)
{
    switch (selectedRect) {
    case Selected::Primary:
        if (primaryPreviewRect().contains(event->pos()))
            event->accept();
        else if (secondaryPreviewRect().contains(event->pos())) {
            event->accept();
            selectedRect = Selected::Secondary;
            update();
            emit activeColorChanged(secondaryColor());
        }
        return;
    case Selected::Secondary:
        if (secondaryPreviewRect().contains(event->pos()))
            event->accept();
        else if (primaryPreviewRect().contains(event->pos())) {
            event->accept();
            selectedRect = Selected::Primary;
            update();
            emit activeColorChanged(primaryColor());
        }
        return;
    }

    QWidget::mousePressEvent(event);
}

void ColorPreview::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);

    QColor foregroundColor, backgroundColor;
    QRect foregroundRect, backgroundRect;

    switch (selectedRect) {
    case Selected::Primary:
        foregroundColor = primaryColor();
        foregroundRect = primaryPreviewRect();
        backgroundColor = secondaryColor();
        backgroundRect = secondaryPreviewRect();
        break;
    case Selected::Secondary:
        foregroundColor = secondaryColor();
        foregroundRect = secondaryPreviewRect();
        backgroundColor = primaryColor();
        backgroundRect = primaryPreviewRect();
        break;
    }

    // Draw background rectangle.
    painter.setPen(Qt::gray);
    painter.setBrush(backgroundColor);
    painter.drawRect(backgroundRect);

    // Draw foreground rectangle.
    painter.setPen(Qt::yellow);
    painter.setBrush(foregroundColor);
    painter.drawRect(foregroundRect);
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
    QWidget(parent)
{
    Tool *dragTool = new DragTool(this);
    Tool *lineTool = new LineTool(this);
    Tool *rectTool = new RectangleTool(this);
    Tool *ellipseTool = new EllipseTool(this);
    Tool *pencilTool = new PencilTool(this);
    Tool *rubberTool = new RubberTool(this);

    QGroupBox *toolBox = new QGroupBox("Tools", this);
    QMargins toolMargins = toolBox->contentsMargins();
    toolMargins.setLeft(-10);
    toolMargins.setRight(-10);
    toolBox->setContentsMargins(toolMargins);

    ToolboxButton *dragButton = new ToolboxButton(dragTool, "Drag view", toolBox);
    ToolboxButton *lineButton = new ToolboxButton(lineTool, "Line tool", toolBox);
    ToolboxButton *rectButton = new ToolboxButton(rectTool, "Rectangle tool", toolBox);
    ToolboxButton *ellipseButton = new ToolboxButton(ellipseTool, "Ellipse tool", toolBox);
    ToolboxButton *pencilButton = new ToolboxButton(pencilTool, "Pencil tool", toolBox);
    ToolboxButton *rubberButton = new ToolboxButton(rubberTool, "Rubber tool", toolBox);

    dragButton->setIcon(QIcon(":/icons/tools/drag.png"));
    lineButton->setIcon(QIcon(":/icons/tools/line.png"));
    rectButton->setIcon(QIcon(":/icons/tools/rect.png"));
    ellipseButton->setIcon(QIcon(":/icons/tools/ellipse.png"));
    pencilButton->setIcon(QIcon(":/icons/tools/pencil.png"));

    QButtonGroup *toolButtons = new QButtonGroup(this);
    toolButtons->addButton(dragButton);
    toolButtons->addButton(lineButton);
    toolButtons->addButton(rectButton);
    toolButtons->addButton(ellipseButton);
    toolButtons->addButton(pencilButton);
    toolButtons->addButton(rubberButton);

    QGridLayout *toolLayout = new QGridLayout(toolBox);
    toolLayout->addWidget(dragButton, 0, 0);
    toolLayout->addWidget(lineButton, 0, 1);
    toolLayout->addWidget(rectButton, 1, 0);
    toolLayout->addWidget(ellipseButton, 1, 1);
    toolLayout->addWidget(pencilButton, 2, 0);
    toolLayout->addWidget(rubberButton, 2, 1);
    toolBox->setLayout(toolLayout);

    penPicker = new PenPicker(this);
    colorPreview = new ColorPreview(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(toolBox);
    layout->addWidget(penPicker);
    layout->addWidget(colorPreview);
    layout->addStretch();
    this->setLayout(layout);

    connect(
        penPicker, SIGNAL(penColorChanged(QColor)),
        colorPreview, SLOT(setActiveColor(QColor))
    );
    connect(
        colorPreview, SIGNAL(activeColorChanged(QColor)),
        penPicker, SLOT(setPenColor(QColor))
    );

    connect(
        penPicker, SIGNAL(penSizeChanged(int)),
        this, SIGNAL(penSizeChanged(int))
    );
    connect(
        colorPreview, SIGNAL(activeColorChanged(QColor)),
        this, SIGNAL(activeColorChanged(QColor))
    );

    connect(
        toolButtons, SIGNAL(buttonClicked(QAbstractButton*)),
        this, SLOT(onToolToggled(QAbstractButton*))
    );
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
