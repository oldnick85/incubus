#ifndef GUIPARAM_H
#define GUIPARAM_H

#include "incubus_common.h"
#include "coordinates.h"

using GuiControlId_t = uint_32;
#define GuiControlId_NONE static_cast<GuiControlId_t>(-1)

enum enGuiControlEvent : uint_8
{
    gceNone,
    gcePress,
};

struct sGuiButtonParam
{
    sGuiButtonParam()
    {
        std::fill(std::begin(caption), std::end(caption), 0);
    }

    std::string print() const
    {
        char s[256];
        snprintf(s, sizeof(s), "button %s %s %s", pos.print().c_str(), size.print().c_str(), caption);
        return s;
    }

    sCoord2             pos;
    sVect2              size;
    char                caption[32];
};

#endif // GUIPARAM_H
