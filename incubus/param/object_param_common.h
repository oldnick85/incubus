#ifndef OBJECT_PARAM_COMMON_H
#define OBJECT_PARAM_COMMON_H

#include "../incubus_common.h"

enum enGameplayMode
{
    gmEdit,
    gmPlay,
};

struct sGameplayMode
{
    std::string Print() const
    {
        char s[64];
        snprintf(s, sizeof(s), "%u; %u;", mode, player_id);
        return s;
    }

    enGameplayMode  mode = gmEdit;
    ObjID_t         player_id = OBJ_ID_NONE;
};

struct sObjGpParam
{
    std::string print() const
    {
        char s[MAX_STRING_SIZE];
        snprintf(s, sizeof(s), "[gp_param]");
        return s;
    }
};

struct sBehaviorParam
{

};

struct sObjBhParam
{
    std::string print() const
    {
        char s[MAX_STRING_SIZE];
        snprintf(s, sizeof(s), "[bh_param]");
        return s;
    }
    sBehaviorParam  bh_param;
};

#endif // OBJECT_PARAM_COMMON_H
