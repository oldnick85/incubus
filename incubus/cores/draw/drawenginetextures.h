#ifndef CDRAWENGINETEXTURES_H
#define CDRAWENGINETEXTURES_H

#include <memory>

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

#include <SOIL/SOIL.h>

class CDrawEngineTextures
{
public:
    CDrawEngineTextures();
    ~CDrawEngineTextures();

    enum enTexture
    {
        txFont,
    };

    GLuint GetTexture(const enTexture texture) const;
    void LoadTextures();
private:
    GLuint m_font_texture = -1;
};
using CDrawEngineTexturesPtr = std::shared_ptr<CDrawEngineTextures>;

#endif // CDRAWENGINETEXTURES_H
