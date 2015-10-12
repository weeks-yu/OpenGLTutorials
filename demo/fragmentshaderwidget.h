#ifndef FRAGMENTSHADERWIDGET_H
#define FRAGMENTSHADERWIDGET_H

#include <QtOpenGL>

class FragmentShaderWidget : public QGLWidget, public QGLFunctions
{
	Q_OBJECT

public:
    FragmentShaderWidget(const QByteArray & fragmentShaderSource, QWidget *parent = nullptr);
    ~FragmentShaderWidget();

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
    QVector<QVector2D> _vertices; // data of vertices
    QVector<quint32> _triangleIndices; // indices of vertices for drawing triangles

	 // the locations of the uniform variables in shader
	GLuint _resolutionLocation;
	GLuint _timeLocation;
	GLuint _mouseLocation;


    // buffer for storing the _vertices data on GPU
    GLuint _vertBuffer;
    // buffer for storing the _triangleIndices data on GPU
    GLuint _triangleIndicesBuffer;

    // id of the OpenGL shader program
    GLuint _program;

private:
    QPointF _lastMousePos;
	QPointF _currentMousePos;

	QTimer * _timer;
	uint32_t _timeValue;

	QByteArray _fragmentShaderSource;
};

#endif // FRAGMENTSHADERWIDGET_H
