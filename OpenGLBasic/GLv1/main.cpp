#include "glv1win.h"
#include <QtCore>
#include <QtWidgets>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	GLv1Win w;
	w.resize(700, 700);
	w.show();
	return a.exec();
}
