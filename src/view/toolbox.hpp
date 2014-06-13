#ifndef VIEW_TOOLBOX_HPP
#define VIEW_TOOLBOX_HPP

#include <QObject>
#include <QWidget>
#include <QDockWidget>
#include <QGroupBox>
#include <QSpinBox>

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
        QColor foregroundColor
        READ foregroundColor WRITE setForegroundColor NOTIFY foregroundColorChanged
    )
    Q_PROPERTY(
        QColor backgroundColor
        READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged
    )

public:
    explicit ColorPreview(QWidget *parent = nullptr);

    const QColor &foregroundColor() const;
    const QColor &backgroundColor() const;

    QSize sizeHint() const;

public slots:
    void setForegroundColor(const QColor &color);
    void setBackgroundColor(const QColor &color);

signals:
    void foregroundColorChanged(const QColor &color);
    void backgroundColorChanged(const QColor &color);

    void foregroundRectClicked();
    void foregroundRectPressed();
    void foregroundRectReleased();

    void backgroundRectClicked();
    void backgroundRectPressed();
    void backgroundRectReleased();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    static const qreal PREVIEW_RECT_RATIO;
    QRect foregroundPreviewRect() const;
    QRect backgroundPreviewRect() const;

    bool foregroundClickInitiated;
    bool backgroundClickInitiated;

    QColor m_foregroundColor;
    QColor m_backgroundColor;
};

inline const QColor &ColorPreview::foregroundColor() const
{
    return m_foregroundColor;
}

inline const QColor &ColorPreview::backgroundColor() const
{
    return m_backgroundColor;
}

inline void ColorPreview::setForegroundColor(const QColor &color)
{
    if (m_foregroundColor == color)
        return;

    m_foregroundColor = color;
    update();
    emit foregroundColorChanged(color);
}

inline void ColorPreview::setBackgroundColor(const QColor &color)
{
    if (m_backgroundColor == color)
        return;

    m_backgroundColor = color;
    update();
    emit backgroundColorChanged(color);
}

inline QSize ColorPreview::sizeHint() const
{
    return QSize(75, 75);
}

inline QRect ColorPreview::foregroundPreviewRect() const
{
    int rw = qRound(width() * PREVIEW_RECT_RATIO);
    int rh = qRound(height() * PREVIEW_RECT_RATIO);
    return QRect(0, 0, rw, rh);
}

inline QRect ColorPreview::backgroundPreviewRect() const
{
    int rw = qRound(width() * PREVIEW_RECT_RATIO);
    int rh = qRound(height() * PREVIEW_RECT_RATIO);
    return QRect(width() - rw, height() - rh, rw, rh);
}

class Toolbox : public QDockWidget
{
    Q_OBJECT
    Q_PROPERTY(
        int penSize
        READ penSize WRITE setPenSize NOTIFY penSizeChanged
    )
    Q_PROPERTY(
        QColor foregroundColor
        READ foregroundColor WRITE setForegroundColor NOTIFY foregroundColorChanged
    )
    Q_PROPERTY(
        QColor backgroundColor
        READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged
    )

public:
    explicit Toolbox(QWidget *parent = nullptr);

    int penSize() const;
    const QColor &foregroundColor() const;
    const QColor &backgroundColor() const;

public slots:
    void setPenSize(int size);
    void setForegroundColor(const QColor &color);
    void setBackgroundColor(const QColor &color);

signals:
    int penSizeChanged(int size);
    void foregroundColorChanged(const QColor &color);
    void backgroundColorChanged(const QColor &color);

    void foregroundRectClicked();
    void foregroundRectPressed();
    void foregroundRectReleased();

    void backgroundRectClicked();
    void backgroundRectPressed();
    void backgroundRectReleased();

private:
    PenPicker *penPicker;
    ColorPreview *colorPreview;
};

inline int Toolbox::penSize() const
{
    return penPicker->penSize();
}

inline const QColor &Toolbox::foregroundColor() const
{
    return colorPreview->foregroundColor();
}

inline const QColor &Toolbox::backgroundColor() const
{
    return colorPreview->backgroundColor();
}

inline void Toolbox::setPenSize(int size)
{
    penPicker->setPenSize(size);
}

inline void Toolbox::setForegroundColor(const QColor &color)
{
    colorPreview->setForegroundColor(color);
}

inline void Toolbox::setBackgroundColor(const QColor &color)
{
    colorPreview->setBackgroundColor(color);
}

#endif // VIEW_TOOLBOX_HPP
