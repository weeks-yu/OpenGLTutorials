#include <gl/glut.h>
#include "meshwidget.h"

MeshWidget::MeshWidget(QWidget *parent)
    : QGLWidget(parent)
{
    setWindowTitle(tr("4. Mesh"));
    setMinimumSize(200, 200);
    setMouseTracking(true);
    setFocusPolicy(Qt::ClickFocus);

    // load mesh
    loadMesh(tr(OPENGL_TUTORIALS_DATA_PATH"/dragon-10000.smf"));

    // initialize model matrix data
    _modelMatrix.setToIdentity();
    _modelMatrix.scale(1.5);
    _modelMatrix.rotate(45, 1, 1, 1);
}

MeshWidget::~MeshWidget()
{}


void MeshWidget::initializeGL()
{
    makeCurrent();
}

void MeshWidget::paintGL()
{
    makeCurrent();

    qglClearColor(Qt::white);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);
    
    // set model-view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, -1000, 0, 0, 0, 0, -1, 0); // now model-view matrix = lookAt(...)
    glMultMatrixf(_modelMatrix.data()); // now model-view matrix = lookAt(...) * _modelMatrix

    // set projection matrix 
    // (projection matrix is often set in resizeGL(width, height) functions, 
    //  since it only relies on the size of the window in most cases)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30, (double)width()/height(), 0.01, 1e5); // now projection matrix = perspective(...)

    // draw mesh
    glBegin(GL_TRIANGLES);
    for(int vid : _triangleIndices)
    {
        glColor3fv(&(_vertices[vid].normal[0]));
        glVertex3fv(&(_vertices[vid].position[0]));
    }
    glEnd();    


    glDisable(GL_DEPTH_TEST);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
} 

void MeshWidget::resizeGL( int w, int h )
{
    glViewport(0, 0, w, h);
}

void MeshWidget::mousePressEvent( QMouseEvent * e )
{
    _lastMousePos = e->pos();
    setCursor(Qt::OpenHandCursor);
}

void MeshWidget::mouseMoveEvent( QMouseEvent * e )
{
    if(e->buttons() != Qt::NoButton)
    {
        setCursor(Qt::ClosedHandCursor);
        auto t = (e->pos() - _lastMousePos);
        QMatrix4x4 rotMat;
        rotMat.rotate(-t.x() / 10.0 * M_PI, 0, 1, 0);
        rotMat.rotate(t.y() / 10.0 * M_PI, 1, 0, 0);
        _modelMatrix = rotMat * _modelMatrix;
        _lastMousePos = e->pos();
        update();
    }
}

void MeshWidget::mouseReleaseEvent( QMouseEvent * e )
{
    setCursor(Qt::ArrowCursor);
}

void MeshWidget::wheelEvent( QWheelEvent * e )
{
    _modelMatrix.scale(exp(e->delta() / 1000.0));
    update();
}

void MeshWidget::loadMesh( const QString & f )
{
    _vertices.clear();
    _triangleIndices.clear();
    QFile file(f);
    if(file.open(QFile::ReadOnly))
    {
        QTextStream ts(&file);
        ts.readLine();
        QString token;
        while(true)
        {
            ts >> token;
            if(token == "v")
            {
                double x, y, z;
                ts >> x >> y >> z;
                Vertex v = {QVector3D(x, y, z), QVector3D(0, 0, 0)};
                _vertices << v;
            }else if (token == "f")
            {
                int a, b, c;
                ts >> a >> b >> c;
                _triangleIndices << (a-1) << (b-1) << (c-1);
                QVector3D normal = QVector3D::crossProduct(
                    _vertices[a-1].position - _vertices[b-1].position, 
                    _vertices[c-1].position - _vertices[b-1].position).normalized();
                _vertices[a-1].normal += normal;
                _vertices[b-1].normal += normal;
                _vertices[c-1].normal += normal;
            }else
            {
                break;
            }
        }
        for(auto & v : _vertices)
        {
            v.normal = -v.normal.normalized();
        }
    }
}
