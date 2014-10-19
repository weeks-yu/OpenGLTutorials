#include <QtOpenGL>

#include "paint2dwidget.h"
#include "transformwidget.h"
#include "camerawidget.h"
#include "texturewidget.h"

#include "glv1win.h"

GLv1Win::GLv1Win(QWidget *parent)
	: QMainWindow(parent)
{
	QGLFormat glf = QGLFormat::defaultFormat();
	qDebug("OpenGL version: %d.%d", glf.majorVersion(), glf.minorVersion());
	glf.setSampleBuffers(true);
	glf.setSamples(8);
	QGLFormat::setDefaultFormat(glf);

	// initialize GUI
	ui.setupUi(this);
	
	_mdiArea = new QMdiArea(this);
	setCentralWidget(_mdiArea);

	_mdiArea->addSubWindow(new Paint2DWidget);
	_mdiArea->addSubWindow(new TransformWidget);
	_mdiArea->addSubWindow(new CameraWidget);
	_mdiArea->addSubWindow(new TextureWidget);

	connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(on_actionAbout_triggered()));
}

GLv1Win::~GLv1Win()
{

}

void GLv1Win::on_actionTileWin_triggered()
{

}

void GLv1Win::on_actionCascadeWin_triggered()
{

}

void GLv1Win::on_actionAbout_triggered()
{

}
