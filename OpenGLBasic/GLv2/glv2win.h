#ifndef GLV2WIN_H
#define GLV2WIN_H

#include <QtWidgets/QMainWindow>
#include "ui_glv2win.h"

class GLv2Win : public QMainWindow
{
	Q_OBJECT

public:
	GLv2Win(QWidget *parent = 0);
	~GLv2Win();

private:
	Ui::GLv2WinClass ui;
};

#endif // GLV2WIN_H
