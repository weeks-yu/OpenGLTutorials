#include "glv2win.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	GLv2Win w;
	w.show();
	return a.exec();
}
