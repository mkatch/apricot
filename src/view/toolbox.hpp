#ifndef VIEW_TOOLBOX_HPP
#define VIEW_TOOLBOX_HPP

#include <QObject>
#include <QWidget>
#include <QDockWidget>
#include <QGroupBox>
#include <QSpinBox>
#include <QAbstractButton>

class PenPreview : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int penSize READ penSize WRITE setPenSize NOTIFY penSizeChanged)
    Q_PROPERTY(QColor penColor READ penColor WRITE setPenColor NOTIFY penColorChanged)

public:
    explicit PenPreview(QWidget *parent = nullptr);

    int penSize() const;
    const QColor &penColor() const;

    static const int MINIMUM_PEN_SIZE;
    static const int MAXIMUM_PEN_SIZE;

public slots:
    void setPenSize(int size);
    void setPenColor(const QColor &color);

signals:
    void penSizeChanged(int size);
    void penColorChanged(const QColor &color);

protected:
    void paintEvent(QPaintEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    int m_penSize;
    QColor m_penColor;
};

inline int PenPreview::penSize() const
{
    return m_penSize;
}

inline const QColor &PenPreview::penColor() const
{
    return m_penColor;
}

inline void PenPreview::setPenSize(int size)
{
    if (size < MINIMUM_PEN_SIZE)
        size = MINIMUM_PEN_SIZE;
    if (size > MAXIMUM_PEN_SIZE)
        size = MAXIMUM_PEN_SIZE;

    if (m_penSize == size)
        return;

    m_penSize = size;
    update();
    emit penSizeChanged(size);
}

inline void PenPreview::setPenColor(const QColor &color)
{
    if (m_penColor == color)
        return;

    m_penColor = color;
    update();
    emit penColorChanged(color);
}

class PenPicker : public QGroupBox
{
    Q_OBJECT
    Q_PROPERTY(int penSize READ penSize WRITE setPenSize NOTIFY penSizeChanged)
    Q_PROPERTY(QColor penColor READ penColor WRITE setPenColor NOTIFY penColorChanged)

public:
    explicit PenPicker(QWidget *parent = nullptr);

    int penSize() const;
    const QColor &penColor() const;

    QSize sizeHint() const;

public slots:
    void setPenSize(int size);
    void setPenColor(const QColor &color);

signals:
    void penSizeChanged(int size);
    void penColorChanged(const QColor &color);

private:
    PenPreview *preview;
    QSpinBox *spinBox;
};

inline int PenPicker::penSize() const
{
    return preview->penSize();
}

inline const QColor &PenPicker::penColor() const
{
    return preview->penColor();
}

inline void PenPicker::setPenSize(int size)
{
    preview->setPenSize(size);
}

inline void PenPicker::setPenColor(const QColor &color)
{
    preview->setPenColor(color);
}

inline QSize PenPicker::sizeHint() const
{
    return QSize(100, 50);
}

class ColorPreview : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(
        QColor activeColor
        READ activeColor WRITE setActiveColor NOTIFY activeColorChanged
    )

public:
    explicit ColorPreview(QWidget *parent = nullptr);

    const QColor &activeColor() const;
    const QColor &primaryColor() const;
    const QColor &secondaryColor() const;

    QSize sizeHint() const;

public slots:
    void setActiveColor(const QColor &color);

signals:
    void activeColorChanged(const QColor &color);

protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    static const qreal PREVIEW_RECT_RATIO;
    QRect primaryPreviewRect() const;
    QRect secondaryPreviewRect() const;

    enum class Selected {
        Primary,
        Secondary
    } selectedRect;
    QColor &selectedColor();

    QColor m_primaryColor;
    QColor m_secondaryColor;
};

inline const QColor &ColorPreview::activeColor() const
{
    switch (selectedRect) {
    case Selected::Primary:
        return m_primaryColor;
    case Selected::Secondary:
        return m_secondaryColor;
    }
}

inline const QColor &ColorPreview::primaryColor() const
{
    return m_primaryColor;
}

inline const QColor &ColorPreview::secondaryColor() const
{
    return m_secondaryColor;
}

inline QColor &ColorPreview::selectedColor()
{
    switch (selectedRect) {
    case Selected::Primary:
        return m_primaryColor;
    case Selected::Secondary:
        return m_secondaryColor;
    }
}

inline void ColorPreview::setActiveColor(const QColor &color)
{
    if (selectedColor() == color)
        return;

    selectedColor() = color;
    update();
    emit activeColorChanged(color);
}

inline QSize ColorPreview::sizeHint() const
{
    return QSize(75, 75);
}

inline QRect ColorPreview::primaryPreviewRect() const
{
    int rw = qRound(width() * PREVIEW_RECT_RATIO);
    int rh = qRound(height() * PREVIEW_RECT_RATIO);
    return QRect(0, 0, rw, rh);
}

inline QRect ColorPreview::secondaryPreviewRect() const
{
    int rw = qRound(width() * PREVIEW_RECT_RATIO);
    int rh = qRound(height() * PREVIEW_RECT_RATIO);
    return QRect(width() - rw, height() - rh, rw, rh);
}

class Tool;

class Toolbox : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(
        int penSize
        READ penSize WRITE setPenSize NOTIFY penSizeChanged
    )
    Q_PROPERTY(
        QColor activeColor
        READ activeColor WRITE setActiveColor NOTIFY activeColorChanged
    )
    Q_PROPERTY(QColor primaryColor READ primaryColor)
    Q_PROPERTY(QColor secondaryColor READ secondaryColor)

public:
    explicit Toolbox(QWidget *parent = nullptr);

    int penSize() const;
    const QColor &activeColor() const;
    const QColor &primaryColor() const;
    const QColor &secondaryColor() const;
    Tool *activeTool();

public slots:
    void setPenSize(int size);
    void setActiveColor(const QColor &color);

signals:
    void penSizeChanged(int size);
    void activeColorChanged(const QColor &color);
    void primaryColorChanged(const QColor &color);
    void secondaryColorChanged(const QColor &color);
    void activeToolChanged(Tool *tool);

private:
    PenPicker *penPicker;
    ColorPreview *colorPreview;

    Tool *m_activeTool;

private slots:
    void onToolToggled(QAbstractButton *button);
};

inline int Toolbox::penSize() const
{
    return penPicker->penSize();
}

inline const QColor &Toolbox::activeColor() const
{
    return colorPreview->activeColor();
}

inline const QColor &Toolbox::primaryColor() const
{
    return colorPreview->primaryColor();
}

inline const QColor &Toolbox::secondaryColor() const
{
    return colorPreview->secondaryColor();
}

inline Tool *Toolbox::activeTool()
{
    return m_activeTool;
}

inline void Toolbox::setPenSize(int size)
{
    penPicker->setPenSize(size);
}

inline void Toolbox::setActiveColor(const QColor &color)
{
    colorPreview->setActiveColor(color);
}

#endif // VIEW_TOOLBOX_HPP
