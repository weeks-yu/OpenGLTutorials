#include "basicwidget.h"

#include <QtWidgets>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	//BasicWidget1 w;
	//w.resize(500, 500);
	//w.show();
	
	BasicWidget2 w;
	BasicWidget2_1 w1;
	BasicWidget2_2 w2;
	BasicWidget2_3 w3;
	BasicWidget2_4 w4;
	w.resize(500, 500);
	w1.resize(500, 500);
	w2.resize(500, 500);
	w3.resize(500, 500);
	w4.resize(500, 500);
	w.show();
	w1.show();
	w2.show();
	w3.show();
	w4.show();

	return a.exec();
}
