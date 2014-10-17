#ifndef OPENGLBASIC3DWIN_H
#define OPENGLBASIC3DWIN_H

#include <QtWidgets/QMainWindow>
#include "ui_openglbasic3dwin.h"

class OpenGLBasic3dWin : public QMainWindow
{
	Q_OBJECT

public:
	OpenGLBasic3dWin(QWidget *parent = 0);
	~OpenGLBasic3dWin();

private:
	Ui::OpenGLBasic3dWinClass ui;
};

#endif // OPENGLBASIC3DWIN_H
