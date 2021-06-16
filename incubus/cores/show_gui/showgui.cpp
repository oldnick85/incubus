#include "showgui.h"

CShowGui::CShowGui(const ILoggerPtr log, CShowUtilPtr show_util, CSoundUtilPtr sound_util, IDrawEnginePtr draw)
    : CLogged(log)
    , m_show_util(show_util)
    , m_sound_util(sound_util)
    , m_draw(draw)
{

}

void CShowGui::AddButton(const GuiControlId_t id, const sGuiButtonParam &param)
{
    auto btn = CGuiElementButtonPtr(new CGuiElementButton(m_log, m_show_util, m_sound_util, m_draw));
    btn->SetParam(param);
    m_buttons[id] = btn;
}

void CShowGui::Del(const GuiControlId_t id)
{
    auto c_it = m_buttons.find(id);
    if (c_it != m_buttons.end())
    {
        m_buttons.erase(c_it);
    }
}

void CShowGui::Draw()
{
    for (auto &btn : m_buttons)
        btn.second->Draw();
}

bool CShowGui::onMouse(const sCoord2 &mouse_pos)
{
    bool event_catched = false;
    for (auto &btn : m_buttons)
        event_catched = event_catched or btn.second->onMouse(mouse_pos);
    return event_catched;
}
