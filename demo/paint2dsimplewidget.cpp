#include <gl/glut.h>
#include "paint2dsimplewidget.h"

Paint2DSimpleWidget::Paint2DSimpleWidget(QWidget *parent)
	: QGLWidget(parent)
{
	setWindowTitle(tr("1. Paint2D Simple"));
	setMinimumSize(200, 200);
}

Paint2DSimpleWidget::~Paint2DSimpleWidget()
{}

void Paint2DSimpleWidget::initializeGL()
{
	makeCurrent();
}

void Paint2DSimpleWidget::paintGL()
{
	// clear background
	glClearColor(1.0, 1.0, 1.0, 1.0); // use white
	glClear(GL_COLOR_BUFFER_BIT);

	// modify the model-view matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();			// now model-view-matrix = I
	//glScalef(2, 1, 1);		// now model-view-matrix = old-model-view-matrix * scalef(...)
	//glTranslatef(0, 0.5, 0);	// now model-view-matrix = old-model-view-matrix * translatef(...)
	glRotatef(45, 0, 0, 1);		// now model-view-matrix = old-model-view-matrix * rotatef(...)


	// draw a rectangle
	glBegin(GL_QUADS);
	glColor3f(1, 0, 0); // use red
	glVertex2f(0.0, 0.0);
	glColor3f(0, 1, 0); // use green
	glVertex2f(0.2, 0.0);
	glColor3f(0, 0, 1); // use blue
	glVertex2f(0.2, 0.2);
	glColor3f(1, 1, 0); // use yellow
	glVertex2f(0.0, 0.2);
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

	// draw a point
	glPointSize(10);
	glBegin(GL_POINTS);
	glColor3f(0, 0, 0);
	glVertex2f(0, 0);
	glEnd();
}

void Paint2DSimpleWidget::resizeGL( int w, int h )
{
	glViewport(0, 0, w, h);
}
