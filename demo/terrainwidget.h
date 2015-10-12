#pragma once

#include <QtOpenGL>

class TerrainWidget : public QGLWidget, public QGLFunctions 
{

public:
    TerrainWidget(QWidget *parent = nullptr);
    ~TerrainWidget();

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

    // prepare data
    void prepare();


private:
    QMatrix4x4 _modelMatrix;
    float _heightRatio;

    QVector<QVector2D> _grids; // grid data
    QVector<quint32> _triangleIndices; // indices of vertices for drawing triangles


    // buffer for storing the _grids data on GPU
    GLuint _gridBuffer;
    // buffer for storing the _triangleIndices data on GPU
    GLuint _triangleIndicesBuffer;

    // id of the OpenGL shader program
    GLuint _program;
    // location of uniform variables in the OpenGL shader program 
    GLuint _modelMatrixLocation, _viewMatrixLocation, _projectionMatrixLocation, 
        _normalHeightMapLocation, _heightRatioLocation;

    // texture of the height map
    GLuint _texture;
    // the normal height map
    QImage _normalHeightMap;

private:
    QPointF _lastMousePos;

};
