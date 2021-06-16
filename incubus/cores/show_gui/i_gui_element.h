#ifndef I_GUI_ELEMENT_H
#define I_GUI_ELEMENT_H

#include "common.h"

/**
 * @brief IGuiElement - интерфейс графического элемента управления
 */
class IGuiElement
{
public:
    virtual bool Draw() = 0;
    virtual bool onMouseEvent() = 0;
    GuiID_t GetID() const {return m_gui_id;}
private:
    GuiID_t     m_gui_id = GUI_ID_NONE;
};

using IGuiElementPtr = std::shared_ptr<IGuiElement>;

#endif // I_GUI_ELEMENT_H
