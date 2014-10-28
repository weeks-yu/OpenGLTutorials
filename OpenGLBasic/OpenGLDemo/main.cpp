#include "opengldemowindow.h"
#include <QtCore>
#include <QtWidgets>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	OpenGLDemoWindow w;
	w.resize(800, 800);
	w.showMaximized();
	return a.exec();
}
