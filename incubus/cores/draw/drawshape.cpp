#include "drawshape.h"

CDrawShape::CDrawShape(IDrawEnginePtr draw)
    : m_draw(draw)
{

}

void CDrawShape::SetShape(const CShape &shape)
{
    CShape my_shape = static_cast<CShape>(*this);
    if (my_shape == shape) return;
    *static_cast<CShape*>(this) = shape;
    m_draw_figures.clear();
    for (const auto &fig : m_figures)
    {
        m_draw_figures.push_back(CDrawFigurePtr(new CDrawFigure(m_draw, fig)));
    }
}

void CDrawShape::Draw()
{
    for (const auto &dfig : m_draw_figures)
    {
        dfig->lined = lined;
        dfig->line_width = line_width;
        dfig->Draw();
    }
}
