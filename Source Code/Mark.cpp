#include "Mark.h"

Mark::Mark(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

void Mark::on_btn_loadply_clicked()
{
	shaderpaths.clear();
	_glslAttributes.clear();
	_glslUniforms.clear();
	shaderpaths.push_back(".\\Shaders\\Vertex.vert");
	shaderpaths.push_back(".\\Shaders\\Fragment.frag");
	_glslAttributes.push_back("vertex_position");
	_glslAttributes.push_back("vertex_color");
	_glslAttributes.push_back("vertex_texcoord");
	_glslAttributes.push_back("vertex_normal");
	_glslUniforms.push_back("ModelMatrix");
	_glslUniforms.push_back("ViewMatrix");
	_glslUniforms.push_back("ProjectionMatrix");
	_glslUniforms.push_back("texture0");
}