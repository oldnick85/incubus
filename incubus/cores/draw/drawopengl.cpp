#include "drawopengl.h"
#include <stdio.h>

bool CDrawOpenGL::Init()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    char window_title[128];
    snprintf(window_title, sizeof(window_title), "Incubus");
    m_window = static_cast<GLFWwindow*>(glfwCreateWindow(WIDTH, HEIGHT, window_title, nullptr, nullptr));
    glfwMakeContextCurrent(m_window);

    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();
    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);
    glViewport(0, 0, width, height);
    glfwMakeContextCurrent(nullptr);
    return true;
}

bool CDrawOpenGL::ShouldCloseWindow()
{
    return glfwWindowShouldClose(m_window);
}

void CDrawOpenGL::Clear()
{
    // Uncommenting this call will result in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // Render
    // Clear the colorbuffer
    glClearColor(0.4f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void CDrawOpenGL::Release()
{
    glfwTerminate();
}

void CDrawOpenGL::ContextHold()
{
    glfwMakeContextCurrent(m_window);
}

void CDrawOpenGL::ContextRelease()
{
    glfwMakeContextCurrent(nullptr);
}

void CDrawOpenGL::GetCursorPos(double &x, double &y)
{
    glfwGetCursorPos(m_window, &x, &y);
}

int CDrawOpenGL::GetMouseButton(int button)
{
    return glfwGetMouseButton(m_window, button);
}

int CDrawOpenGL::GetKey(int key)
{
    return glfwGetKey(m_window, key);
}

//void CDrawOpenGL::BeginPoints() {glBegin(GL_POINTS);}
//void CDrawOpenGL::BeginTriangles() {glBegin(GL_TRIANGLES);}
//void CDrawOpenGL::BeginTriangleFan() {glBegin(GL_TRIANGLE_FAN);}
//void CDrawOpenGL::BeginLines() {glBegin(GL_LINES);}
//void CDrawOpenGL::BeginLineStrip() {glBegin(GL_LINE_STRIP);}
//void CDrawOpenGL::BeginLineLoop() {glBegin(GL_LINE_LOOP);}
//void CDrawOpenGL::BeginPolygon() {glBegin(GL_POLYGON);}
//void CDrawOpenGL::BeginQuads() {glBegin(GL_QUADS);}
//void CDrawOpenGL::End() {glEnd();}

void CDrawOpenGL::PushMatrix()
{
    glPushMatrix();
}

void CDrawOpenGL::PopMatrix()
{
    glPopMatrix();
}

void CDrawOpenGL::Translate(const Real_t x, const Real_t y, const Real_t z)
{
    glTranslated(x.Val(), y.Val(), z.Val());
}

//void CDrawOpenGL::Vertex2(const Real_t x, const Real_t y) {glVertex2d(x.Val(), y.Val());}

void CDrawOpenGL::Scale(const Real_t x, const Real_t y, const Real_t z)
{
    glScaled(x.Val(), y.Val(), z.Val());
}

uint CDrawOpenGL::GenLists(const uint size)
{
    return glGenLists(size);
}

void CDrawOpenGL::NewList(const uint list)
{
    glNewList(list, GL_COMPILE);
}

void CDrawOpenGL::EndList()
{
    glEndList();
}

void CDrawOpenGL::CallList(const uint list)
{
    glCallList(list);
}

void CDrawOpenGL::PointSize(const Real_t sz)
{
    glPointSize(sz.Val());
}

void CDrawOpenGL::LineWidth(const Real_t w)
{
    glLineWidth(w.Val());
}

void CDrawOpenGL::EnablePointSmooth()
{
    glEnable(GL_POINT_SMOOTH);
}

void CDrawOpenGL::EnableLineSmooth()
{
    glEnable(GL_LINE_SMOOTH);
}

void CDrawOpenGL::Color3(const Real_t r, const Real_t g, const Real_t b)
{
    glColor3d(r.Val(), g.Val(), b.Val());
}

void CDrawOpenGL::Color4(const Real_t r, const Real_t g, const Real_t b, const Real_t a)
{
    //glColor4d(r.Val(), g.Val(), b.Val(), a.Val());
}

void CDrawOpenGL::RasterPos2(const Real_t x, const Real_t y)
{
    glRasterPos2d(x.Val(), y.Val());
}

void CDrawOpenGL::PixelZoom(const Real_t x, const Real_t y)
{
    glPixelZoom(x.Val(), y.Val());
}

void CDrawOpenGL::DrawPixels(uint width, uint height, const void *pixels)
{
    glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}
