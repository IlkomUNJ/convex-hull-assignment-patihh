#include "drawingcanvas.h"
#include <QPainter>

DrawingCanvas::DrawingCanvas(QWidget *parent)
    : QWidget{parent}
{}

void DrawingCanvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event); // The 'event' parameter is not used in this simple example, so we mark it to avoid a compiler warning.

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    /*
    Draw line
    painter.drawLine(0, 0, 300, 200);
    painter.drawLine(300, 200, 0, 399);

    painter.drawLine( 300, 300, 300, 300);
    painter.drawLine(QPoint(250,250));
    */

    // gambar titik
    QPen pointPen(Qt::red, 6);
    painter.setPen(pointPen);
    for (const QPoint &p : std::as_const(m_points)) {
        painter.drawPoint(p);
    }

    // gambar slow hull
    if (m_slowHull.size() > 1) {
        QPen slowPen(Qt::blue, 2);
        painter.setPen(slowPen);
        painter.drawPolygon(m_slowHull.data(), m_slowHull.size());
    }

    // gambar fast hull
    if (m_fastHull.size() > 1) {
        QPen fastPen(Qt::green, 2);
        painter.setPen(fastPen);
        painter.drawPolygon(m_fastHull.data(), m_fastHull.size());
    }

    // tulis jumlah iterasi
    painter.setPen(Qt::black);
    QString text = QString("Slow: %1 iterations | Fast: %2 iterations")
                       .arg(m_slowIterations)
                       .arg(m_fastIterations);
    painter.drawText(10, 20, text);
}

// Start new drawing path
void DrawingCanvas::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        //m_points.clear();
        m_points.append(event->pos());
        update();
    }
}

// Add points to path
void DrawingCanvas::mouseMoveEvent(QMouseEvent* event) {
    if (event->buttons() & Qt::LeftButton) {
        m_points.append(event->pos());
        // Request repaint widget
        update();
    }
}

void DrawingCanvas::runConvexHull() {
    if (m_points.size() < 3) return;

    m_slowIterations = 0;
    m_fastIterations = 0;

    m_slowHull = slowConvexHull(m_points, m_slowIterations);
    m_fastHull = fastConvexHull(m_points, m_fastIterations);

    update(); // gambar ulang
}

QVector<QPoint> DrawingCanvas::slowConvexHull(const QVector<QPoint> &points, int &iterations) {
    QVector<QPoint> hull;
    iterations = 0;

    int n = points.size();
    if (n < 3) return hull;

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int a = points[j].y() - points[i].y();
            int b = points[i].x() - points[j].x();
            int c = a * points[i].x() + b * points[i].y();

            int pos = 0, neg = 0;
            for (int k = 0; k < n; ++k) {
                if (k == i || k == j) continue;
                int val = a * points[k].x() + b * points[k].y() - c;
                if (val > 0) pos++;
                if (val < 0) neg++;
                iterations++;
            }

            if (pos == 0 || neg == 0) {
                if (!hull.contains(points[i])) hull.append(points[i]);
                if (!hull.contains(points[j])) hull.append(points[j]);
            }
        }
    }

    return hull;
}

QVector<QPoint> DrawingCanvas::fastConvexHull(const QVector<QPoint> &points, int &iterations) {
    iterations = 0;
    QVector<QPoint> pts = points;
    QVector<QPoint> hull;

    if (pts.size() < 3) return hull;

    // cari titik dengan y minimum
    int ymin = pts[0].y(), minIdx = 0;
    for (int i = 1; i < pts.size(); i++) {
        if ((pts[i].y() < ymin) ||
            (pts[i].y() == ymin && pts[i].x() < pts[minIdx].x())) {
            ymin = pts[i].y();
            minIdx = i;
        }
    }
    std::swap(pts[0], pts[minIdx]);
    QPoint p0 = pts[0];

    // sort berdasarkan sudut polar
    std::sort(pts.begin() + 1, pts.end(), [&](const QPoint &a, const QPoint &b) {
        long long cross = (long long)(a.x()-p0.x())*(b.y()-p0.y()) -
                          (long long)(a.y()-p0.y())*(b.x()-p0.x());
        iterations++;
        if (cross == 0) {
            long long da = (long long)(a.x()-p0.x())*(a.x()-p0.x()) + (long long)(a.y()-p0.y())*(a.y()-p0.y());
            long long db = (long long)(b.x()-p0.x())*(b.x()-p0.x()) + (long long)(b.y()-p0.y())*(b.y()-p0.y());
            return da < db;
        }
        return cross > 0;
    });

    // buat hull pakai stack
    hull.push_back(pts[0]);
    hull.push_back(pts[1]);
    hull.push_back(pts[2]);

    for (int i = 3; i < pts.size(); i++) {
        while (hull.size() > 1) {
            QPoint p1 = hull[hull.size()-2];
            QPoint p2 = hull[hull.size()-1];
            long long cross = (long long)(p2.x()-p1.x())*(pts[i].y()-p1.y()) -
                              (long long)(p2.y()-p1.y())*(pts[i].x()-p1.x());
            if (cross > 0) break;
            hull.pop_back();
            iterations++;
        }
        hull.push_back(pts[i]);
        iterations++;
    }

    return hull;
}

// Clear canvas and trigger repaint
void DrawingCanvas::clearCanvas() {
    m_points.clear();
    m_slowHull.clear();
    m_fastHull.clear();
    m_slowIterations = 0;
    m_fastIterations = 0;
    update();
}
