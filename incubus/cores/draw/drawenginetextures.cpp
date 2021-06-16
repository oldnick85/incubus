#include "drawenginetextures.h"

CDrawEngineTextures::CDrawEngineTextures()
{

}

CDrawEngineTextures::~CDrawEngineTextures()
{
    glDeleteTextures(1, &m_font_texture);
}

void CDrawEngineTextures::LoadTextures()
{
    glGenTextures(1, &m_font_texture);
    glBindTexture(GL_TEXTURE_2D, m_font_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, channels;
    unsigned char* image = SOIL_load_image("resources/fonts/font_pixel_18_36_a.png", &width, &height, &channels, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    assert(image);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint CDrawEngineTextures::GetTexture(const enTexture texture) const
{
    switch (texture)
    {
    case txFont:    return m_font_texture;
    default:        break;
    }
    return 0;
}
