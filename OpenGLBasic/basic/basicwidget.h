#ifndef BASICWIDGET_H
#define BASICWIDGET_H

#include <QtGui>
#include <QtOpenGL>
#include <QtWidgets>

class BasicWidget1 : public QGLWidget
{
	Q_OBJECT

public:
	BasicWidget1(QWidget *parent = 0);
	~BasicWidget1();

protected:
	// opengl methods
	virtual void initializeGL() override;
	virtual void paintGL() override;
	virtual void resizeGL(int w, int h) override;
};

class BasicWidget2 : public QGLWidget
{
	Q_OBJECT

public:
	BasicWidget2(QWidget *parent = 0);
	~BasicWidget2();

protected:
	// opengl methods
	virtual void initializeGL() override;
	virtual void paintGL() override;
	virtual void resizeGL(int w, int h) override;
};

class BasicWidget2_1 : public QGLWidget
{
	Q_OBJECT

public:
	BasicWidget2_1(QWidget *parent = 0);
	~BasicWidget2_1();

protected:
	// opengl methods
	virtual void initializeGL() override;
	virtual void paintGL() override;
	virtual void resizeGL(int w, int h) override;
};

class BasicWidget2_2 : public QGLWidget
{
	Q_OBJECT

public:
	BasicWidget2_2(QWidget *parent = 0);
	~BasicWidget2_2();

protected:
	// opengl methods
	virtual void initializeGL() override;
	virtual void paintGL() override;
	virtual void resizeGL(int w, int h) override;
};

class BasicWidget2_3 : public QGLWidget
{
	Q_OBJECT

public:
	BasicWidget2_3(QWidget *parent = 0);
	~BasicWidget2_3();

protected:
	// opengl methods
	virtual void initializeGL() override;
	virtual void paintGL() override;
	virtual void resizeGL(int w, int h) override;
};

class BasicWidget2_4 : public QGLWidget
{
	Q_OBJECT

public:
	BasicWidget2_4(QWidget *parent = 0);
	~BasicWidget2_4();

protected:
	// opengl methods
	virtual void initializeGL() override;
	virtual void paintGL() override;
	virtual void resizeGL(int w, int h) override;
};







#endif // BASICWIDGET_H
