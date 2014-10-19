#ifndef GLV1WIN_H
#define GLV1WIN_H

#include <QtWidgets>
#include "ui_glv1win.h"

class GLv1Win : public QMainWindow
{
	Q_OBJECT

public:
	GLv1Win(QWidget *parent = 0);
	~GLv1Win();

public slots:
	void on_actionTileWin_triggered();
	void on_actionCascadeWin_triggered();
	void on_actionAbout_triggered();

private:
	QMdiArea * _mdiArea;

private:
	Ui::GLv1WinClass ui;
};

#endif // GLV1WIN_H
