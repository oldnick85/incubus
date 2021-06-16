#include "drawprogressbar.h"

CDrawProgressBar::CDrawProgressBar(IDrawEnginePtr draw)
    : CDrawBuffers({{0, 3, 6, 0}, {1, 3, 6, 3}})
    , m_draw(draw)
{
    SetShaders(m_vertex_shader_source, m_fragment_shader_source);
}

void CDrawProgressBar::SetProgress(const float progress)
{
    if (m_progress == progress)
        return;
    m_progress = progress;
    MakeBuffersProgressBar();
}

void CDrawProgressBar::SetSize(const float width, const float height)
{
    if ((m_width == width) and (m_height == height))
        return;
    m_width = width;
    m_height = height;
    MakeBuffersProgressBar();
}

void CDrawProgressBar::SetPosition(const std::array<float, 2> &position)
{
    if (m_position == position)
        return;
    m_position = position;
    MakeBuffersProgressBar();
}

void CDrawProgressBar::SetColors(const std::array<float, 4> &color_done, const std::array<float, 4> &color_undone)
{
    m_color_done = color_done;
    m_color_undone = color_undone;
    MakeBuffersProgressBar();
}

void CDrawProgressBar::MakeBuffersProgressBar()
{
    std::vector<GLfloat> vertices;
    vertices.reserve(12);

    float x0_d = m_position[0];
    float y0_d = m_position[1];
    float x1_d = x0_d+m_width*m_progress;
    float y1_d = y0_d+m_height;

    vertices.push_back(x0_d);
    vertices.push_back(y0_d);
    vertices.push_back(0.0);
    vertices.push_back(m_color_done[0]);
    vertices.push_back(m_color_done[1]);
    vertices.push_back(m_color_done[2]);

    vertices.push_back(x0_d);
    vertices.push_back(y1_d);
    vertices.push_back(0.0);
    vertices.push_back(m_color_done[0]);
    vertices.push_back(m_color_done[1]);
    vertices.push_back(m_color_done[2]);

    vertices.push_back(x1_d);
    vertices.push_back(y1_d);
    vertices.push_back(0.0);
    vertices.push_back(m_color_done[0]);
    vertices.push_back(m_color_done[1]);
    vertices.push_back(m_color_done[2]);

    vertices.push_back(x0_d);
    vertices.push_back(y0_d);
    vertices.push_back(0.0);
    vertices.push_back(m_color_done[0]);
    vertices.push_back(m_color_done[1]);
    vertices.push_back(m_color_done[2]);

    vertices.push_back(x1_d);
    vertices.push_back(y0_d);
    vertices.push_back(0.0);
    vertices.push_back(m_color_done[0]);
    vertices.push_back(m_color_done[1]);
    vertices.push_back(m_color_done[2]);

    vertices.push_back(x1_d);
    vertices.push_back(y1_d);
    vertices.push_back(0.0);
    vertices.push_back(m_color_done[0]);
    vertices.push_back(m_color_done[1]);
    vertices.push_back(m_color_done[2]);

    float x0_u = x1_d;
    float y0_u = y0_d;
    float x1_u = x0_d+m_width;
    float y1_u = y1_d;

    vertices.push_back(x0_u);
    vertices.push_back(y0_u);
    vertices.push_back(0.0);
    vertices.push_back(m_color_undone[0]);
    vertices.push_back(m_color_undone[1]);
    vertices.push_back(m_color_undone[2]);

    vertices.push_back(x0_u);
    vertices.push_back(y1_u);
    vertices.push_back(0.0);
    vertices.push_back(m_color_undone[0]);
    vertices.push_back(m_color_undone[1]);
    vertices.push_back(m_color_undone[2]);

    vertices.push_back(x1_u);
    vertices.push_back(y1_u);
    vertices.push_back(0.0);
    vertices.push_back(m_color_undone[0]);
    vertices.push_back(m_color_undone[1]);
    vertices.push_back(m_color_undone[2]);

    vertices.push_back(x0_u);
    vertices.push_back(y0_u);
    vertices.push_back(0.0);
    vertices.push_back(m_color_undone[0]);
    vertices.push_back(m_color_undone[1]);
    vertices.push_back(m_color_undone[2]);

    vertices.push_back(x1_u);
    vertices.push_back(y0_u);
    vertices.push_back(0.0);
    vertices.push_back(m_color_undone[0]);
    vertices.push_back(m_color_undone[1]);
    vertices.push_back(m_color_undone[2]);

    vertices.push_back(x1_u);
    vertices.push_back(y1_u);
    vertices.push_back(0.0);
    vertices.push_back(m_color_undone[0]);
    vertices.push_back(m_color_undone[1]);
    vertices.push_back(m_color_undone[2]);

    MakeBuffers(vertices.data(), vertices.size());
}

void CDrawProgressBar::Draw()
{
    BeginShadersProgram();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_POLYGON_SMOOTH );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glm::mat4 transform = glm::mat4(1.0);
    if (m_use_transform)
    {
        transform = m_draw->GetCurrentTransform();
    }
    GLint transform_location = glGetUniformLocation(GetShadersProgram(), "transform");
    glUniformMatrix4fv(transform_location, 1, GL_FALSE, glm::value_ptr(transform));

    BeginVertexBuffer();
    glDrawArrays(GL_TRIANGLES, 0, 12);
    EndVertexBuffer();

    EndShadersProgram();
}
