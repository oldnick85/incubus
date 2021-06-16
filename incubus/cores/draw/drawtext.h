#ifndef CDRAWTEXT_H
#define CDRAWTEXT_H

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

class CDrawText
        : private CDrawShaders
        , private CDrawBuffers
{
public:
    CDrawText(IDrawEnginePtr draw);
    ~CDrawText() = default;
    void SetText(const std::string &text);
    void SetSymbolWidth(const float width);
    void SetColor(const std::array<float, 3> &color);
    void SetPosition(const std::array<float, 2> &position);
    void Draw();
private:
    void MakeTextBuffers();

    IDrawEnginePtr          m_draw;
    std::string             m_text;
    float                   m_symbol_width = 0.05;
    std::array<float, 3>    m_color = {0.0, 0.0, 0.0};
    std::array<float, 2>    m_position = {0.0, 0.0};

    const GLchar* m_vertex_shader_source =
            "#version 330 core\n"
            "layout (location = 0) in vec3 position;\n"
            "layout (location = 1) in vec3 color;\n"
            "layout (location = 2) in vec2 texCoord;\n"
            "out vec3 ourColor;\n"
            "out vec2 TexCoord;\n"
            "void main()\n"
            "{\n"
            "gl_Position = vec4(position, 1.0f);\n"
            "ourColor = color;\n"
            "TexCoord = texCoord;\n"
            "}\n";
    const GLchar* m_fragment_shader_source =
            "#version 330 core\n"
            "in vec3 ourColor;\n"
            "in vec2 TexCoord;\n"
            "out vec4 color;\n"
            "uniform sampler2D ourTexture;\n"
            "void main()\n"
            "{\n"
            "color = vec4(ourColor, 1.0f) * texture(ourTexture, TexCoord);\n"
            "}\n";
};
using CDrawTextPtr = std::shared_ptr<CDrawText>;

#endif // CDRAWTEXT_H
