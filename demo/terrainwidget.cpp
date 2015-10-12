#include "terrainwidget.h"

TerrainWidget::TerrainWidget(QWidget *parent)
    : QGLWidget(parent), QGLFunctions() 
{
    setWindowTitle(tr("6. Shader - Height Map"));
    setMinimumSize(200, 200);
    setMouseTracking(true);
    setFocusPolicy(Qt::ClickFocus);

    // initialize height ratio
    _heightRatio = 0.1f;
    prepare();

    // initialize model matrix data
    _modelMatrix.setToIdentity();
    _modelMatrix.scale(4);
    _modelMatrix.rotate(180, 1, 0, 0);
  

    // initialize buffer ids to 0
    _gridBuffer = 0;
    _triangleIndicesBuffer = 0;

    // initialize program id to -1
    _program = -1;

    // initialize uniform variable locations to -1
    _modelMatrixLocation = -1;
    _viewMatrixLocation = -1;
    _projectionMatrixLocation = -1;
    _normalHeightMapLocation = -1;
    _heightRatioLocation = -1;

    _texture = -1;
}


TerrainWidget::~TerrainWidget() {}

// the source code of vertex shader
static const char * vshaderSource =
    "#version 120\n"                    // the version of this shader
    "attribute lowp vec2 position;\n"   // the 2d position of each grid vertex
    "uniform lowp mat4 viewMatrix;\n"       // the viewMatrix of this shader program
    "uniform lowp mat4 modelMatrix;\n"      // the modelMatrix of this shader program
    "uniform lowp mat4 projectionMatrix;\n" // the projectionMatrix of this shader program
    "uniform sampler2D normalHeightMap;\n" // the normalHeightMap of this shader program
    "uniform lowp float heightRatio;\n" // the heightRatio of this shader program
    "varying lowp vec3 pixelNormal;\n"    // the output normal on this vertex (will be interpolated in fragment shader)
    "varying lowp vec3 pixelPosition;\n"  // the output position on this vertex (will be interpolated in fragment shader)
    "varying lowp float pixelHeight;\n"
    "void main(void)\n" // the main function
    "{\n"
    "    lowp vec4 normalHeight = texture2D(normalHeightMap, position);\n"
    "    lowp float height = normalHeight.a * heightRatio;\n"
    // get pixelNormal
    "    pixelNormal = normalize(normalHeight.rgb);\n"

    // gl_Position is the final coordinate of this vertex on screen
    "    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position * 2.0 - 1.0, height, 1.0);\n"  

    // get pixelPosition
    "    lowp vec4 position4 = viewMatrix * modelMatrix * vec4(position * 2.0 - 1.0, height, 1.0);\n"
    "    pixelPosition = position4.xyz / position4.w;\n"

    "    pixelHeight = normalHeight.a;\n"

    "}\n";

// the source code of fragment shader
static const char * fshaderSource =
    "#version 120\n" // the version of this shader
    "varying lowp vec3 pixelNormal;\n"    // the input normal on this pixel
    "varying lowp vec3 pixelPosition;\n"  // the input position of this pixel
    "varying lowp float pixelHeight;\n"
    "void main(void)\n" // the main function
    "{\n"

    "    gl_FragColor = vec4(pixelNormal * pixelHeight, 1.0);\n"

    "}\n";


void TerrainWidget::initializeGL() 
{
    makeCurrent();
    initializeGLFunctions(context());

    // create OpenGL shader program
    _program = glCreateProgram();

    // create a vertex shader
    int vshader = glCreateShader(GL_VERTEX_SHADER);

    // load shader source and compile
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
    glShaderSource(fshader, 1, &fshaderSource, 0);
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

    // now we deal with the GL_TEXTURE0 group only 
    // (one texture group contains multiple kinds of textures, eg. GL_TEXTURE_1D, GL_TEXTURE_2D...)
    // see https://www.opengl.org/discussion_boards/showthread.php/174926-when-to-use-glActiveTexture for explanation
    glActiveTexture(GL_TEXTURE0);
    // this functon wraps two tasks:
    // 1. we create a new texture object whose data comes from the _normalHeightMap, 
    //    and the name of the texture object is returned as '_texture';
    // 2. we bind the object '_texture' onto the GL_TEXTURE_2D part within the GL_TEXTURE0 group
    // related gl calls include:
    //  glCreateTextures, glBindTexture, glTexImage2D
    _texture = bindTexture(_normalHeightMap, GL_TEXTURE_2D, GL_RGBA);



    // get locations of uniform variable from the linked program
    _modelMatrixLocation = glGetUniformLocation(_program, "modelMatrix");
    _viewMatrixLocation = glGetUniformLocation(_program, "viewMatrix");
    _projectionMatrixLocation = glGetUniformLocation(_program, "projectionMatrix");
    _normalHeightMapLocation = glGetUniformLocation(_program, "normalHeightMap");
    _heightRatioLocation = glGetUniformLocation(_program, "heightRatio");


    Q_ASSERT(_modelMatrixLocation != -1 && _viewMatrixLocation != -1 && 
        _projectionMatrixLocation != -1 && _normalHeightMapLocation != -1 &&
        _heightRatioLocation != -1);


    // bind 0 to the "position" attribute of each vertex in shader program
    glBindAttribLocation(_program, 0, "position");


    // generate buffers
    glGenBuffers(1, &_gridBuffer);
    glGenBuffers(1, &_triangleIndicesBuffer);

    // use _vertBuffer as the ArrayBuffer and fill it with vertices array 
    glBindBuffer(GL_ARRAY_BUFFER, _gridBuffer);
   
    glBufferData(GL_ARRAY_BUFFER, sizeof(_grids.first()) * _grids.size(),
        _grids.data(), GL_STATIC_DRAW);

    // use _triangleIndicesBuffer as the ElementArrayBuffer and fill it with triangle indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _triangleIndicesBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_triangleIndices.first()) * _triangleIndices.size(),
        _triangleIndices.data(), GL_STATIC_DRAW);

    // unbind buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void TerrainWidget::paintGL() 
{
    qglClearColor(Qt::white);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // use the OpenGL shader program for painting
    glUseProgram(_program);



    //// set uniform values
    // set model matrix
    glUniformMatrix4fv(_modelMatrixLocation, 1, GL_FALSE, _modelMatrix.data());

    // set view matrix
    QMatrix4x4 viewMatrix;
    viewMatrix.setToIdentity();
    viewMatrix.lookAt(QVector3D(0, 0, -10), QVector3D(0, 0, 0), QVector3D(0, -1, 0));
    glUniformMatrix4fv(_viewMatrixLocation, 1, GL_FALSE, viewMatrix.data());

    // set projection matrix
    QMatrix4x4 projectionMatrix;
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(30, (float)width() / height(), 0.01f, 1e5f);
    glUniformMatrix4fv(_projectionMatrixLocation, 1, GL_FALSE, projectionMatrix.data());

    // bind _texture as GL_TEXTURE_2D in the texture group 0  
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture);
    // set normalHeightMap to 0 so that we can access the content of _texture via 'sampler2D' in the shader
    glUniform1i(_normalHeightMapLocation, 0);

    // set height ratio
    glUniform1f(_heightRatioLocation, _heightRatio);



    //// set attributes data
    // bind ArrayBuffer to _gridBuffer
    glBindBuffer(GL_ARRAY_BUFFER, _gridBuffer);
    // enable vertex attribute "position" (bound to 0 already)
    glEnableVertexAttribArray(0);
    // set the data of vertex attribute "position" using current ArrayBuffer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(_grids.first()), 0);   




    //// draw call
    // bind ElementArrayBuffer to _triangleIndicesBuffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _triangleIndicesBuffer);
    // draw mesh using the indices stored in ElementArrayBuffer
    glDrawElements(GL_TRIANGLES, _triangleIndices.size(), GL_UNSIGNED_INT, 0);



    //// finalize
    // disable vertex attributes
    glDisableVertexAttribArray(0);

    // unbind buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // restore states
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
}

