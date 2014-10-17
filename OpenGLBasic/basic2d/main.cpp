#include "openglbasic2dwin.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	OpenGLBasic2dWin w;
	w.show();
	return a.exec();
}
