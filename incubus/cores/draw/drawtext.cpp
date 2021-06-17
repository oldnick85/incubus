#include "drawtext.h"

CDrawText::CDrawText(IDrawEnginePtr draw)
    : CDrawBuffers({{0, 3, 8, 0}, {1, 3, 8, 3}, {2, 2, 8, 6}})
    , m_draw(draw)
{
    SetShaders(m_vertex_shader_source, m_fragment_shader_source);
}

void CDrawText::SetText(const std::string &text)
{
    if (m_text == text)
        return;
    m_text = text;
    MakeTextBuffers();
}

void CDrawText::SetSymbolWidth(const float width)
{
    m_symbol_width = width;
}

void CDrawText::SetColor(const std::array<float, 3> &color)
{
    m_color = color;
}

void CDrawText::SetPosition(const std::array<float, 2> &position)
{
    m_position = position;
}

void CDrawText::MakeTextBuffers()
{
    std::vector<GLfloat> vertices;
    vertices.reserve(m_text.size()*8*6);
    float i = 0;
    for (const char c : m_text)
    {
        float x0 = i*m_symbol_width+m_position[0];
        float y0 = m_position[1];
        float x1 = (i+1)*m_symbol_width+m_position[0];
        float y1 = m_symbol_width*2+m_position[1];
        float cx = c % 16;
        float cy = c / 16;
        float tx_x0 = cx/16.0;
        float tx_y1 = cy/16.0;
        float tx_x1 = (cx+1)/16.0;
        float tx_y0 = (cy+1)/16.0;
        vertices.push_back(x0);
        vertices.push_back(y0);
        vertices.push_back(0.0);
        vertices.push_back(m_color[0]);
        vertices.push_back(m_color[1]);
        vertices.push_back(m_color[2]);
        vertices.push_back(tx_x0);
        vertices.push_back(tx_y0);

        vertices.push_back(x1);
        vertices.push_back(y0);
        vertices.push_back(0.0);
        vertices.push_back(m_color[0]);
        vertices.push_back(m_color[1]);
        vertices.push_back(m_color[2]);
        vertices.push_back(tx_x1);
        vertices.push_back(tx_y0);

        vertices.push_back(x1);
        vertices.push_back(y1);
        vertices.push_back(0.0);
        vertices.push_back(m_color[0]);
        vertices.push_back(m_color[1]);
        vertices.push_back(m_color[2]);
        vertices.push_back(tx_x1);
        vertices.push_back(tx_y1);

        vertices.push_back(x0);
        vertices.push_back(y0);
        vertices.push_back(0.0);
        vertices.push_back(m_color[0]);
        vertices.push_back(m_color[1]);
        vertices.push_back(m_color[2]);
        vertices.push_back(tx_x0);
        vertices.push_back(tx_y0);

        vertices.push_back(x0);
        vertices.push_back(y1);
        vertices.push_back(0.0);
        vertices.push_back(m_color[0]);
        vertices.push_back(m_color[1]);
        vertices.push_back(m_color[2]);
        vertices.push_back(tx_x0);
        vertices.push_back(tx_y1);

        vertices.push_back(x1);
        vertices.push_back(y1);
        vertices.push_back(0.0);
        vertices.push_back(m_color[0]);
        vertices.push_back(m_color[1]);
        vertices.push_back(m_color[2]);
        vertices.push_back(tx_x1);
        vertices.push_back(tx_y1);
        i += 1;
    }
    MakeBuffers(vertices.data(), vertices.size());
}

void CDrawText::Draw()
{
    const auto texture = m_draw->GetFontTexture();

    BeginShadersProgram();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_POLYGON_SMOOTH );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    const auto texture_location = glGetUniformLocation(GetShadersProgram(), "ourTexture");
    glUniform1i(texture_location, 0);

    BeginVertexBuffer();
    const auto sz = m_text.size()*6;
    glDrawArrays(GL_TRIANGLES, 0, sz);
    EndVertexBuffer();

    EndShadersProgram();

    glBindTexture(GL_TEXTURE_2D, 0);
}
