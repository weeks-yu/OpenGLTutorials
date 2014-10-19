#include "camerawidget.h"


PerspectiveCamera::PerspectiveCamera(int w, int h, double focal, const QVector3D & eye,
									 const QVector3D & center, const QVector3D & up, double n, double f)
									 : _screenW(w), _screenH(h), _focal(focal), _eye(eye), _center(center), _up(up), _near(n), _far(f) 
{
	 updateMatrices();
}

void PerspectiveCamera::updateMatrices() 
{
	_viewMatrix.setToIdentity();
	_viewMatrix.lookAt(_eye, _center, _up);

	double verticalViewAngle = atan(_screenH / 2.0 / _focal) * 2;
	double aspect = double(_screenW) / double(_screenH);
	_projectionMatrix.setToIdentity();
	_projectionMatrix.perspective(verticalViewAngle * 180.0 / M_PI, aspect, _near, _far);

	_viewProjectionMatrix = _projectionMatrix * _viewMatrix;
	_viewProjectionMatrixInv = _viewProjectionMatrix.inverted();
}

QVector2D PerspectiveCamera::screenProjection(const QVector3D & p3) const 
{
	QVector4D p4(p3, 1);
	QVector4D position = _viewProjectionMatrix * p4;
	double xratio = position.x() / position.w() / 2;
	double yratio = position.y() / position.w() / 2;
	double x = (xratio + 0.5) * _screenW;
	double y = _screenH - (yratio + 0.5) * _screenH;
	return QVector2D(x, y);
}

bool PerspectiveCamera::isVisibleOnScreen(const QVector3D & p3d) const 
{
	QVector4D p4(p3d, 1);
	QVector4D position = _viewProjectionMatrix * p4;
	return position.w() > 0 && position.z() > 0;
}

QVector3D PerspectiveCamera::spatialDirection(const QVector2D & p2d) const 
{
	double xratio = (p2d.x() / _screenW - 0.5) * 2;
	double yratio = ((_screenH - p2d.y()) / _screenH - 0.5) * 2;
	QVector4D position(xratio, yratio, 1, 1);
	QVector4D realPosition = _viewProjectionMatrixInv * position;
	return QVector3D(realPosition.x() / realPosition.w(),
		realPosition.y() / realPosition.w(),
		realPosition.z() / realPosition.w());
}

void PerspectiveCamera::resizeScreen(const QSizeF & sz, bool updateMat) 
{
	if (_screenH == sz.height() && _screenW == sz.width())
		return;
	_screenH = sz.height();
	_screenW = sz.width();
	if (updateMat)
		updateMatrices();
}

void PerspectiveCamera::setFocal(double f, bool updateMat) {

	if (f == _focal)
		return;
	_focal = f;
	if (updateMat)
		updateMatrices();
}

void PerspectiveCamera::setEye(const QVector3D & e, bool updateMat)
{
	if (_eye == e)
		return;
	_eye = e;
	if (updateMat)
		updateMatrices();
}

void PerspectiveCamera::setCenter(const QVector3D & c, bool updateMat)
{
	if (_center == c)
		return;
	_center = c;
	if (updateMat)
		updateMatrices();
}

void PerspectiveCamera::setUp(const QVector3D & up, bool updateMat)
{
	if (_up == up)
		return;
	_up = up;
	if (updateMat)
		updateMatrices();
}

void PerspectiveCamera::setNearAndFarPlanes(double n, double f, bool updateMat) 
{
	if (_near == n && _far == f)
		return;
	_near = n;
	_far = f;
	if (updateMat)
		updateMatrices();
}



template <class T>
inline T BoundBetween(const T & v, const T & low, const T & high)
{
	return v < low ? low : (v > high ? high : v);
}





CameraWidget::CameraWidget(QWidget *parent)
	: QGLWidget(parent)
{
	setWindowTitle(tr("Camera Widget"));
	setMinimumSize(500, 500);
	setMouseTracking(true);
	setFocusPolicy(Qt::ClickFocus);

	// initialize model matrix data
	_modelMatrix.setToIdentity();
	_modelMatrix.translate(0, 0, -1);
	_modelMatrix.rotate(45, 1, 1, 1);

	// set camera
	_camera.setEye(QVector3D(0, 0, -5), false);
	_camera.setCenter(QVector3D(0, 0, 0), false);
	_camera.setUp(QVector3D(0, 1, 0), false);
	_camera.resizeScreen(size());
}

CameraWidget::~CameraWidget()
{

}

void CameraWidget::initializeGL()
{
	makeCurrent();

	glEnable(GL_MULTISAMPLE);
	GLint bufs;
	GLint samples;
	glGetIntegerv(GL_SAMPLE_BUFFERS, &bufs);
	glGetIntegerv(GL_SAMPLES, &samples);

	printf("Have %d buffers and %d samples", bufs, samples);
}

