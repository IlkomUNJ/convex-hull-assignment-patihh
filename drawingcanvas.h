#ifndef DRAWINGCANVAS_H
#define DRAWINGCANVAS_H

#include <QWidget>
#include <QMouseEvent>

class DrawingCanvas : public QWidget
{
    Q_OBJECT
public:
    explicit DrawingCanvas(QWidget *parent = nullptr);

public slots:
    void runConvexHull();
    void clearCanvas();

protected:
    void extracted(QPainter &painter);
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QVector<QPoint> m_points;
    QVector<QPoint> m_slowHull;
    QVector<QPoint> m_fastHull;
    QVector<QPoint> slowConvexHull(const QVector<QPoint> &points, int &iterations);
    QVector<QPoint> fastConvexHull(const QVector<QPoint> &points, int &iterations);
    int m_slowIterations = 0;
    int m_fastIterations = 0;
};

#endif // DRAWINGCANVAS_H
