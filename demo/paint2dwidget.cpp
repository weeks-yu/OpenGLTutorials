#include <QtGui>

#include "paint2dwidget.h"

Paint2DWidget::Paint2DWidget(QWidget *parent)
    : QGLWidget(parent)
{
    setWindowTitle(tr("1. 2D"));
    setMinimumSize(200, 200);
    setMouseTracking(true);
    _scaleOfDrawing = 1;
    _centerOfDrawing = QPointF(0.5, 0.5);
}

Paint2DWidget::~Paint2DWidget()
{}

void Paint2DWidget::initializeGL()
{
    makeCurrent(); 
}

void Paint2DWidget::paintGL()
{
    // clear background
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static const int N = 10;
    float centerX = _centerOfDrawing.x();
    float centerY = _centerOfDrawing.y();

    // paint spiral triangles
    glBegin(GL_TRIANGLES);
    for(int i = 0; i <= N; i++)
    {
        double angle1 = 3.1415926 * 2.0 / N * i;
        double x1 = cos(angle1) * i / N * _scaleOfDrawing;
        double y1 = sin(angle1) * i / N * _scaleOfDrawing;

        double angle2 = 3.1415926 * 2.0 / N * (i+1);
        double x2 = cos(angle2) * i / N * _scaleOfDrawing;
        double y2 = sin(angle2) * i / N * _scaleOfDrawing;

        glColor3f(double(i) / N, double(i) / N, double(i) / N);
        glVertex2d(centerX, centerY);
        glVertex2d(x1 + centerX, y1 + centerY);
        glVertex2d(x2 + centerX, y2 + centerY);
    }
    glEnd();

    // decorate with lines
    glLineWidth(5.0f);
    glBegin(GL_LINES);
    for(int i = 0; i <= N; i++)
    {
        double angle1 = 3.1415926 * 2.0 / N * i;
        double x1 = cos(angle1) * i / N * _scaleOfDrawing;
        double y1 = sin(angle1) * i / N * _scaleOfDrawing;

        double angle2 = 3.1415926 * 2.0 / N * (i+1);
        double x2 = cos(angle2) * i / N * _scaleOfDrawing;
        double y2 = sin(angle2) * i / N * _scaleOfDrawing;

        qglColor(Qt::black);
        glVertex2d(x1 + centerX, y1 + centerY);
        glVertex2d(x2 + centerX, y2 + centerY);
    }
    glEnd();

    // center point
    glPointSize(20.0);
    glBegin(GL_POINTS);
    qglColor(Qt::red);
    glVertex2d(centerX, centerY);
    glEnd();
}

void Paint2DWidget::resizeGL( int w, int h )
{
    glViewport(0, 0, qMax(w, h), qMax(w, h));
}

void Paint2DWidget::mousePressEvent( QMouseEvent * e )
{
    _lastMousePos = e->pos();
    setCursor(Qt::OpenHandCursor);
}

void Paint2DWidget::mouseMoveEvent( QMouseEvent * e )
{
    if(e->buttons() != Qt::NoButton)
    {
        setCursor(Qt::ClosedHandCursor);
        auto trans = (e->pos() - _lastMousePos) / qMax(width(), height()) * 2.0;
        _centerOfDrawing.rx() += trans.x();
        _centerOfDrawing.ry() -= trans.y();
        _lastMousePos = e->pos();
        update();
    }
}

void Paint2DWidget::mouseReleaseEvent( QMouseEvent * e )
{
    setCursor(Qt::ArrowCursor);
}

void Paint2DWidget::wheelEvent( QWheelEvent * e )
{
    _scaleOfDrawing *= std::exp(e->delta() / 10000.0f);
    update();
}
