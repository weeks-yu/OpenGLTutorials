#include <gl/glut.h>
#include "texturewidget.h"

static const int M = 128, N = 256;

TextureWidget::TextureWidget(QWidget *parent)
	: QGLWidget(parent)
{
	setWindowTitle(tr("6. Texture and Light"));
	setMinimumSize(200, 200);
	setMouseTracking(true);

	// initialize model matrix data
	_modelMatrix.setToIdentity();
	_modelMatrix.scale(100);
	_modelMatrix.rotate(45, 1, 1, 1);

	// initialize projection matrix data
	_projectionMatrix.setToIdentity();
	_projectionMatrix.ortho(-width()/2.0, width()/2.0, -height()/2.0, height()/2.0, -1e4, 1e4);
}

TextureWidget::~TextureWidget()
{

}

void TextureWidget::initializeGL()
{
	makeCurrent();
	buildModel();
}
	
void TextureWidget::paintGL()
{
	QPainter painter;
	painter.begin(this);

	painter.beginNativePainting();
	qglClearColor(Qt::black);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// enable and bind texture
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);

	static GLfloat mat_diffuse[] = {100, 100, 100, 100};
	static GLfloat mat_ambient[] = {0.f, .5f, .5f, .5f};
	static GLfloat mat_specular[] = {100, 100, 100, 100};
	static GLfloat mat_shininess = 100.0f;

	// set material of the model
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// setup projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(_projectionMatrix.data());

	// set position of the light
	static GLfloat lightPosition[4] = { 1, 1, 1, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	// setup model matrix (ignore the "view matrix" here)
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(_modelMatrix.data());

	// draw the sphere
	glBegin(GL_QUADS);
	for (int i = 1; i < M; i++) {
		for (int j = 1; j < N; j++){
			glNormal3fv(&(_normals[_vertIds[i][j]][0]));
			glTexCoord2fv(&(_texCoords[_vertIds[i][j]][0]));
			glVertex4fv(&(_vertexPositions[_vertIds[i][j]][0]));

			glNormal3fv(&(_normals[_vertIds[i][j-1]][0]));
			glTexCoord2fv(&(_texCoords[_vertIds[i][j-1]][0]));
			glVertex4fv(&(_vertexPositions[_vertIds[i][j-1]][0]));

			glNormal3fv(&(_normals[_vertIds[i-1][j-1]][0]));
			glTexCoord2fv(&(_texCoords[_vertIds[i-1][j-1]][0]));
			glVertex4fv(&(_vertexPositions[_vertIds[i-1][j-1]][0]));

			glNormal3fv(&(_normals[_vertIds[i-1][j]][0]));
			glTexCoord2fv(&(_texCoords[_vertIds[i-1][j]][0]));
			glVertex4fv(&(_vertexPositions[_vertIds[i-1][j]][0]));
		}
	}
	glEnd();	

	// restore all native states
	painter.endNativePainting();
}

void TextureWidget::resizeGL( int w, int h )
{
	glViewport(0, 0, w, h);
	_projectionMatrix.setToIdentity();
	_projectionMatrix.ortho(-width()/2.0, width()/2.0, -height()/2.0, height()/2.0, -1e4, 1e4);
}

void TextureWidget::mousePressEvent( QMouseEvent * e )
{
	_lastMousePos = e->pos();
	setCursor(Qt::OpenHandCursor);
}

void TextureWidget::mouseMoveEvent( QMouseEvent * e )
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

void TextureWidget::mouseReleaseEvent( QMouseEvent * e )
{
	setCursor(Qt::ArrowCursor);
}

void TextureWidget::wheelEvent( QWheelEvent * e )
{
	_modelMatrix.scale(exp(e->delta() / 1000.0));
	update();
}

void TextureWidget::buildModel()
{
	// load texture
	QImage im(":/images/earthmap.jpg");
	Q_ASSERT(!im.isNull());
	_textureId = bindTexture(im);

	// build sphere data
	_vertexPositions.reserve(M * N);
	_normals.reserve(M * N);
	_texCoords.reserve(M * N);
	_vertIds = QVector<QVector<int>>(M, QVector<int>(N));
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			float xratio = 1.0f / (N - 1) * j;
			float yratio = 1.0f / (M - 1) * i;
			float xangle = M_PI * 2 * xratio;
			float yangle = M_PI * yratio - M_PI_2;
			QVector4D pos(cos(xangle)*cos(yangle), sin(yangle), sin(xangle)*cos(yangle),  1);
			_vertexPositions << pos;
			QVector3D normal(cos(xangle)*cos(yangle), sin(yangle), sin(xangle)*cos(yangle));
			_normals << normal;
			QVector2D texCoord(1.0 - xratio, yratio);
			_texCoords << texCoord;
			_vertIds[i][j] = _texCoords.size() - 1;
		}
	}
}
