#include "Extra_Head_files/libs.h"
#include "MyWidget.h"

MyWidget::MyWidget(QWidget* parent)
	: QOpenGLWidget(parent), 
	m_program(0), m_shader(0), m_texture(0), 
	xRot(0), yRot(0), zRot(0), x(0), y(0), sx(1),sy(1)
{
	setFocusPolicy(Qt::StrongFocus);
}

MyWidget::~MyWidget()
{
	makeCurrent();

	//delete m_texture;
	//delete m_shader;
	//delete m_program;

	m_vbo.destroy();
	m_vao.destroy();

	doneCurrent();
}

QString loadShaderSource(char* fileName)
{
	std::string temp = "";                                                      //To get each line
	std::string src = "";                                                       //To get the sources

	std::ifstream in_file;

	//1. vertex
	in_file.open(fileName);

	if (in_file.is_open())
	{
		while (std::getline(in_file, temp))
			src += temp + "\n";
	}
	else
	{
		std::cout << "ERROR::SHADER::COULUD_NOT_OPEN_FILE" << fileName << "\n";
	}

	in_file.close();
	return QString::fromStdString(src);

}

void MyWidget::initializeGL()
{
	QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();   // represents a native OpenGL context, enabling OpenGL rendering on a QSurface.
	
	f->glClearColor(0, 0, 0, 1);  //init() part
	f->glEnable(GL_DEPTH_TEST);

	f->glFrontFace(GL_CCW);
	f->glEnable(GL_BLEND);
	f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	f->glEnable(GL_LIGHTING);
	f->glEnable(GL_LIGHT0);
	f->glEnable(GL_COLOR_MATERIAL);
	f->glEnable(GL_TEXTURE_2D);


	//-----------texture---------//
	glEnable(GL_TEXTURE0);
	QImage img("Images/man.png");
	Q_ASSERT(!img.isNull());
	m_texture = new QOpenGLTexture(img);  //m_texture = new QOpenGLTexture(img.scaled(32, 36).mirrored());
	//----------------------------//


	//-----------Shader-----------//
	//QOpenGLShader vertexshader(QOpenGLShader::Vertex);
	//shadersource = loadShaderSource("Vertex.vert");
	//vertexshader.compileSourceCode(shadersource);
	//
	//QOpenGLShader fragmentshader(QOpenGLShader::Fragment);
	//shadersource = loadShaderSource("Fragment.frag");
	//fragmentshader.compileSourceCode(shadersource);
	//----------------------------//


	//-----------program---------//
	m_program = new QOpenGLShaderProgram;

	//m_program->addShader(&vertexshader);
	//m_program->addShader(&fragmentshader);

	m_program->addShaderFromSourceFile(QOpenGLShader::Vertex,
		"Vertex.vert");
	m_program->addShaderFromSourceFile(QOpenGLShader::Fragment,
		"Fragment.frag");
	
	m_program->link();

	m_modelMatrixLoc = m_program->uniformLocation("ModelMatrix");
	m_viewMatrixLoc = m_program->uniformLocation("ViewMatrix");
	m_projectionMatrixLoc = m_program->uniformLocation("ProjectionMatrix");
	m_lightPosLoc = m_program->uniformLocation("LightPos0");
	//----------------------------//


	//-----------VAO/VBO-----------//
	float arrVertex[] = {
		//   position                 color            //texcoord      normal
		-0.5f, 0.5f, 0.0f,       0.0f, 0.f, 1.f,     0.f, 1.f,      0.0f, 0.f, 1.f,
		-0.5f, -0.5f, 0.0f,      1.0f, 1.f, 1.f,     0.f, 0.f,      0.0f, 0.f, 1.f,
		 0.5f, -0.5f, 0.0f,     1.0f, 1.f, 1.f,     1.f, 0.f,      0.0f, 0.f, 1.f,
		 0.5f, 0.5f, 0.0f,      1.0f, 0.f, 1.f,     1.f, 1.f,      0.0f, 0.f, 1.f
	};

	m_vao.create();
	if (m_vao.isCreated())
		m_vao.bind();
	m_program->bind();
	m_vbo.create();
	m_vbo.bind();
	m_vbo.allocate(arrVertex, sizeof(arrVertex));

	//f->glEnableVertexAttribArray(0);
	//f->glEnableVertexAttribArray(1);
	//f->glEnableVertexAttribArray(2);
	//f->glEnableVertexAttribArray(3);
	//f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), 0);
	//f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat),
	//	reinterpret_cast<void*>(3 * sizeof(GLfloat)));
	//f->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat),
	//	reinterpret_cast<void*>(6 * sizeof(GLfloat)));
	//f->glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat),
	//	reinterpret_cast<void*>(8 * sizeof(GLfloat)));
	
	m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(float) * 11);
	m_program->enableAttributeArray(0);
	
	m_program->setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 3, sizeof(float) * 11);
	m_program->enableAttributeArray(1);
	
	m_program->setAttributeBuffer(2, GL_FLOAT, 6 * sizeof(float), 2, sizeof(float) * 11);
	m_program->enableAttributeArray(2);
	
	m_program->setAttributeBuffer(3, GL_FLOAT, 8 * sizeof(float), 3, sizeof(float) * 11);
	m_program->enableAttributeArray(3);

		
	m_vbo.release();

	//----------------------------//

}

void MyWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	m_proj.setToIdentity();
	m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);

	//m_proj.ortho(-w/2, w/2, -h/2, h/2, 0.1f, 100.0f);

	m_uniformsDirty = true;
}


void MyWidget::paintGL()     //inside of while loop
{
	QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();
	
	f->glClearColor(0, 1, 0, 1);
	f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//f->glEnable(texture0);

	m_program->bind();
	m_texture->bind();

	if (m_uniformsDirty) {
		m_uniformsDirty = false;
		QMatrix4x4 camera;
		camera.lookAt(m_eye, m_eye + m_target, QVector3D(0, 1, 0));
		m_program->setUniformValue(m_projectionMatrixLoc, m_proj);
		m_program->setUniformValue(m_viewMatrixLoc, camera);
		QMatrix4x4 mx = m_world;                                                  //Make an identity model matrix
		mx.translate(x,y);
		mx.rotate(xRot, 1.f, 0.f, 0.f);
		mx.rotate(yRot, 0.f, 1.f, 0.f);
		mx.rotate(zRot, 0.f, 0.f, 1.f);
		mx.scale(sx/sy,sx/sy);
		m_program->setUniformValue(m_modelMatrixLoc, mx);

		m_program->setUniformValue(m_lightPosLoc, QVector3D(0, 0, 2));
	}

	m_vbo.bind();
	f->glDrawArrays(GL_QUADS, 0, 4);
	m_vbo.release();
	m_program->release();

}	

void MyWidget::rotatex(int angle)
{
	if (angle != xRot) {
		xRot = angle;
		emit xRotationChanged(angle);
		m_uniformsDirty = true;
		update();
	}
}

void MyWidget::rotatey(int angle)
{
	if (angle != yRot) {
		yRot = angle;
		emit yRotationChanged(angle);
		m_uniformsDirty = true;
		update();
	}
}

void MyWidget::rotatez(int angle)
{
	if (angle != zRot) {
		zRot = angle;
		emit zRotationChanged(angle);
		m_uniformsDirty = true;
		update();
	}
}

void MyWidget::redraw()
{
	xRot = 0;
	yRot = 0;
	zRot = 0;
	x = 0;
	y = 0;
	sx = sy = 1;
	emit xRotationChanged(0);
	emit yRotationChanged(0);
	emit zRotationChanged(0);
	m_uniformsDirty = true;
	update();
}

void MyWidget::keyPressEvent(QKeyEvent* e) //Keyboard interaction
{
	if (e->key() == Qt::Key_S) {
		y -= 0.01f; m_uniformsDirty = true; update();
	}
	if (e->key() == Qt::Key_W) {
		y += 0.01f; m_uniformsDirty = true; update();
	}
	if (e->key() == Qt::Key_A) {
		x -= 0.01f; m_uniformsDirty = true; update();
	}
	if (e->key() == Qt::Key_D) {
		x += 0.01f; m_uniformsDirty = true; update();
	}
	if (e->key() == Qt::Key_Q) {
		yRot -= 2.0f; m_uniformsDirty = true; update(); 
	}
	if (e->key() == Qt::Key_E) {
		yRot += 2.0f; m_uniformsDirty = true; update();
	}
	if (e->key() == Qt::Key_X) {
		sx += 0.05f; m_uniformsDirty = true; update();
	}
	if (e->key() == Qt::Key_Z) {
		sy += 0.05f; m_uniformsDirty = true; update();
	}

}

