// author: yanghao (yangh2007@gmail.com)

#ifndef SHADERWIDGET_H
#define SHADERWIDGET_H

#include <QtOpenGL>

class ShaderWidget : public QGLWidget, public QGLFunctions
{
    Q_OBJECT

public:
    ShaderWidget(QWidget *parent = nullptr);
    ~ShaderWidget();

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

    // load mesh
    void loadMesh(const QString & file);

private:
    QMatrix4x4 _modelMatrix;

    // mesh data
    struct Vertex // data of each vertex
    {
        QVector3D position;
        QVector3D normal;
    };
    QVector<Vertex> _vertices; // data of all vertices
    QVector<quint32> _triangleIndices; // indices of vertices for drawing triangles


    // buffer for storing the _vertices data on GPU
    GLuint _vertBuffer;
    // buffer for storing the _triangleIndices data on GPU
    GLuint _triangleIndicesBuffer;

    // id of the OpenGL shader program
    GLuint _program;
    // location of uniform variables in the OpenGL shader program 
    GLuint _modelMatrixLocation, _viewMatrixLocation, _projectionMatrixLocation;

private:
    QPointF _lastMousePos;

};

#endif // SHADERWIDGET_H
