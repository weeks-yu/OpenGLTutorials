#pragma once

#include <QtOpenGL>

class CubeWidget : public QGLWidget
{
public:
    CubeWidget(QWidget *parent = nullptr);
    ~CubeWidget();

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
    QMatrix4x4 _modelMatrix;

private:
    QPointF _lastMousePos;
};
