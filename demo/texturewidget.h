#ifndef TEXTUREWIDGET_H
#define TEXTUREWIDGET_H

#include <QtOpenGL>

class TextureWidget : public QGLWidget
{
public:
    TextureWidget(QWidget *parent = nullptr);
    ~TextureWidget();

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

    // load texture and build sphere
    void buildModel();

private:
    QMatrix4x4 _modelMatrix, _projectionMatrix;
    QVector<QVector4D> _vertexPositions;
    QVector<QVector2D> _texCoords;
    QVector<QVector3D> _normals;
    QVector<QVector<int>> _vertIds;
    GLuint _textureId;

private:
    QPointF _lastMousePos;  
};

#endif // TEXTUREWIDGET_H
