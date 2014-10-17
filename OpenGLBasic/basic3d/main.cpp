#include "openglbasic3dwin.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	OpenGLBasic3dWin w;
	w.show();
	return a.exec();
}
