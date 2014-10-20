#include <QtOpenGL>

#include "paint2dsimplewidget.h"
#include "paint2dwidget.h"
#include "transformwidget.h"
#include "camerawidget.h"
#include "meshwidget.h"
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

	_mdiArea->addSubWindow(new TextureWidget);
	_mdiArea->addSubWindow(new CameraWidget);
	_mdiArea->addSubWindow(new MeshWidget);
	_mdiArea->addSubWindow(new TransformWidget);
	_mdiArea->addSubWindow(new Paint2DWidget);
	_mdiArea->addSubWindow(new Paint2DSimpleWidget);	

	_mdiArea->setBackground(Qt::BrushStyle::CrossPattern);
	_mdiArea->tileSubWindows();
}

GLv1Win::~GLv1Win()
{

}

void GLv1Win::on_actionTileWin_triggered()
{
	_mdiArea->tileSubWindows();
}

void GLv1Win::on_actionCascadeWin_triggered()
{
	_mdiArea->cascadeSubWindows();
}

void GLv1Win::on_actionAbout_triggered()
{
	QMessageBox::about(this, 
		tr("About this program"), 
		tr("This is a simple program based on OpenGL 1.0 and Qt"));
}
