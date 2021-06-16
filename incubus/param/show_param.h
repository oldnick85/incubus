#ifndef SHOW_PARAM_H
#define SHOW_PARAM_H

#include "../incubus_common.h"

struct sShowParam
{
    enum enShowHeight
    {
        shGround,
        shOnGround,
        shStay,
        shFly,
        shTop,
        shInterface,
    };

    std::string print() const
    {
        char s[MAX_STRING_SIZE];
        snprintf(s, sizeof(s), "[sh_param height=%u; trace_ttl=%.3f;]",
                 sh_height, trace_ttl);
        return s;
    }

    enShowHeight    sh_height = shGround;
    double  trace_ttl = 0.0;
};

struct sGlobalShowParam
{
    bool HeightGround() const {return (current_sh_height == sShowParam::enShowHeight::shGround);}
    bool HeightOnGround() const {return (current_sh_height == sShowParam::enShowHeight::shOnGround);}
    bool HeightStay() const {return (current_sh_height == sShowParam::enShowHeight::shStay);}
    bool HeightFly() const {return (current_sh_height == sShowParam::enShowHeight::shFly);}
    bool HeightTop() const {return (current_sh_height == sShowParam::enShowHeight::shTop);}
    bool HeightInterface() const {return (current_sh_height == sShowParam::enShowHeight::shInterface);}

    sShowParam::enShowHeight    current_sh_height = sShowParam::enShowHeight::shGround;
    double                      current_z = 0.0;
};

struct sCoreShowParam
{
    std::string Print() const
    {
        char s[64];
        snprintf(s, sizeof(s), "%u%u%u%u%u%u", show_shape, show_velocity, show_direction, show_graphics, show_fps, show_statistic);
        return s;
    }

    enum enFlag
    {
        scShape,
        scVelocity,
        scDirection,
        scGraphics,
        scFPS,
        scStatistic,
    };

    void SetFlag(const enFlag flag, const bool value)
    {
        switch (flag)
        {
        case scShape:       show_shape = value;         break;
        case scVelocity:    show_velocity = value;      break;
        case scDirection:   show_direction = value;     break;
        case scGraphics:    show_graphics = value;      break;
        case scFPS:         show_fps = value;           break;
        case scStatistic:   show_statistic = value;     break;
        }

    }

    bool ShowShape() const      {return show_shape;}
    bool ShowVelocity() const   {return show_velocity;}
    bool ShowDirection() const  {return show_direction;}
    bool ShowGraphics() const   {return show_graphics;}
    bool ShowFPS() const        {return show_fps;}
    bool ShowStatistic() const  {return show_statistic;}

    bool        show_shape = true;
    bool        show_velocity = true;
    bool        show_direction = true;
    bool        show_graphics = true;
    bool        show_fps = true;
    bool        show_statistic = false;
};

#endif // SHOW_PARAM_H
