#ifndef CDRAWPROGRESSBAR_H
#define CDRAWPROGRESSBAR_H

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
#include "drawshaders.h"
#include "drawbuffers.h"

class CDrawProgressBar
        : private CDrawShaders
        , private CDrawBuffers
{
public:
    CDrawProgressBar(IDrawEnginePtr draw);
    ~CDrawProgressBar() = default;

    void Draw();

    void SetProgress(const float progress);
    void SetSize(const float width, const float height);
    void SetPosition(const std::array<float, 2> &position);
    void SetColors(const std::array<float, 4> &color_done, const std::array<float, 4> &color_undone);
    void SetUseTransform(const bool use) {m_use_transform = use;}
private:
    void MakeBuffersProgressBar();

    IDrawEnginePtr          m_draw;

    bool                    m_use_transform = false;
    float                   m_progress = 0.0;
    float                   m_width = 0.1;
    float                   m_height = 0.01;
    std::array<float, 2>    m_position = {0.0, 0.0};
    std::array<float, 4>    m_color_done = {0.0, 1.0, 0.0, 1.0};
    std::array<float, 4>    m_color_undone = {0.5, 0.5, 0.5, 1.0};

    const GLchar* m_vertex_shader_source =
            "#version 330 core\n"
            "layout (location = 0) in vec3 position;\n"
            "layout (location = 1) in vec3 color;\n"
            "uniform mat4 transform;\n"
            "out vec3 ourColor;\n"
            "void main()\n"
            "{\n"
            "gl_Position = transform * vec4(position, 1.0f);\n"
            "ourColor = color;\n"
            "}\n";
    const GLchar* m_fragment_shader_source =
            "#version 330 core\n"
            "in vec3 ourColor;\n"
            "out vec4 color;\n"
            "void main()\n"
            "{\n"
            "color = vec4(ourColor, 1.0f);\n"
            "}\n";
};

#endif // CDRAWPROGRESSBAR_H
