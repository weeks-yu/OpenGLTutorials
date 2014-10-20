#include <iostream>
#include <QtDebug>
#include "transformwidget.h"

TransformWidget::TransformWidget(QWidget *parent)
	: QGLWidget(parent)
{
	setWindowTitle(tr("3. Model Transform"));
	setMinimumSize(200, 200);
	setMouseTracking(true);
	setFocusPolicy(Qt::ClickFocus);

	// initialize model matrix data
	_modelMatrix.setToIdentity();
	_modelMatrix.scale(100);
	_modelMatrix.rotate(45, 1, 1, 1);

	// initialize projection matrix data
	_usePerspectiveProjection = false;
	updateProjectionMatrix();
}

TransformWidget::~TransformWidget()
{

}

void TransformWidget::initializeGL()
{
	makeCurrent();

	glEnable(GL_MULTISAMPLE);
	GLint bufs;
	GLint samples;
	glGetIntegerv(GL_SAMPLE_BUFFERS, &bufs);
	glGetIntegerv(GL_SAMPLES, &samples);

	printf("Have %d buffers and %d samples", bufs, samples);
}

void TransformWidget::paintGL()
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

	// setup model matrix (ignore the "view matrix" since it is identity here)
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(_modelMatrix.data());

	// setup projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(_projectionMatrix.data());

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

void TransformWidget::resizeGL( int w, int h )
{
	glViewport(0, 0, w, h);
	updateProjectionMatrix();
}

void TransformWidget::mousePressEvent( QMouseEvent * e )
{
	_lastMousePos = e->pos();
	setCursor(Qt::OpenHandCursor);
}

void TransformWidget::mouseMoveEvent( QMouseEvent * e )
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

void TransformWidget::mouseReleaseEvent( QMouseEvent * e )
{
	setCursor(Qt::ArrowCursor);
}

void TransformWidget::wheelEvent( QWheelEvent * e )
{
	_modelMatrix.scale(exp(e->delta() / 1000.0));
	update();
}

void TransformWidget::updateProjectionMatrix()
{
	if(!_usePerspectiveProjection)
	{
		_projectionMatrix.setToIdentity();
		_projectionMatrix.ortho(-width()/2.0, width()/2.0, -height()/2.0, height()/2.0, -1e3, 1e3);
	}
	else
	{
		_projectionMatrix.setToIdentity();
		_projectionMatrix.perspective(20.0f, double(height()) / width(), 0.01, 1e4);
	}
	update();
}

void TransformWidget::keyPressEvent( QKeyEvent * e )
{
	if(e->key() == Qt::Key_C)
	{
		_usePerspectiveProjection = !_usePerspectiveProjection;
		updateProjectionMatrix();
	}
}
