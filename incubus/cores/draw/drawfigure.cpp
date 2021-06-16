#include "drawfigure.h"

CDrawFigure::CDrawFigure(IDrawEnginePtr draw)
    : m_draw(draw)
{
    SetShaders(m_vertex_shader_source, m_fragment_shader_source);
}


CDrawFigure::CDrawFigure(IDrawEnginePtr draw, const CFigure &figure)
    : m_draw(draw)
{
    SetShaders(m_vertex_shader_source, m_fragment_shader_source);
    SetFigure(figure);
}

void CDrawFigure::SetFigure(const CFigure &figure)
{
    if (static_cast<CFigure>(*this) == figure) return;
    *static_cast<CFigure*>(this) = figure;
    DeleteBuffers();
    switch (type) {
    case figNone:                                       break;
    case figPoint:          MakeBuffersPoint();         break;
    case figCircle:         MakeBuffersCircle();        break;
    case figLine:           MakeBuffersLine();          break;
    case figTriangle:       MakeBuffersTriangle();      break;
    case figRectangle:      MakeBuffersRectangle();     break;
    case figRectangleAxes:  MakeBuffersRectangleAxes(); break;
    default:                                            break;
    }
}

void CDrawFigure::Draw()
{
    BeginShadersProgram();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_POLYGON_SMOOTH );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );

    auto transform = m_draw->GetCurrentTransform();
    const auto transformLoc = glGetUniformLocation(GetShadersProgram(), "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    auto color = m_draw->GetCurrentColor();
    const auto vertexColorLocation = glGetUniformLocation(GetShadersProgram(), "ourColor");
    glUniform4f(vertexColorLocation, color[0], color[1], color[2], color[3]);
    if (lined)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(line_width);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    switch (type) {
    case figNone:                                       break;
    case figPoint:          DrawBuffersPoint();         break;
    case figCircle:         DrawBuffersCircle();        break;
    case figLine:           DrawBuffersLine();          break;
    case figTriangle:       DrawBuffersTriangle();      break;
    case figRectangle:      DrawBuffersRectangle();     break;
    case figRectangleAxes:  DrawBuffersRectangleAxes(); break;
    default:                                            break;
    }
    EndShadersProgram();
}

void CDrawFigure::MakeBuffersPoint()
{
    std::array<GLfloat, 3> vertices = {
        static_cast<GLfloat>(center.x.Val()),  static_cast<GLfloat>(center.y.Val()), 0.0f
    };
    MakeBuffers(vertices.data(), vertices.size());
}

void CDrawFigure::DrawBuffersPoint()
{
    BeginVertexBuffer();
    glDrawArrays(GL_POINTS, 0, 1);
    EndVertexBuffer();
}

void CDrawFigure::MakeBuffersCircle()
{
    std::array<std::array<GLfloat, 3>, 32> vertices;
    for (uint i = 0; i < 32; ++i)
    {
        auto angle = i*M_PI*2.0/32.0;
        vertices[i] = {static_cast<GLfloat>(center.x.Val()+radius.Val()*cos(angle)), static_cast<GLfloat>(center.y.Val()+radius.Val()*sin(angle)), 0.0f};
    }
    MakeBuffers(vertices[0].data(), 3*vertices.size());
}

void CDrawFigure::DrawBuffersCircle()
{
    BeginVertexBuffer();
    glDrawArrays(GL_LINE_LOOP, 0, 32);
    EndVertexBuffer();
}

void CDrawFigure::MakeBuffersLine()
{
    std::array<GLfloat, 6> vertices = {
        static_cast<GLfloat>(pnt[0].x.Val()),  static_cast<GLfloat>(pnt[0].y.Val()), 0.0f,
        static_cast<GLfloat>(pnt[1].x.Val()),  static_cast<GLfloat>(pnt[1].y.Val()), 0.0f,
    };
    MakeBuffers(vertices.data(), vertices.size());
}

void CDrawFigure::DrawBuffersLine()
{
    BeginVertexBuffer();
    glDrawArrays(GL_LINES, 0, 2);
    EndVertexBuffer();
}

void CDrawFigure::MakeBuffersTriangle()
{
    std::array<GLfloat, 9> vertices = {
        static_cast<GLfloat>(pnt[0].x.Val()),  static_cast<GLfloat>(pnt[0].y.Val()), 0.0f,
        static_cast<GLfloat>(pnt[1].x.Val()),  static_cast<GLfloat>(pnt[1].y.Val()), 0.0f,
        static_cast<GLfloat>(pnt[2].x.Val()),  static_cast<GLfloat>(pnt[2].y.Val()), 0.0f,
    };
    MakeBuffers(vertices.data(), vertices.size());
}

void CDrawFigure::DrawBuffersTriangle()
{
    BeginVertexBuffer();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    EndVertexBuffer();
}

void CDrawFigure::MakeBuffersRectangle()
{
    std::array<GLfloat, 9> vertices = {
        static_cast<GLfloat>(pnt[0].x.Val()),  static_cast<GLfloat>(pnt[0].y.Val()), 0.0f,
        static_cast<GLfloat>(pnt[1].x.Val()),  static_cast<GLfloat>(pnt[1].y.Val()), 0.0f,
        static_cast<GLfloat>(pnt[2].x.Val()),  static_cast<GLfloat>(pnt[2].y.Val()), 0.0f
    };
    MakeBuffers(vertices.data(), vertices.size());
}

void CDrawFigure::DrawBuffersRectangle()
{
    BeginVertexBuffer();
    glDrawArrays(GL_QUADS, 0, 4);
    EndVertexBuffer();
}

void CDrawFigure::MakeBuffersRectangleAxes()
{
    std::array<GLfloat, 12> vertices = {
        static_cast<GLfloat>((center.x+radius).Val()), static_cast<GLfloat>((center.y+radius1).Val()), 0.0f,
        static_cast<GLfloat>((center.x+radius).Val()), static_cast<GLfloat>((center.y-radius1).Val()), 0.0f,
        static_cast<GLfloat>((center.x-radius).Val()), static_cast<GLfloat>((center.y-radius1).Val()), 0.0f,
        static_cast<GLfloat>((center.x-radius).Val()), static_cast<GLfloat>((center.y+radius1).Val()), 0.0f,
    };
    MakeBuffers(vertices.data(), vertices.size());
}

void CDrawFigure::DrawBuffersRectangleAxes()
{
    BeginVertexBuffer();
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    EndVertexBuffer();
}
