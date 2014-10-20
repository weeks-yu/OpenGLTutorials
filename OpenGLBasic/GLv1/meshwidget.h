#ifndef MESHWIDGET_H
#define MESHWIDGET_H

#include <QtOpenGL>

class MeshWidget : public QGLWidget, public QGLFunctions
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
	QVector<QVector3D> _vertPositions;
	QVector<QVector3D> _vertNormals;
	QVector<int32_t> _triangleIndices;

	GLuint _vertPositionsBuffer;
	GLuint _vertNormalsBuffer;
	GLuint _triangleIndicesBuffer;


private:
	QPointF _lastMousePos;
};

#endif // MESHWIDGET_H
