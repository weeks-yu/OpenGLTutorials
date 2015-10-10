// author: yanghao (yangh2007@gmail.com)

#ifndef OPENGLDEMOWINDOW_H
#define OPENGLDEMOWINDOW_H

#include <QtWidgets>
#include "ui_opengldemowindow.h"

// the main window
class OpenGLDemoWindow : public QMainWindow
{
	Q_OBJECT

public:
	OpenGLDemoWindow(QWidget *parent = 0);
	~OpenGLDemoWindow();

public slots:
	void on_actionTileWin_triggered();
	void on_actionCascadeWin_triggered();
	void on_actionAbout_triggered();

private:
	QMdiArea * _mdiArea;

private:
	Ui::OpenGLDemoWindowClass ui;
};

#endif // OPENGLDEMOWINDOW_H
