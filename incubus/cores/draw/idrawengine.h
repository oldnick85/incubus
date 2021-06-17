#ifndef IDRAWENGINE_H
#define IDRAWENGINE_H

#include <memory>
#include <vector>
#include "../math/real_number.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class IDrawEngine
{
public:
    virtual ~IDrawEngine() = default;
    virtual bool Init() = 0;
    virtual void Release() = 0;
    virtual void ContextHold() = 0;
    virtual void ContextRelease() = 0;
    virtual bool ShouldCloseWindow() = 0;
    virtual void Clear() = 0;
    virtual void SwapBuffers() = 0;
    virtual void GetCursorPos(double &x, double &y) = 0;
    virtual int GetMouseButton(int button) = 0;
    virtual int GetKey(int key) = 0;
    virtual void PushMatrix() = 0;
    virtual void PopMatrix() = 0;
    virtual void Translate(const Real_t x, const Real_t y, const Real_t z) = 0;
    virtual void Scale(const Real_t x, const Real_t y, const Real_t z) = 0;
    virtual uint GenLists(const uint size) = 0;
    virtual void NewList(const uint list) = 0;
    virtual void EndList() = 0;
    virtual void CallList(const uint list) = 0;
    virtual void PointSize(const Real_t sz) = 0;
    virtual void LineWidth(const Real_t w) = 0;
    virtual void EnablePointSmooth() = 0;
    virtual void EnableLineSmooth() = 0;
    virtual void Color3(const Real_t r, const Real_t g, const Real_t b) = 0;
    virtual void Color4(const Real_t r, const Real_t g, const Real_t b, const Real_t a) = 0;
    virtual void RasterPos2(const Real_t x, const Real_t y) = 0;
    virtual void PixelZoom(const Real_t x, const Real_t y) = 0;
    virtual void DrawPixels(uint width, uint height, const void *pixels) = 0;
    virtual void DrawPoints2(const std::vector<std::array<Real_t, 2>> &points) = 0;
    virtual void DrawTriangles2(const std::vector<std::array<Real_t, 2>> &points) = 0;
    virtual void DrawTriangleFan2(const std::vector<std::array<Real_t, 2>> &points) = 0;
    virtual void DrawLines2(const std::vector<std::array<Real_t, 2>> &points) = 0;
    virtual void DrawLineStrip2(const std::vector<std::array<Real_t, 2>> &points) = 0;
    virtual void DrawLineLoop2(const std::vector<std::array<Real_t, 2>> &points) = 0;
    virtual void DrawPolygon2(const std::vector<std::array<Real_t, 2>> &points) = 0;
    virtual void DrawQuads2(const std::vector<std::array<Real_t, 2>> &points) = 0;
    virtual uint GetFontTexture() const = 0;
    virtual glm::mat4 GetCurrentTransform() const = 0;
    virtual glm::vec4 GetCurrentColor() const = 0;
private:
};
using IDrawEnginePtr = std::shared_ptr<IDrawEngine>;

#endif // IDRAWENGINE_H
