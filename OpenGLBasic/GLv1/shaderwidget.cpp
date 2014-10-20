#include "shaderwidget.h"

ShaderWidget::ShaderWidget(QWidget *parent)
	: QGLWidget(parent), QGLFunctions()
{
	setWindowTitle(tr("7. Shader"));
	setMinimumSize(200, 200);
	setMouseTracking(true);
	setFocusPolicy(Qt::ClickFocus);

	// load mesh
	loadMesh(tr("../data/dragon-10000.smf"));

	// initialize model matrix data
	_modelMatrix.setToIdentity();
	_modelMatrix.scale(1.5);
	_modelMatrix.rotate(45, 1, 1, 1);

	_vertBuffer = 0;
	_triangleIndicesBuffer = 0;

	_program = -1;
	_modelMatrixLocation = -1;
	_viewMatrixLocation = -1;
	_projectionMatrixLocation = -1;
}

ShaderWidget::~ShaderWidget()
{

}


static const char * vshaderSource = 
	"#version 120\n"
	"attribute lowp vec3 position;\n"
	"attribute lowp vec3 normal;\n"
	"uniform lowp mat4 viewMatrix;\n"
	"uniform lowp mat4 modelMatrix;\n"
	"uniform lowp mat4 projectionMatrix;\n"
	"varying vec4 pixelColor;\n"
	"varying vec3 pixelPosition;\n"
	"void main(void)\n"
	"{\n"
	"    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);\n"
	"    pixelColor = vec4(normal, 1.0);\n"
	"	 vec4 pixelPosition4 = modelMatrix * vec4(position, 1.0);\n"
	"    pixelPosition = pixelPosition4.xyz / pixelPosition4.w;\n"
	"}\n";

static const char * fshaderSource = 
	"#version 120\n"
	"varying vec4 pixelColor;\n"
	"varying vec3 pixelPosition;\n"
	"void main(void)\n"
	"{\n"
	"	 vec3 center = vec3(1.0, 0.0, 0.0);\n"
	"    float distance = length(center - pixelPosition);\n"
	///"    gl_FragColor = pixelColor;\n"
	"    gl_FragColor = pixelColor * sin(distance);\n"
	"}\n";


void ShaderWidget::initializeGL()
{
	makeCurrent();
	initializeGLFunctions(context());

	// create program
	_program = glCreateProgram();

	// create vertex shader
	int vshader = glCreateShader(GL_VERTEX_SHADER);
	// set shader source and compile
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

	// create fragment shader
	int fshader = glCreateShader(GL_FRAGMENT_SHADER);
	// set shader source and compile
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

	// attach shaders to program and link program
	glAttachShader(_program, vshader);
	glAttachShader(_program, fshader);
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

	// get location of uniform values
	_modelMatrixLocation = glGetUniformLocation(_program, "modelMatrix");
	_viewMatrixLocation = glGetUniformLocation(_program, "viewMatrix");
	_projectionMatrixLocation = glGetUniformLocation(_program, "projectionMatrix");

	Q_ASSERT(_modelMatrixLocation != -1 && _viewMatrixLocation != -1 && _projectionMatrixLocation != -1);

	// bind 0 to position
	glBindAttribLocation(_program, 0, "position");
	// bind 1 to normal
	glBindAttribLocation(_program, 1, "normal");
	{
		GLenum e = glGetError();
		Q_ASSERT(e == GL_NO_ERROR);
	}


	// generate buffers
	glGenBuffers(1, &_vertBuffer);
	glGenBuffers(1, &_triangleIndicesBuffer);

	// fill buffers with data
	glBindBuffer(GL_ARRAY_BUFFER, _vertBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices.first()) * _vertices.size(), 
		_vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _triangleIndicesBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_triangleIndices.first()) * _triangleIndices.size(),
		_triangleIndices.data(), GL_STATIC_DRAW);	

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void ShaderWidget::paintGL()
{
	QPainter painter;
	painter.begin(this);

	painter.beginNativePainting();
	qglClearColor(Qt::black);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_SMOOTH);

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
	projectionMatrix.perspective(30, (double)width()/height(), 0.01, 1e5);
	glUniformMatrix4fv(_projectionMatrixLocation, 1, GL_FALSE, projectionMatrix.data());

	// set vertex attrib pointer to these buffer
	glBindBuffer(GL_ARRAY_BUFFER, _vertBuffer);
	glEnableVertexAttribArray(0); // 0 is already bound to position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(_vertices.first()), 0);
	glEnableVertexAttribArray(1); // 1 is already bound to normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(_vertices.first()), (void*)(3 * sizeof(float)));

	// draw mesh
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _triangleIndicesBuffer);
	glDrawElements(GL_TRIANGLES, _triangleIndices.size(), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// restore all native states
	painter.endNativePainting();
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
