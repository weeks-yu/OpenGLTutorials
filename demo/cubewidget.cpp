#include "cubewidget.h"

CubeWidget::CubeWidget(QWidget *parent)
    : QGLWidget(parent)
{
    setWindowTitle(tr("2. Cube"));
    setMinimumSize(200, 200);
    setMouseTracking(true);
    setFocusPolicy(Qt::ClickFocus);

    // initialize model matrix data
    _modelMatrix.setToIdentity();
    _modelMatrix.scale(100);
    _modelMatrix.rotate(45, 1, 1, 1);
}

CubeWidget::~CubeWidget()
{}

void CubeWidget::initializeGL()
{
    makeCurrent();
}

void CubeWidget::paintGL()
{
    qglClearColor(Qt::white);
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

    //   4 --- 7
    //  /|    /|
    // 5 --- 6 |
    // | 0 --| 3
    // |/    |/
    // 1 --- 2

    static const int lines[][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0},
        {5, 6}, {6, 7}, {7, 4}, {4, 5},
        {1, 5}, {2, 6}, {3, 7}, {0, 4},
        {5, 7}, {7, 2}, {5, 2}, {1, 4}, {4, 3}, {1, 3}
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
        {0, 0, 0, 0.8},
        {1, 1, 0, 0.8},
        {1, 0, 1, 0.8},
        {0, 1, 0, 0.8},
        {0, 1, 1, 0.8},
        {0, 0, 1, 0.8}
    };

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 6; i++) {
        glColor4dv(faceColors[i]);
        glVertex4dv(verts[quadFaces[i][0]]);
        glVertex4dv(verts[quadFaces[i][1]]);
        glVertex4dv(verts[quadFaces[i][2]]);
        glColor4d(faceColors[i][0]/2, faceColors[i][1]/2, faceColors[i][2]/2, 0.8);
        glVertex4dv(verts[quadFaces[i][0]]);
        glVertex4dv(verts[quadFaces[i][2]]);
        glVertex4dv(verts[quadFaces[i][3]]);
    }
    glEnd(); 

    //glLineWidth(1.0f);
    //glBegin(GL_LINES);
    //for (int i = 0; i < 18; i++) {
    //    glColor4d(0.5, 0.5, 0.5, 0.5);
    //    glVertex4dv(verts[lineIndices[i][0]]);
    //    glVertex4dv(verts[lineIndices[i][1]]);
    //}
    //glEnd();


}

void CubeWidget::resizeGL( int w, int h )
{
    glViewport(0, 0, w, h);
}

void CubeWidget::mousePressEvent( QMouseEvent * e )
{
    _lastMousePos = e->pos();
    setCursor(Qt::OpenHandCursor);
}

void CubeWidget::mouseMoveEvent( QMouseEvent * e )
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

void CubeWidget::mouseReleaseEvent( QMouseEvent * e )
{
    setCursor(Qt::ArrowCursor);
}

void CubeWidget::wheelEvent( QWheelEvent * e )
{
    _modelMatrix.scale(exp(e->delta() / 1000.0));
    update();
}