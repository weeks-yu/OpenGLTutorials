#include <glut.h>

#include "basicwidget.h"

BasicWidget1::BasicWidget1(QWidget *parent)
	: QGLWidget(parent)
{
}

BasicWidget1::~BasicWidget1()
{

}

void BasicWidget1::initializeGL()
{

}

void BasicWidget1::paintGL()
{
	// clear background
	glClearColor(0.0, 0.0, 0.0, 0.0); // use black
	//glClearColor(1.0, 1.0, 1.0, 1.0); // use white
	glClear(GL_COLOR_BUFFER_BIT);

	// draw a polygon
	glBegin(GL_POLYGON);
	glColor3f(1, 0, 0); // use red
	glVertex2f(0.0, 1.0);
	glColor3f(0, 1, 0); // use green
	glVertex2f(-1.0, 0.0);
	glColor3f(0, 0, 1); // use blue
	glVertex2f(0.0, -1.0);
	glColor3f(1, 1, 0); // use yellow
	glVertex2f(1.0, 0.0);
	glEnd();	
}

void BasicWidget1::resizeGL( int w, int h )
{
	// view port
	glViewport(0, 0, w, h);
}






void DrawRectangleWithAxis()
{

	// draw a teapot
	glutSolidTeapot(0.4);

	// draw a rectangle
	glBegin(GL_QUADS);
	glColor3f(1, 0, 0); // use red
	glVertex2f(0.0, 0.0);
	glColor3f(0, 1, 0); // use green
	glVertex2f(0.5, 0.0);
	glColor3f(0, 0, 1); // use blue
	glVertex2f(0.5, 0.5);
	glColor3f(1, 1, 0); // use yellow
	glVertex2f(0.0, 0.5);
	glEnd();

	// draw axis
	glLineWidth(5);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex2f(0, 0);
	glVertex2f(1, 0);
	glColor3f(0, 0, 1);
	glVertex2f(0, 0);
	glVertex2f(0, 1);
	glEnd();

	// draw original point
	glPointSize(10);
	glBegin(GL_POINTS);
	glColor3f(0, 0, 0);
	glVertex2f(0, 0);
	glEnd();

}







BasicWidget2::BasicWidget2(QWidget *parent)
	: QGLWidget(parent)
{
	setWindowTitle(tr("Original"));
}

BasicWidget2::~BasicWidget2()
{

}

void BasicWidget2::initializeGL()
{

}

void BasicWidget2::paintGL()
{
	// clear background
	glClearColor(1.0, 1.0, 1.0, 1.0); // white
	glClear(GL_COLOR_BUFFER_BIT);

	DrawRectangleWithAxis();
}

void BasicWidget2::resizeGL( int w, int h )
{
	// view port
	glViewport(0, 0, w, h);
}




BasicWidget2_1::BasicWidget2_1(QWidget *parent)
	: QGLWidget(parent)
{
	setWindowTitle(tr("Translated (0.4, 0)"));
}

BasicWidget2_1::~BasicWidget2_1()
{

}

void BasicWidget2_1::initializeGL()
{

}

void BasicWidget2_1::paintGL()
{
	// clear background
	glClearColor(1.0, 1.0, 1.0, 1.0); // white
	glClear(GL_COLOR_BUFFER_BIT);


	// set model matrix
	glMatrixMode(GL_MODELVIEW); // adjust the model-view matrix
	glLoadIdentity(); // set to identity

	glTranslatef(0.4, 0, 0);

	DrawRectangleWithAxis();
}

void BasicWidget2_1::resizeGL( int w, int h )
{
	// view port
	glViewport(0, 0, w, h);
}



BasicWidget2_2::BasicWidget2_2(QWidget *parent)
	: QGLWidget(parent)
{
	setWindowTitle(tr("Rotated (30 degree CCW)"));
}

BasicWidget2_2::~BasicWidget2_2()
{

}

void BasicWidget2_2::initializeGL()
{

}

void BasicWidget2_2::paintGL()
{
	// clear background
	glClearColor(1.0, 1.0, 1.0, 1.0); // white
	glClear(GL_COLOR_BUFFER_BIT);


	// set model matrix
	glMatrixMode(GL_MODELVIEW); // adjust the model-view matrix
	glLoadIdentity(); // set to identity

	glRotatef(30, 0, 0, 1);


	DrawRectangleWithAxis();
}

void BasicWidget2_2::resizeGL( int w, int h )
{
	// view port
	glViewport(0, 0, w, h);
}




BasicWidget2_3::BasicWidget2_3(QWidget *parent)
	: QGLWidget(parent)
{
	setWindowTitle(tr("Rotate (30 degree CCW), then Translate (0.4, 0)"));
}

BasicWidget2_3::~BasicWidget2_3()
{

}

void BasicWidget2_3::initializeGL()
{

}

void BasicWidget2_3::paintGL()
{
	// clear background
	glClearColor(1.0, 1.0, 1.0, 1.0); // white
	glClear(GL_COLOR_BUFFER_BIT);

	// set model matrix
	glMatrixMode(GL_MODELVIEW); // adjust the model-view matrix
	glLoadIdentity(); // set to identity

	glTranslatef(0.4, 0, 0);
	glRotatef(30, 0, 0, 1);

	DrawRectangleWithAxis();
}

void BasicWidget2_3::resizeGL( int w, int h )
{
	// view port
	glViewport(0, 0, w, h);
}



BasicWidget2_4::BasicWidget2_4(QWidget *parent)
	: QGLWidget(parent)
{
	setWindowTitle(tr("Translate (0.4, 0), then Rotate (30 degree CCW)"));
}

BasicWidget2_4::~BasicWidget2_4()
{

}

void BasicWidget2_4::initializeGL()
{

}

void BasicWidget2_4::paintGL()
{
	// clear background
	glClearColor(1.0, 1.0, 1.0, 1.0); // white
	glClear(GL_COLOR_BUFFER_BIT);

	// set model matrix
	glMatrixMode(GL_MODELVIEW); // adjust the model-view matrix
	glLoadIdentity(); // set to identity

	glRotatef(30, 0, 0, 1);
	glTranslatef(0.4, 0, 0);

	DrawRectangleWithAxis();
}

void BasicWidget2_4::resizeGL( int w, int h )
{
	// view port
	glViewport(0, 0, w, h);
}