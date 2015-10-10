#include "qttestwindow.h"

QtTestWindow::QtTestWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    _w = new TestWidget(this);
    setCentralWidget(_w);
}

QtTestWindow::~QtTestWindow()
{

}
