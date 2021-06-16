#ifndef DISPATCHER_COMMON_H
#define DISPATCHER_COMMON_H

#include <chrono>
#include <string>
#include "../common/common.h"

using TimerID_t = int;
using ActID_t = uint_32;
using TimePoint_t = std::chrono::time_point<std::chrono::high_resolution_clock>;

enum
{
    TIMER_ID_NONE = static_cast<TimerID_t>(-1),
    ACT_ID_ALL = static_cast<ActID_t>(-2),
    ACT_ID_NONE = static_cast<ActID_t>(-1),
};

enum enActor : uint_16
{
    actNone,
    actGameplay,
    actPhysics,
    actBehavior,
    actShow,
};

inline const char* ActorType_print(const enActor type)
{
    switch (type)
    {
    case actNone :      return "none";
    case actGameplay :  return "gameplay";
    case actPhysics :   return "physics";
    case actBehavior :  return "behavior";
    case actShow :      return "show";
    }
    return "UNKNOWN";
}

/**
 * @brief sActorID - структура идентификатор актора
 */
struct sActorID
{
    bool IsValid() {return (id != ACT_ID_NONE);}

    std::string print() const
    {
        char s[128];
        snprintf(s, sizeof(s), "[actor:%s:%u]", ActorType_print(type), id);
        return s;
    }

    enActor     type    = enActor::actNone;     ///< тип актора
    ActID_t     id      = ACT_ID_NONE;          ///< идентификатор актора
};

#endif // DISPATCHER_COMMON_H
