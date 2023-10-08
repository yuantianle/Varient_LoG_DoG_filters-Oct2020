#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Mark.h"

#include <qtimer.h>
#include <qopenglfunctions.h>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShader>
#include <qoffscreensurface.h>
#include <qopenglvertexarrayobject.h>
#include <qopenglbuffer.h>

#include <tuple>
#include "Quaternion.h"
#include <QWidget>
#include <qfile.h>
#include <qfiledialog.h>
#include <stdio.h>


class Mark : public QMainWindow
{
    Q_OBJECT

public:
    Mark(QWidget *parent = Q_NULLPTR);

private:
    Ui::MarkClass ui;
    std::vector<std::string> shaderpaths;
    std::vector<std::string> _glslUniforms;
    std::vector<std::string> _glslAttributes;
    QStringList shaderCompileLog;

private slots:
    void on_btn_loadply_clicked();
};
