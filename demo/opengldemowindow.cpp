#include <QtOpenGL>

#include "paint2dsimplewidget.h"
#include "paint2dwidget.h"
#include "transformwidget.h"
#include "camerawidget.h"
#include "meshwidget.h"
#include "texturewidget.h"
#include "shaderwidget.h"

#include "opengldemowindow.h"

OpenGLDemoWindow::OpenGLDemoWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QGLFormat glf = QGLFormat::defaultFormat();
    qDebug("OpenGL version: %d.%d", glf.majorVersion(), glf.minorVersion());
    // enable anti-aliasing by over-sampling
    glf.setSampleBuffers(true);
    glf.setSamples(8);
    QGLFormat::setDefaultFormat(glf);

    // initialize GUI
    ui.setupUi(this);
    
    _mdiArea = new QMdiArea(this);
    setCentralWidget(_mdiArea);

    _mdiArea->addSubWindow(new ShaderWidget);
    _mdiArea->addSubWindow(new TextureWidget);
    _mdiArea->addSubWindow(new CameraWidget);
    _mdiArea->addSubWindow(new MeshWidget);
    _mdiArea->addSubWindow(new TransformWidget);
    _mdiArea->addSubWindow(new Paint2DWidget);
    //_mdiArea->addSubWindow(new Paint2DSimpleWidget);    

    _mdiArea->setBackground(Qt::darkGray);
    _mdiArea->tileSubWindows();
}

OpenGLDemoWindow::~OpenGLDemoWindow()
{}

void OpenGLDemoWindow::on_actionTileWin_triggered()
{
    _mdiArea->tileSubWindows();
}

void OpenGLDemoWindow::on_actionCascadeWin_triggered()
{
    _mdiArea->cascadeSubWindows();
}

void OpenGLDemoWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, 
        tr("About this program"), 
        tr(
        "Developed using OpenGL and Qt.\n"
        "Copyright (c) 2014, Institute of Computer Graphics and Computer-Aided Design, Tsinghua University."
        ));
}
