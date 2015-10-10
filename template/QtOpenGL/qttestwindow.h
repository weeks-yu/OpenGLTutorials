#ifndef QTTESTWINDOW_H
#define QTTESTWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_qttestwindow.h"

#include "testwidget.h"

class QtTestWindow : public QMainWindow
{
    Q_OBJECT

public:
    QtTestWindow(QWidget *parent = 0);
    ~QtTestWindow();

private:
    Ui::QtTestWindowClass ui;
    TestWidget * _w;
};

#endif // QTTESTWINDOW_H
