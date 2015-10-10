#include <QtCore>
#include <QtWidgets>
#include <QtOpenGL>

#include "opengldemowindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    QGLFormat glf = QGLFormat::defaultFormat();
    qDebug("OpenGL version: %d.%d", glf.majorVersion(), glf.minorVersion());
    glf.setSampleBuffers(true);
    glf.setSamples(16);
    QGLFormat::setDefaultFormat(glf);

    OpenGLDemoWindow w;
    w.resize(800, 800);
    w.showMaximized();
    return a.exec();
}
