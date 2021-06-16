#include "guielementbutton.h"

CGuiElementButton::CGuiElementButton(const ILoggerPtr log, CShowUtilPtr show_util, CSoundUtilPtr sound_util, IDrawEnginePtr draw)
    : CLogged (log)
    , m_show_util(show_util)
    , m_sound_util(sound_util)
    , m_draw(draw)
{

}

void CGuiElementButton::SetParam(const sGuiButtonParam &param)
{
    m_param = param;
    const auto p1 = m_param.pos;
    const auto p2 = m_param.pos + m_param.size;
    CFigure fg;
    fg.MakeRectangleAxes(p1, p2);
    m_shape.Clear();
    m_shape.AddFigure(fg);
}

void CGuiElementButton::Draw()
{
    m_draw->Color4(0.7, 0.5, 0.5, 1.0);
    const auto p1 = m_param.pos;
    const auto p2 = m_param.pos + m_param.size;
    m_show_util->DrawRectAxes(p1, p2, true);
    if (m_mouse_over)
    {
        m_draw->Color4(0.7, 0.7, 0.5, 1.0);
        m_show_util->DrawRectAxes(p1, p2, false);
    }
    auto caption_x = m_param.pos.x;
    auto caption_y = m_param.pos.y + m_param.size.vy*0.5;
    m_show_util->DrawText(m_param.caption, caption_x, caption_y);
}

bool CGuiElementButton::onMouse(const sCoord2 &mouse_pos)
{
    m_mouse_over = m_shape.PointInside(mouse_pos);
    return m_mouse_over;
}
