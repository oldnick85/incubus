#ifndef CDRAWTRACE_H
#define CDRAWTRACE_H

#include <iostream>

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

class CDrawTrace
        : private CDrawShaders
        , private CDrawBuffers
{
public:
    CDrawTrace(IDrawEnginePtr draw);
    ~CDrawTrace();
    void SetTrace(const std::vector<std::array<Real_t, 3>> &points);
    void Draw();

    double time_to_live = 0.2;
private:
    void FillBuffers();
    void DrawBuffers();

    IDrawEnginePtr  m_draw;

    std::vector<std::array<Real_t, 3>> m_points;

    const GLchar* m_vertex_shader_source = "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "uniform mat4 transform;\n"
        "out float vertexAlpha;\n"
        "void main()\n"
        "{\n"
        "gl_Position = transform * vec4(position.x, position.y, 0.0, 1.0);\n"
        "vertexAlpha = position.z;\n"
        "}\0";
    const GLchar* m_fragment_shader_source = "#version 330 core\n"
        "in float vertexAlpha;\n"
        "out vec4 color;\n"
        "uniform vec4 ourColor;\n"
        "void main()\n"
        "{\n"
        //"color = vec4(0.0f, 0.0f, 0.0f, 1.0f);\n"
        "color = ourColor;\n"
        "color.a = vertexAlpha;\n"
        "}\n\0";
};
using CDrawTracePtr = std::shared_ptr<CDrawTrace>;

#endif // CDRAWTRACE_H
