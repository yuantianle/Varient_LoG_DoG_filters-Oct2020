#pragma once

#include <QOpenGLWidget>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLExtraFunctions>
#include <QString>
#include <QVector3D>
#include <QKeyEvent>


class MyWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	MyWidget(QWidget* parent = Q_NULLPTR);
	~MyWidget();

private:
	QOpenGLVertexArrayObject m_vao;
	QOpenGLBuffer m_vbo;
	QOpenGLShaderProgram* m_program;
	QOpenGLShader* m_shader;
	QOpenGLTexture* m_texture;
	QString shadersource;
	int m_modelMatrixLoc;
	int m_viewMatrixLoc;
	int m_projectionMatrixLoc;
	int m_lightPosLoc;

	QVector3D m_eye = QVector3D(0,0, 2);
	QVector3D m_target = QVector3D(0,0,-1);

	QMatrix4x4 m_proj= QMatrix4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	QMatrix4x4 m_world= QMatrix4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	
	bool m_uniformsDirty;

protected:
	void initializeGL();            //initialization
	void resizeGL(int w, int h);    //reshape
	void paintGL();                 //display

//Slots for slider bar and redraw botton. Signals to slider bar and label.
private:
	float xRot;
	float yRot;
	float zRot;
	float x;
	float sx,sy;  //scale, we dont use y
	float y;

public slots:
	void rotatex(int angle);
	void rotatey(int angle);
	void rotatez(int angle);
	void redraw();

signals:
	void xRotationChanged(int angle);
	void yRotationChanged(int angle);
	void zRotationChanged(int angle);

public:
	void MyWidget::keyPressEvent(QKeyEvent* e);
};