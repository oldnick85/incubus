#ifndef CDRAWFIGURE_H
#define CDRAWFIGURE_H

#include <iostream>
#include <cassert>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#define GLFW_STATIC
#include <GLFW/glfw3.h>
// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "idrawengine.h"
#include "../math/shape.h"
#include "drawshaders.h"
#include "drawbuffers.h"

class CDrawFigure
        : protected CFigure
        , private CDrawShaders
        , private CDrawBuffers
{
public:
    CDrawFigure(IDrawEnginePtr draw);
    CDrawFigure(IDrawEnginePtr draw, const CFigure &figure);
    ~CDrawFigure() = default;
    void SetFigure(const CFigure &figure);
    void Draw();
    bool lined = false;
    double line_width = 1.0;
private:
    void MakeBuffersPoint();
    void MakeBuffersCircle();
    void MakeBuffersLine();
    void MakeBuffersTriangle();
    void MakeBuffersRectangle();
    void MakeBuffersRectangleAxes();
    void DrawBuffersPoint();
    void DrawBuffersCircle();
    void DrawBuffersLine();
    void DrawBuffersTriangle();
    void DrawBuffersRectangle();
    void DrawBuffersRectangleAxes();

    IDrawEnginePtr  m_draw;

    const GLchar* m_vertex_shader_source = "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "uniform mat4 transform;\n"
        "void main()\n"
        "{\n"
        "gl_Position = transform * vec4(position.x, position.y, position.z, 1.0);\n"
        "}\0";
    const GLchar* m_fragment_shader_source = "#version 330 core\n"
        "out vec4 color;\n"
        "uniform vec4 ourColor;\n"
        "void main()\n"
        "{\n"
        //"color = vec4(0.9f, 0.9f, 0.9f, 1.0f);\n"
        "color = ourColor;\n"
        "}\n\0";
};
using CDrawFigurePtr = std::shared_ptr<CDrawFigure>;

#endif // CDRAWFIGURE_H
