#include <QtCore>
#include <QtWidgets>
#include <QtOpenGL>

#include "opengldemowindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    OpenGLDemoWindow w;
    w.resize(800, 800);
    w.show();

    return a.exec();
}
