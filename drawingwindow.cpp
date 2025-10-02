#include "drawingwindow.h"
#include "drawingcanvas.h"

DrawingWindow::DrawingWindow(QWidget *parent)
    : QMainWindow{parent}
{
    setWindowTitle("Computer Graphics Simulation");
    resize(600, 400);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    QBoxLayout *hlayout = new QBoxLayout(QBoxLayout::LeftToRight, this);

    DrawingCanvas *canvas = new DrawingCanvas(this);
    layout->addWidget(canvas, 1);

    // Clear Button
    QPushButton* runSimulationButton = new QPushButton("Fix Convex Hull", this);

    QPushButton* clearButton = new QPushButton("Clear Canvas", this);
    layout->addLayout(hlayout, 1);
    hlayout->addWidget(runSimulationButton, 0);
    hlayout->addWidget(clearButton, 0);
    // Stretch factor 0 to only take up required space

    // Connect button
    connect(runSimulationButton, &QPushButton::clicked, canvas, &DrawingCanvas::runConvexHull);
    connect(clearButton, &QPushButton::clicked, canvas, &DrawingCanvas::clearCanvas);
}
