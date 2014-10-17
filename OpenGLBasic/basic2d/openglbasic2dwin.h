#ifndef OPENGLBASIC2DWIN_H
#define OPENGLBASIC2DWIN_H

#include <QtWidgets/QMainWindow>
#include "ui_openglbasic2dwin.h"

class OpenGLBasic2dWin : public QMainWindow
{
	Q_OBJECT

public:
	OpenGLBasic2dWin(QWidget *parent = 0);
	~OpenGLBasic2dWin();

private:
	Ui::OpenGLBasic2dWinClass ui;
};

#endif // OPENGLBASIC2DWIN_H
