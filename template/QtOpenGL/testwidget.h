#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QGLWidget>

class TestWidget : public QGLWidget
{
	Q_OBJECT

public:
	TestWidget(QWidget *parent);
	~TestWidget();

protected:
	virtual void initializeGL() override;
	virtual void paintGL() override;
	virtual void resizeGL(int w, int h) override;

};

#endif // TESTWIDGET_H
