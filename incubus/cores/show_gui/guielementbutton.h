#ifndef GUIELEMENTBUTTON_H
#define GUIELEMENTBUTTON_H

#include "param/gui_param.h"
#include "../logger/i_logger.h"
#include "cores/messages/messagesender.h"
#include "cores/common_show.h"
#include "media/sound.h"

class CGuiElementButton
        : private CLogged
{
public:
    CGuiElementButton(const ILoggerPtr log, CShowUtilPtr show_util, CSoundUtilPtr sound_util, IDrawEnginePtr draw);

    void SetParam(const sGuiButtonParam &param);

    void Draw();
    bool onMouse(const sCoord2 &mouse_pos);
private:
    CShowUtilPtr        m_show_util;
    CSoundUtilPtr       m_sound_util;
    IDrawEnginePtr      m_draw;

    sGuiButtonParam     m_param;
    CShape              m_shape;
    bool                m_mouse_over = false;
};
using CGuiElementButtonPtr = std::shared_ptr<CGuiElementButton>;

#endif // GUIELEMENTBUTTON_H
