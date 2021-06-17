#ifndef CDRAWOPENGL3_H
#define CDRAWOPENGL3_H

#include <iostream>
#include <vector>
#include <stack>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#define GLFW_STATIC
#include <GLFW/glfw3.h>

#include "idrawengine.h"
#include "drawenginetextures.h"

class CDrawOpenGL3
        : public IDrawEngine
        , private CDrawEngineTextures
{
public:
    virtual ~CDrawOpenGL3() = default;
    virtual bool Init() override final;
    virtual void Release() override final;
    virtual void Clear() override final;
    virtual void SwapBuffers() override final;
    virtual bool ShouldCloseWindow() override final;
    virtual void ContextHold() override final;
    virtual void ContextRelease() override final;
    virtual void GetCursorPos(double &x, double &y) override final;
    virtual int GetMouseButton(int button) override final;
    virtual int GetKey(int key) override final;
    virtual void PushMatrix() override final;
    virtual void PopMatrix() override final;
    virtual void Translate(const Real_t x, const Real_t y, const Real_t z) override final;
    virtual void Scale(const Real_t x, const Real_t y, const Real_t z) override final;
    virtual uint GenLists(const uint size) override final;
    virtual void NewList(const uint list) override final;
    virtual void EndList() override final;
    virtual void CallList(const uint list) override final;
    virtual void PointSize(const Real_t sz) override final;
    virtual void LineWidth(const Real_t w) override final;
    virtual void EnablePointSmooth() override final;
    virtual void EnableLineSmooth() override final;
    virtual void Color3(const Real_t r, const Real_t g, const Real_t b) override final;
    virtual void Color4(const Real_t r, const Real_t g, const Real_t b, const Real_t a) override final;
    virtual void RasterPos2(const Real_t x, const Real_t y) override final;
    virtual void PixelZoom(const Real_t x, const Real_t y) override final;
    virtual void DrawPixels(uint width, uint height, const void *pixels) override final;

    virtual void DrawPoints2(const std::vector<std::array<Real_t, 2>> &points) override final;
    virtual void DrawTriangles2(const std::vector<std::array<Real_t, 2>> &points) override final;
    virtual void DrawTriangleFan2(const std::vector<std::array<Real_t, 2>> &points) override final;
    virtual void DrawLines2(const std::vector<std::array<Real_t, 2>> &points) override final;
    virtual void DrawLineStrip2(const std::vector<std::array<Real_t, 2>> &points) override final;
    virtual void DrawLineLoop2(const std::vector<std::array<Real_t, 2>> &points) override final;
    virtual void DrawPolygon2(const std::vector<std::array<Real_t, 2>> &points) override final;
    virtual void DrawQuads2(const std::vector<std::array<Real_t, 2>> &points) override final;
    virtual glm::mat4 GetCurrentTransform() const override final;
    virtual glm::vec4 GetCurrentColor() const override final;
    virtual uint GetFontTexture() const override final;
private:
    void DrawPrimitives2(const GLenum type, const std::vector<std::array<Real_t, 2>> &points);
    void ApplyCurTransform();

    GLFWwindow* m_window = nullptr;
    GLuint VBO, VAO, EBO;
    GLuint shaderProgram;
    GLuint m_simple_shader_program;
    const GLuint WIDTH = 600;
    const GLuint HEIGHT = 600;

    const GLchar* m_simple_vertex_shader_source = "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "uniform mat4 transform;\n"
        "void main()\n"
        "{\n"
        "gl_Position = transform * vec4(position.x, position.y, position.z, 1.0);\n"
        "}\0";
    const GLchar* m_simple_fragment_shader_source = "#version 330 core\n"
        "out vec4 color;\n"
        "uniform vec4 ourColor;\n"
        "void main()\n"
        "{\n"
        "color = ourColor;\n"
        "}\n\0";

    const GLchar* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "uniform mat4 transform;\n"
        "out vec4 vertexColor;\n"
        "void main()\n"
        "{\n"
        "gl_Position = transform * vec4(position.x, position.y, position.z, 1.0);\n"
        "vertexColor = vec4(0.5f, position.y, gl_Position.x, 1.0f);"
        "}\0";
    const GLchar* fragmentShaderSource = "#version 330 core\n"
        "in vec4 vertexColor;\n"
        "out vec4 color;\n"
        "uniform vec4 ourColor;\n"
        "void main()\n"
        "{\n"
        "color = vertexColor + ourColor;\n"
        "}\n\0";

    std::array<GLfloat, 12> m_vertices = {
         0.5f,  0.4f, 0.0f,  // Top Right
         0.5f, -0.5f, 0.0f,  // Bottom Right
        -0.5f, -0.5f, 0.0f,  // Bottom Left
        -0.5f,  0.5f, 0.0f   // Top Left
    };

    std::array<GLuint, 6> m_indices = {  // Note that we start from 0!
        0, 1, 3,  // First Triangle
        1, 2, 3   // Second Triangle
    };

    glm::mat4               m_cur_transform;
    glm::vec4               m_cur_color;
    std::stack<glm::mat4>   m_transform_stack;

};
using CDrawOpenGL3Ptr = std::shared_ptr<CDrawOpenGL3>;

#endif // CDRAWOPENGL3_H
