#ifndef SHOWGUI_H
#define SHOWGUI_H

#include "../logger/i_logger.h"
#include "cores/messages/messagesender.h"
#include "cores/common_show.h"
#include "media/sound.h"
#include "guielementbutton.h"

class CShowGui
        : private CLogged
{
public:
    CShowGui(const ILoggerPtr log, CShowUtilPtr show_util, CSoundUtilPtr sound_util, IDrawEnginePtr draw);
    void AddButton(const GuiControlId_t id, const sGuiButtonParam &param);
    void Del(const GuiControlId_t id);
    void Draw();
    bool onMouse(const sCoord2 &mouse_pos);
private:
    CShowUtilPtr        m_show_util;
    CSoundUtilPtr       m_sound_util;
    IDrawEnginePtr      m_draw;

    std::unordered_map<uint, CGuiElementButtonPtr>  m_buttons;
};
using CShowGuiPtr = std::shared_ptr<CShowGui>;

#endif // SHOWGUI_H
