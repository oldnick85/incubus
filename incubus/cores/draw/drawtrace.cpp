#include "drawtrace.h"

CDrawTrace::CDrawTrace(IDrawEnginePtr draw)
    : m_draw(draw)
{
    SetShaders(m_vertex_shader_source, m_fragment_shader_source);
}

CDrawTrace::~CDrawTrace()
{

}

void CDrawTrace::SetTrace(const std::vector<std::array<Real_t, 3>> &points)
{
    m_points = points;
    DeleteBuffers();
    FillBuffers();
}

void CDrawTrace::Draw()
{
    BeginShadersProgram();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_POLYGON_SMOOTH );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );

    auto transform = m_draw->GetCurrentTransform();
    GLint transformLoc = glGetUniformLocation(GetShadersProgram(), "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    auto color = m_draw->GetCurrentColor();
    GLint vertexColorLocation = glGetUniformLocation(GetShadersProgram(), "ourColor");
    glUniform4f(vertexColorLocation, color[0], color[1], color[2], color[3]);
    glLineWidth(4.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    DrawBuffers();
    EndShadersProgram();
}

void CDrawTrace::FillBuffers()
{
    std::vector<GLfloat> vertices;
    vertices.resize(m_points.size()*3);
    uint i = 0;
    for (const auto &pt : m_points)
    {
        vertices[3*i+0] = pt[0].Val();
        vertices[3*i+1] = pt[1].Val();
        double al = 0.0;
        if (pt[2] < time_to_live)
        {
            al = 1.0 - pt[2].Val()/time_to_live;
        }
        vertices[3*i+2] = al;
        i++;
    }
    MakeBuffers(vertices.data(), vertices.size());
}

void CDrawTrace::DrawBuffers()
{
    BeginVertexBuffer();
    glDrawArrays(GL_LINE_STRIP, 0, m_points.size());
    EndVertexBuffer();
}
