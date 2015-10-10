// author: yanghao (yangh2007@gmail.com)

#ifndef PAINT2DWIDGET_H
#define PAINT2DWIDGET_H

#include <QGLWidget>

class Paint2DWidget : public QGLWidget
{
    Q_OBJECT

public:
    Paint2DWidget(QWidget *parent = nullptr);
    ~Paint2DWidget();

protected:
    // opengl methods
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int w, int h) override;

    // mouse event handlers
    virtual void mousePressEvent(QMouseEvent * e) override;
    virtual void mouseMoveEvent(QMouseEvent * e) override;
    virtual void mouseReleaseEvent(QMouseEvent * e) override;
    virtual void wheelEvent(QWheelEvent * e) override;

private:
    QPointF _centerOfDrawing;
    float _scaleOfDrawing;

private:
    QPointF _lastMousePos;
};

#endif // PAINT2DWIDGET_H
