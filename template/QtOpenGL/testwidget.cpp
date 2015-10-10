#include "testwidget.h"

TestWidget::TestWidget(QWidget *parent)
    : QGLWidget(parent)
{

}

TestWidget::~TestWidget()
{

}

void TestWidget::initializeGL()
{

}

void TestWidget::paintGL()
{
    glClearColor(1, 0, 0, 1); //red
    glClear(GL_COLOR_BUFFER_BIT);

    // draw a rectangle
    glBegin(GL_QUADS);
    glColor3f(1, 0, 0); // use red
    glVertex2f(0.0, 0.0);
    glColor3f(0, 1, 0); // use green
    glVertex2f(0.8, 0.0);
    glColor3f(0, 0, 1); // use blue
    glVertex2f(0.8, 0.8);
    glColor3f(1, 1, 0); // use yellow
    glVertex2f(0.0, 0.8);
    glEnd();
}

void TestWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}
