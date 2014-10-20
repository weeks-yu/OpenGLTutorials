#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QtOpenGL>

// perspective camera
class PerspectiveCamera 
{
public:
	explicit PerspectiveCamera(int w = 500, int h = 500, double focal = 250,
		const QVector3D & eye = QVector3D(0, 0, 0),
		const QVector3D & center = QVector3D(1, 0, 0),
		const QVector3D & up = QVector3D(0, 0, 1),
		double nearPlane = 0.01, double farPlane = 1e4);

	inline QSizeF screenSize() const { return QSizeF(static_cast<float>(_screenW), static_cast<float>(_screenH)); }
	inline double screenWidth() const { return _screenW; }
	inline double screenHeight() const { return _screenH; }
	inline double fovRadians() const { return atan(_screenH / 2.0 / _focal) * 2; }
	inline double fovAngles() const { return fovRadians() * 180.0 / M_PI; }
	inline double aspect() const { return double(_screenW) / double(_screenH); }
	inline double focal() const { return _focal; }
	inline const QVector3D & eye() const { return _eye; }
	inline const QVector3D & center() const { return _center; }
	inline const QVector3D & up() const { return _up; }
	inline double nearPlane() const { return _near; }
	inline double farPlane() const { return _far; }
	QVector2D screenProjection(const QVector3D & p3d) const;
	bool isVisibleOnScreen(const QVector3D & p3d) const;
	QVector3D spatialDirection(const QVector2D & p2d) const;

	inline const QMatrix4x4 & viewMatrix() const { return _viewMatrix; }
	inline const QMatrix4x4 & projectionMatrix() const { return _projectionMatrix; }
	inline const QMatrix4x4 & viewProjectionMatrix() const { return _viewProjectionMatrix; }
	inline const QMatrix4x4 & viewProjectionMatrixInv() const { return _viewProjectionMatrixInv; }

	// operations
	void resizeScreen(const QSizeF & sz, bool updateMat = true);
	void setFocal(double f, bool updateMat = true);
	void setEye(const QVector3D & e, bool updateMat = true);
	void setCenter(const QVector3D & c, bool updateMat = true);
	void setUp(const QVector3D & up, bool updateMat = true);
	void setNearAndFarPlanes(double nearPlane, double farPlane, bool updateMat = true);

	// directions
	inline QVector3D forward() const { return (_center - _eye).normalized(); }
	inline QVector3D backward() const { return (_eye - _center).normalized(); }
	inline QVector3D upward() const { return _up.normalized(); }
	inline QVector3D downward() const { return -upward(); }
	inline QVector3D leftward() const { return QVector3D::crossProduct(_up, _center - _eye).normalized(); }
	inline QVector3D rightward() const { return -leftward(); }

	// travel
	void rectifyUp(bool updateMat = true); // rectify the up vector
	void translate(const QVector3D & t, bool updateMat = true);
	void turnDirection(double rightAngle, double upAngle, bool updateMat = true);

private:
	void updateMatrices();

private:
	double _screenW, _screenH;
	double _focal;
	double _near, _far;
	QVector3D _eye, _center, _up;
	QMatrix4x4 _viewMatrix, _projectionMatrix, _viewProjectionMatrix, _viewProjectionMatrixInv;
};


class CameraWidget : public QGLWidget
{
	Q_OBJECT

public:
	CameraWidget(QWidget *parent = nullptr);
	~CameraWidget();

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
	virtual void keyPressEvent(QKeyEvent * e) override;

private:
	QMatrix4x4 _modelMatrix;
	PerspectiveCamera _camera;

	QPointF _lastMousePos;
};

#endif // CAMERAWIDGET_H
