#include <glut.h>
#include <QGLFunctions>

#include "meshwidget.h"

static const bool UseShaders = false;

MeshWidget::MeshWidget(QWidget *parent)
	: QGLWidget(parent), QGLFunctions()
{
	setWindowTitle(tr("4. Mesh and Projection"));
	setMinimumSize(200, 200);
	setMouseTracking(true);
	setFocusPolicy(Qt::ClickFocus);

	// load mesh
	loadMesh(tr("../data/dragon-10000.smf"));


	// initialize model matrix data
	_modelMatrix.setToIdentity();
	_modelMatrix.scale(1.5);
	_modelMatrix.rotate(45, 1, 1, 1);

	_vertPositionsBuffer = 0;
	_vertNormalsBuffer = 0;
	_triangleIndicesBuffer = 0;
}

MeshWidget::~MeshWidget()
{
	if(UseShaders)
	{
		glDeleteBuffers(1, &_vertPositionsBuffer);
		glDeleteBuffers(1, &_vertNormalsBuffer);
		glDeleteBuffers(1, &_triangleIndicesBuffer);
	}
}

void MeshWidget::initializeGL()
{
	makeCurrent();
	initializeGLFunctions(context());

	if(UseShaders)
	{
		// generate buffers
		glGenBuffers(1, &_vertPositionsBuffer);
		glGenBuffers(1, &_vertNormalsBuffer);
		glGenBuffers(1, &_triangleIndicesBuffer);

		// fill buffers with data
		glBindBuffer(GL_ARRAY_BUFFER, _vertPositionsBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(_vertPositions.first()) * _vertPositions.size(), 
			_vertPositions.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, _vertNormalsBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(_vertNormals.first()) * _vertNormals.size(),
			_vertNormals.data(), GL_STATIC_DRAW);
	
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _triangleIndicesBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_triangleIndices.first()) * _triangleIndices.size(),
			_triangleIndices.data(), GL_STATIC_DRAW);	

		// set vertex attrib pointer to these buffer
		glBindBuffer(GL_ARRAY_BUFFER, _vertPositionsBuffer);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(_vertPositions.first()), 0);

		glBindBuffer(GL_ARRAY_BUFFER, _vertNormalsBuffer);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(_vertNormals.first()), 0);
	}

	resizeGL(width(), height());
}

void MeshWidget::paintGL()
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
	glShadeModel(GL_SMOOTH);

	static GLfloat mat_diffuse[] = {1, 1, 1, 1};
	static GLfloat mat_ambient[] = {1, 0, 0, 1};
	static GLfloat mat_specular[] = {1, 1, 1, 1};
	static GLfloat mat_shininess = 5;
	static GLfloat lightPosition[4] = { 1e5, 1e5, 1e5, 1.0 };

	if(UseShaders)
	{

	}
	else
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		// set light position
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//////////////////////////////////////////////////////////////////////////
		// set view matrix
		gluLookAt(0, 0, -1000, 0, 0, 0, 0, -1, 0);
		// set model matrix
		glMultMatrixf(_modelMatrix.data());

		// set projection matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//////////////////////////////////////////////////////////////////////////
		gluPerspective(30, (double)width()/height(), 0.01, 1e5);
		//glOrtho(-width()/2, width()/2, -height()/2, height()/2, 0.01, 1e5);


		// draw mesh
		glBegin(GL_TRIANGLES);
		for(int vid : _triangleIndices)
		{
			glNormal3fv(&(_vertNormals[vid][0]));
			glVertex3fv(&(_vertPositions[vid][0]));
		}
		glEnd();	
	}

	// restore all native states
	painter.endNativePainting();

} 

void MeshWidget::resizeGL( int w, int h )
{
	glViewport(0, 0, w, h);
}

void MeshWidget::mousePressEvent( QMouseEvent * e )
{
	_lastMousePos = e->pos();
	setCursor(Qt::OpenHandCursor);
}

void MeshWidget::mouseMoveEvent( QMouseEvent * e )
{
	if(e->buttons() != Qt::NoButton)
	{
		setCursor(Qt::ClosedHandCursor);
		auto t = (e->pos() - _lastMousePos);
		QMatrix4x4 rotMat;
		rotMat.rotate(-t.x() / 10.0 * M_PI, 0, 1, 0);
		rotMat.rotate(t.y() / 10.0 * M_PI, 1, 0, 0);
		_modelMatrix = rotMat * _modelMatrix;
		_lastMousePos = e->pos();
		update();
	}
}

void MeshWidget::mouseReleaseEvent( QMouseEvent * e )
{
	setCursor(Qt::ArrowCursor);
}

void MeshWidget::wheelEvent( QWheelEvent * e )
{
	_modelMatrix.scale(exp(e->delta() / 1000.0));
	update();
}

void MeshWidget::loadMesh( const QString & f )
{
	_vertPositions.clear();
	_vertNormals.clear();
	_triangleIndices.clear();
	QFile file(f);
	if(file.open(QFile::ReadOnly))
	{
		QTextStream ts(&file);
		ts.readLine();
		QString token;
		while(true)
		{
			ts >> token;
			if(token == "v")
			{
				double x, y, z;
				ts >> x >> y >> z;
				_vertPositions << QVector3D(x, y, z);
				_vertNormals << QVector3D(0, 0, 0);
			}else if (token == "f")
			{
				int a, b, c;
				ts >> a >> b >> c;
				_triangleIndices << (a-1) << (b-1) << (c-1);
				QVector3D normal = QVector3D::crossProduct(_vertPositions[a-1] - _vertPositions[b-1], 
					_vertPositions[c-1] - _vertPositions[b-1]).normalized();
				_vertNormals[a-1] += normal;
				_vertNormals[b-1] += normal;
				_vertNormals[c-1] += normal;
			}else
			{
				break;
			}
		}
		for(auto & n : _vertNormals)
		{
			n = -n.normalized();
		}
	}
}
