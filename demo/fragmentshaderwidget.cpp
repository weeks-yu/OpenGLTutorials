#include "fragmentshaderwidget.h

FragmentShaderWidget::FragmentShaderWidget(const QByteArray & fragmentShaderSource, QWidget *parent)
    : QGLWidget(parent), QGLFunctions(), _fragmentShaderSource(fragmentShaderSource)
{
    setWindowTitle(tr("9. Fragment Shader Only"));
    setMinimumSize(200, 200);
    setMouseTracking(true);
    setFocusPolicy(Qt::ClickFocus);

    // initialize buffer ids to 0
    _vertBuffer = 0;
    _triangleIndicesBuffer = 0;

    // initialize program id to -1
    _program = -1;

    // initialize uniform variable locations to -1
    _resolutionLocation = -1;
    _timeLocation = -1;
    _mouseLocation = -1;

	_timer = new QTimer(this);
	_timer->setInterval(20);
	_timer->setSingleShot(false);
	connect(_timer, SIGNAL(timeout()), this, SLOT(update()));
	_timeValue = 0u;
}

FragmentShaderWidget::~FragmentShaderWidget()
{}


void FragmentShaderWidget::initializeGL()
{
    makeCurrent();
    initializeGLFunctions(context());

    // create OpenGL shader program
    _program = glCreateProgram();

    // create a vertex shader
    int vshader = glCreateShader(GL_VERTEX_SHADER);

    // load shader source and compile


	const char * vshaderSource = 
		"attribute vec3 position;\n"
		"attribute vec2 surfacePosAttrib;\n"
		"varying vec2 surfacePosition;\n";
		"void main() {\n"
			"surfacePosition = surfacePosAttrib;\n"
			"gl_Position = vec4( position, 1.0 );\n"
		"}";

    glShaderSource(vshader, 1, &vshaderSource, 0);
    glCompileShader(vshader);       
    { // check status
        GLint logLength;
        glGetShaderiv(vshader, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            GLchar *log = (GLchar *)malloc(logLength);
            glGetShaderInfoLog(vshader, logLength, &logLength, log);
            qDebug("Shader compile log:\n%s", log);
            free(log);
        }
        GLint status;
        glGetShaderiv(vshader, GL_COMPILE_STATUS, &status);
        if (status == 0) {
            glDeleteShader(vshader);
            return;
        }
    }

    // create a fragment shader
    int fshader = glCreateShader(GL_FRAGMENT_SHADER);

    // load shader source and compile
    glShaderSource(fshader, 1, &_fragmentShaderSource.data(), 0);
    glCompileShader(fshader);
    { // check status
        GLint logLength;
        glGetShaderiv(fshader, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            GLchar *log = (GLchar *)malloc(logLength);
            glGetShaderInfoLog(fshader, logLength, &logLength, log);
            qDebug("Shader compile log:\n%s", log);
            free(log);
        }
        GLint status;
        glGetShaderiv(fshader, GL_COMPILE_STATUS, &status);
        if (status == 0) {
            glDeleteShader(fshader);
            return;
        }
    }

    // attach shaders to the program
    glAttachShader(_program, vshader);
    glAttachShader(_program, fshader);

    // link the program
    glLinkProgram(_program);
    { // check status
        GLint logLength, status;
        glValidateProgram(_program);
        glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            GLchar *log = (GLchar *)malloc(logLength);
            glGetProgramInfoLog(_program, logLength, &logLength, log);
            qDebug("Program validate log:\n%s", log);
            free(log);
        }

        glGetProgramiv(_program, GL_VALIDATE_STATUS, &status);
        if (status == 0) {
            return;
        }
    }

    // get locations of uniform variable from the linked program
    _modelMatrixLocation = glGetUniformLocation(_program, "modelMatrix");
    _viewMatrixLocation = glGetUniformLocation(_program, "viewMatrix");
    _projectionMatrixLocation = glGetUniformLocation(_program, "projectionMatrix");

    Q_ASSERT(_modelMatrixLocation != -1 && _viewMatrixLocation != -1 && _projectionMatrixLocation != -1);

    // bind 0 to the "position" attribute of each vertex in shader program
    glBindAttribLocation(_program, 0, "position");
    // bind 1 to the "normal" attribute of each vertex in shader program
    glBindAttribLocation(_program, 1, "normal");


    // generate buffers
    glGenBuffers(1, &_vertBuffer);
    glGenBuffers(1, &_triangleIndicesBuffer);

    // use _vertBuffer as the ArrayBuffer and fill it with vertices array 
    glBindBuffer(GL_ARRAY_BUFFER, _vertBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices.first()) * _vertices.size(), 
        _vertices.data(), GL_STATIC_DRAW);

    // use _triangleIndicesBuffer as the ElementArrayBuffer and fill it with triangle indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _triangleIndicesBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_triangleIndices.first()) * _triangleIndices.size(),
        _triangleIndices.data(), GL_STATIC_DRAW);   

    // unbind buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void ShaderWidget::paintGL()
{
    qglClearColor(Qt::white);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // use the OpenGL shader program for painting
    glUseProgram(_program);

    // set model matrix
    glUniformMatrix4fv(_modelMatrixLocation, 1, GL_FALSE, _modelMatrix.data());

    // set view matrix
    QMatrix4x4 viewMatrix;
    viewMatrix.setToIdentity();
    viewMatrix.lookAt(QVector3D(0, 0, -1000), QVector3D(0, 0, 0), QVector3D(0, -1, 0));
    glUniformMatrix4fv(_viewMatrixLocation, 1, GL_FALSE, viewMatrix.data());

    // set projection matrix
    QMatrix4x4 projectionMatrix;
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(30, (float)width()/height(), 0.01f, 1e5f);
    glUniformMatrix4fv(_projectionMatrixLocation, 1, GL_FALSE, projectionMatrix.data());

    // bind ArrayBuffer to _vertBuffer
    glBindBuffer(GL_ARRAY_BUFFER, _vertBuffer);
    // enable vertex attribute "position" (bound to 0 already)
    glEnableVertexAttribArray(0); 
    // set the data of vertex attribute "position" using current ArrayBuffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(_vertices.first()), 0);
    // enable vertex attribute "normal" (bound to 1 already)
    glEnableVertexAttribArray(1);
    // set the data of vertex attribute "normal" using current ArrayBuffer
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(_vertices.first()), (void*)(3 * sizeof(float)));

    // bind ElementArrayBuffer to _triangleIndicesBuffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _triangleIndicesBuffer);
    // draw mesh using the indices stored in ElementArrayBuffer
    glDrawElements(GL_TRIANGLES, _triangleIndices.size(), GL_UNSIGNED_INT, 0);

    // disable vertex attributes
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    // unbind buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // restore states
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
}

void ShaderWidget::resizeGL( int w, int h )
{
    glViewport(0, 0, w, h);
}

void ShaderWidget::mousePressEvent( QMouseEvent * e )
{
    _lastMousePos = e->pos();
    setCursor(Qt::OpenHandCursor);
}

void ShaderWidget::mouseMoveEvent( QMouseEvent * e )
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

void ShaderWidget::mouseReleaseEvent( QMouseEvent * e )
{
    setCursor(Qt::ArrowCursor);
}

void ShaderWidget::wheelEvent( QWheelEvent * e )
{
    _modelMatrix.scale(exp(e->delta() / 1000.0));
    update();
}

void ShaderWidget::loadMesh( const QString & f )
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
