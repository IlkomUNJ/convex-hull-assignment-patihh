#ifndef DRAWINGWINDOW_H
#define DRAWINGWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>

class DrawingWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit DrawingWindow(QWidget *parent = nullptr);

signals:
};

#endif // DRAWINGWINDOW_H
