#ifndef MESHWIDGET_H
#define MESHWIDGET_H

#include <QtOpenGL>

class MeshWidget : public QGLWidget
{
    Q_OBJECT

public:
    MeshWidget(QWidget *parent = nullptr);
    ~MeshWidget();

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

private:
    QPointF _lastMousePos;
};

#endif // MESHWIDGET_H
