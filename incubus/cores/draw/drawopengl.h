#ifndef CDRAWOPENGL_H
#define CDRAWOPENGL_H

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

#include "idrawengine.h"

class CDrawOpenGL
        : public IDrawEngine
{
public:
    virtual ~CDrawOpenGL() = default;
    virtual bool Init() override final;
    virtual void Release() override final;
    virtual void Clear() override final;
    virtual bool ShouldCloseWindow() override final;
    virtual void ContextHold() override final;
    virtual void ContextRelease() override final;
    virtual void GetCursorPos(double &x, double &y) override final;
    virtual int GetMouseButton(int button) override final;
    virtual int GetKey(int key) override final;
    //virtual void BeginPoints() override final;
    //virtual void BeginTriangles() override final;
    //virtual void BeginTriangleFan() override final;
    //virtual void BeginLines() override final;
    //virtual void BeginLineStrip() override final;
    //virtual void BeginLineLoop() override final;
    //virtual void BeginPolygon() override final;
    //virtual void BeginQuads() override final;
    //virtual void End() override final;
    virtual void PushMatrix() override final;
    virtual void PopMatrix() override final;
    virtual void Translate(const Real_t x, const Real_t y, const Real_t z) override final;
    //virtual void Vertex2(const Real_t x, const Real_t y) override final;
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
private:
    GLFWwindow*     m_window;
    const GLuint WIDTH = 600;
    const GLuint HEIGHT = 600;
};
using CDrawOpenGLPtr = std::shared_ptr<CDrawOpenGL>;

#endif // CDRAWOPENGL3_H