void TerrainWidget::resizeGL(int w, int h) 
{
    glViewport(0, 0, w, h);
}

void TerrainWidget::mousePressEvent(QMouseEvent * e) {
    _lastMousePos = e->pos();
    setCursor(Qt::OpenHandCursor);
}

void TerrainWidget::mouseMoveEvent(QMouseEvent * e) 
{
    if (e->buttons() != Qt::NoButton) {
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

void TerrainWidget::mouseReleaseEvent(QMouseEvent * e) 
{
    setCursor(Qt::ArrowCursor);
}

void TerrainWidget::wheelEvent(QWheelEvent * e) 
{
    _modelMatrix.scale(exp(e->delta() / 1000.0));
    update();
}

void TerrainWidget::prepare() 
{
    // create grid data
    static const int resolution = 256;
    _grids.resize(resolution * resolution);
    for (int i = 0; i < resolution; i++) {
        for (int j = 0; j < resolution; j++) {
            _grids[i * resolution + j].setX(1.0f / resolution * i);
            _grids[i * resolution + j].setY(1.0f / resolution * j);
        }
    }
    // create triangle indices data
    for (int i = 0; i < resolution - 1; i++) {
        for (int j = 0; j < resolution - 1; j++) {
            int p1 = i * resolution + j;
            int p2 = (i + 1) * resolution + j;
            int p3 = (i + 1) * resolution + j + 1;
            int p4 = i * resolution + j + 1;
            _triangleIndices << p1 << p2 << p4;
            _triangleIndices << p2 << p3 << p4;
        }
    }

    // load the image
    QImage im(tr(":/images/australia.jpg"));
    // compute the normal and height map
    _normalHeightMap = QImage(im.width(), im.height(), QImage::Format_ARGB32);
    for (int i = 0; i < im.width(); i++) {
        for (int j = 0; j < im.height(); j++) {
            float height = qGray(im.pixel(i, j)) / 255.0f * _heightRatio;
            float adjHs[4] = { 
                i > 0 ? (qAlpha(_normalHeightMap.pixel(i - 1, j)) / 255.0f * _heightRatio) : height,
                j > 0 ? (qAlpha(_normalHeightMap.pixel(i, j - 1)) / 255.0f * _heightRatio) : height,
                i < im.width() - 1 ? (qAlpha(_normalHeightMap.pixel(i + 1, j)) / 255.0f * _heightRatio) : height,
                j < im.height() - 1 ? (qAlpha(_normalHeightMap.pixel(i, j + 1)) / 255.0f * _heightRatio) : height
            };
            float dxdy[][2] = { {-1.0f, 0.0f}, {0.0f, -1.0f}, {1.0f, 0.0f}, {0.0f, 1.0f} };
            QVector3D normal;
            for (int k = 0; k < 4; k++) {
                QVector3D v1(dxdy[k][0], dxdy[k][1], (adjHs[k] - height) * resolution);
                QVector3D v2(dxdy[(k + 1) % 4][0], dxdy[(k + 1) % 4][1], 
                    (adjHs[(k + 1) % 4] - height) * resolution);
                normal += QVector3D::crossProduct(v1, v2).normalized();
            }
            normal.normalize();
            QRgb normalHeight = qRgba(normal.x() * 255, normal.y() * 255, normal.z() * 255, qGray(im.pixel(i, j)));
            _normalHeightMap.setPixel(i, j, normalHeight);
        }
    }
}
