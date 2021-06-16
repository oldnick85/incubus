#ifndef INCUBUS_COMMON_H
#define INCUBUS_COMMON_H

#include <string>
#include <unordered_map>
#include <math.h>
#include <cstring>
#include <cassert>

#include "../common/common.h"
#include "../dispatcher/dispatcher_common.h"

/**
 * @brief sAllCores - структура с идентификаторами акторов всех программных ядер
 */
struct sAllCores
{
    sActorID    gameplay;
    sActorID    physics;
    sActorID    behavior;
    sActorID    show;
};

// ======== OBJECT =====
typedef uint_32 ObjID_t;
#define OBJ_ID_NONE static_cast<ObjID_t>(-1)

// ======== SHOW ========
using GuiID_t = uint_32;

enum
{
    GUI_ID_NONE = static_cast<GuiID_t>(-1),
};


#endif // INCUBUS_COMMON_H