void CameraWidget::paintGL()
{
	QPainter painter;
	painter.begin(this);

	painter.beginNativePainting();
	qglClearColor(Qt::black);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// setup projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf(_camera.projectionMatrix().data());

	// setup view matrix for axis (model matrix of axis is identity)
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf(_camera.viewMatrix().data());

	// paint axis 
	glLineWidth(2.0);
	glBegin(GL_LINES);
	qglColor(Qt::red);
	glVertex3d(0, 0, 0);
	glVertex3d(1, 0, 0);
	qglColor(Qt::green);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 1, 0);
	qglColor(Qt::blue);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, 1);
	glEnd();
	glPointSize(5.0);
	glBegin(GL_POINTS);
	qglColor(Qt::yellow);
	glVertex3d(0, 0, 0);
	glEnd();


	// setup model-view matrix for cube
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf((_camera.viewMatrix() * _modelMatrix).data());

	// draw a cube
	static const double verts[8][4] = {
		{ -1, -1, -1, 1 },
		{ 1, -1, -1, 1 },
		{ 1, 1, -1, 1 },
		{ -1, 1, -1, 1 },
		{ -1, -1, 1, 1 },
		{ 1, -1, 1, 1 },
		{ 1, 1, 1, 1 },
		{ -1, 1, 1, 1 }
	};

	static const int quadFaces[6][4] = {
		{ 0, 1, 5, 4 },
		{ 4, 5, 6, 7 },
		{ 1, 2, 6, 5 },
		{ 0, 4, 7, 3 },
		{ 2, 3, 7, 6 },
		{ 1, 0, 3, 2 }
	};

	static const QColor faceColors[6] = {
		Qt::red,
		Qt::yellow,
		Qt::green,
		Qt::blue,
		Qt::cyan,
		Qt::magenta
	};

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 6; i++) {
		glColor4f(faceColors[i].redF(), faceColors[i].greenF(), faceColors[i].blueF(), 0.8f);

		glVertex4dv(verts[quadFaces[i][0]]);
		glVertex4dv(verts[quadFaces[i][1]]);
		glVertex4dv(verts[quadFaces[i][2]]);

		glVertex4dv(verts[quadFaces[i][0]]);
		glVertex4dv(verts[quadFaces[i][2]]);
		glVertex4dv(verts[quadFaces[i][3]]);
	}
	glEnd();


	// restore all native states
	painter.endNativePainting();
}

void CameraWidget::resizeGL( int w, int h )
{
	glViewport(0, 0, w, h);
	_camera.resizeScreen(QSizeF(w, h));
}

void CameraWidget::mousePressEvent( QMouseEvent * e )
{
	_lastMousePos = e->pos();
	setCursor(Qt::OpenHandCursor);
}

void CameraWidget::mouseMoveEvent( QMouseEvent * e )
{
	if(e->buttons() != Qt::NoButton)
	{
		setCursor(Qt::ClosedHandCursor);
		auto t = (e->pos() - _lastMousePos);
		QMatrix4x4 rotMat;
		rotMat.rotate(t.x() / 10.0 * M_PI, 0, 1, 0);
		rotMat.rotate(t.y() / 10.0 * M_PI, 1, 0, 0);
		_modelMatrix = rotMat * _modelMatrix;
		_lastMousePos = e->pos();
		update();
	}
}

void CameraWidget::mouseReleaseEvent( QMouseEvent * e )
{
	setCursor(Qt::ArrowCursor);
}

void CameraWidget::wheelEvent( QWheelEvent * e )
{
	_modelMatrix.scale(exp(e->delta() / 1000.0));
	update();
}

void CameraWidget::keyPressEvent( QKeyEvent * e )
{
	if(e->key() == Qt::Key_W) // move forward
	{
		auto trans = _camera.forward() * 0.1;
		_camera.setEye(_camera.eye() + trans, false);
		_camera.setCenter(_camera.center() + trans);
	}
	else if(e->key() == Qt::Key_A) // left move
	{
		auto trans = _camera.leftward() * 0.1;
		_camera.setEye(_camera.eye() + trans, false);
		_camera.setCenter(_camera.center() + trans);
	}
	else if(e->key() == Qt::Key_D) // right move
	{
		auto trans = _camera.rightward() * 0.1;
		_camera.setEye(_camera.eye() + trans, false);
		_camera.setCenter(_camera.center() + trans);
	}
	else if(e->key() == Qt::Key_S) // back move
	{
		auto trans = _camera.backward() * 0.1;
		_camera.setEye(_camera.eye() + trans, false);
		_camera.setCenter(_camera.center() + trans);
	}
	else if(e->key() == Qt::Key_Left)
	{
		double d = (_camera.center() - _camera.eye()).length();
		auto newCenter = (_camera.center() + _camera.leftward() * tan(0.05) * d).normalized() * d;
		_camera.setCenter(newCenter);
	}
	else if(e->key() == Qt::Key_Right)
	{
		double d = (_camera.center() - _camera.eye()).length();
		auto newCenter = (_camera.center() + _camera.rightward() * tan(0.05) * d).normalized() * d;
		_camera.setCenter(newCenter);
	}
	
	double d = QVector3D::dotProduct((_modelMatrix * QVector4D(0, 0, 0, 1)).toVector3DAffine() - _camera.eye(), _camera.forward());
	double n = BoundBetween(d - 100, 1e-3, 1e3);
	double f = BoundBetween(d + 100, 1e-3, 1e3);
	_camera.setNearAndFarPlanes(n, f);

	qDebug() << "....";

	update();
}
