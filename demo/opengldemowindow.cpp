#include <QtOpenGL>

#include "paint2dwidget.h"
#include "cubewidget.h"
#include "dragonwidget.h"
#include "dragon2widget.h"
#include "earthwidget.h"
#include "terrainwidget.h"

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

    _mdiArea->addSubWindow(new TerrainWidget);
    _mdiArea->addSubWindow(new Dragon2Widget); 
    _mdiArea->addSubWindow(new EarthWidget);
    _mdiArea->addSubWindow(new DragonWidget);
    _mdiArea->addSubWindow(new CubeWidget);
    _mdiArea->addSubWindow(new Paint2DWidget);

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
        "Copyright (c) 2015, Institute of Computer Graphics and Computer-Aided Design, Tsinghua University."
        ));
}
