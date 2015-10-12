#ifndef PAINT2DSIMPLEWIDGET_H
#define PAINT2DSIMPLEWIDGET_H

#include <QGLWidget>

class Paint2DSimpleWidget : public QGLWidget
{
public:
    Paint2DSimpleWidget(QWidget *parent = nullptr);
    ~Paint2DSimpleWidget();

protected:
    // opengl methods
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int w, int h) override;

private:
    
};

#endif // PAINT2DSIMPLEWIDGET_H
