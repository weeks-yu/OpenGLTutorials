#include "transformwidget.h"

TransformWidget::TransformWidget(QWidget *parent)
    : QGLWidget(parent)
{
    setWindowTitle(tr("2. Model Transform"));
    setMinimumSize(200, 200);
    setMouseTracking(true);
    setFocusPolicy(Qt::ClickFocus);

    // initialize model matrix data
    _modelMatrix.setToIdentity();
    _modelMatrix.scale(100);
    _modelMatrix.rotate(45, 1, 1, 1);
}

TransformWidget::~TransformWidget()
{}

void TransformWidget::initializeGL()
{
    makeCurrent();
}

void TransformWidget::paintGL()
{
    qglClearColor(Qt::gray);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // setup model matrix (ignore the "view matrix" since it is identity here)
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(_modelMatrix.data());

    // setup projection matrix
    glMatrixMode(GL_PROJECTION);
    QMatrix4x4 projectionMatrix;
    projectionMatrix.setToIdentity();
    projectionMatrix.ortho(-width()/2.0, width()/2.0, -height()/2.0, height()/2.0, -1e3, 1e3);
    glLoadMatrixf(projectionMatrix.data());

    // draw a cube
    static const double verts[8][4] = {
        { -1, -1, -1, 1 },
        { 1, -1, -1, 1 },
        { 1, 1, -1, 1 },
        { -1, 1, -1, 1 },
        { -1, -1, 1, 1 },
        { 1, -1, 1, 1 },
        { 1, 1, 1, 1 },
        { -1, 1, 1, 1 }
    };

    static const int quadFaces[6][4] = {
        { 0, 1, 5, 4 },
        { 4, 5, 6, 7 },
        { 1, 2, 6, 5 },
        { 0, 4, 7, 3 },
        { 2, 3, 7, 6 },
        { 1, 0, 3, 2 }
    };

    static const double faceColors[6][4] = {
        {1, 1, 1, 1},
        {1, 1, 0, 0.8},
        {1, 0, 1, 0.8},
        {0, 1, 0, 0.8},
        {0, 1, 1, 0.8},
        {0, 0, 1, 0.8}
    };

    glBegin(GL_QUADS);
    for (int i = 0; i < 6; i++) {
        glColor4dv(faceColors[i]);

        glVertex4dv(verts[quadFaces[i][0]]);
        glVertex4dv(verts[quadFaces[i][1]]);
        glVertex4dv(verts[quadFaces[i][2]]);
        glVertex4dv(verts[quadFaces[i][3]]);
    }
    glEnd();    
}

void TransformWidget::resizeGL( int w, int h )
{
    glViewport(0, 0, w, h);
}

void TransformWidget::mousePressEvent( QMouseEvent * e )
{
    _lastMousePos = e->pos();
    setCursor(Qt::OpenHandCursor);
}

void TransformWidget::mouseMoveEvent( QMouseEvent * e )
{
    if(e->buttons() != Qt::NoButton)
    {
        setCursor(Qt::ClosedHandCursor);
        auto t = (e->pos() - _lastMousePos);
        QMatrix4x4 rotMat;
        rotMat.rotate(t.x() / 10.0 * M_PI, 0, 1, 0);
        rotMat.rotate(t.y() / 10.0 * M_PI, 1, 0, 0);
        _modelMatrix = rotMat * _modelMatrix;
        _lastMousePos = e->pos();
        update();
    }
}

void TransformWidget::mouseReleaseEvent( QMouseEvent * e )
{
    setCursor(Qt::ArrowCursor);
}

void TransformWidget::wheelEvent( QWheelEvent * e )
{
    _modelMatrix.scale(exp(e->delta() / 1000.0));
    update();
}