#ifndef MESSAGES_H
#define MESSAGES_H

#include <i_actor.h>
#include "incubus_common.h"
#include "objects/common_obj.h"
#include "param/physics_param.h"
#include "param/show_param.h"
#include "param/gui_param.h"
#include "media/mediasound.h"

/**
 * @brief enMessageType - типы сообщений, пересылаемых через диспетчер
 */
enum enMessageType : uint_16
{
    msgNone                 = 0,
    msgNewObjectGameplay,
    msgDelObjectGameplay,
    msgNewObjectPhysics,
    msgDelObjectPhysics,
    msgNewObjectBehavior,
    msgDelObjectBehavior,
    msgNewObjectShow,
    msgDelObjectShow,
    msgEndAnimations,
    msgDelObject,

    msgSetVelocity,
    msgSetPhysicsState,
    msgSetPosition,
    msgSetKinematicParam,
    msgSetPhysicsParam,
    msgSetShape,
    msgSetPhysicsGoal,
    msgSetHealth,

    msgRequestPhysicsParamAll,

    msgWindowClose,
    msgWindowMouseEvent,
    msgWindowKeyboardEvent,

    msgViewChange,
    msgGameplayModeChange,
    msgGlobalPhysicsParam,

    msgGameActionAttack,
    msgCreateProjectile,

    msgStatisticGp,
    msgStatisticPh,
    msgStatisticSh,
    msgStatisticBh,

    msgCoreShowParam,
    msgCoreShowParamFlag,

    msgGoalEvent,

    msgSoundEvent,

    msgObjectAction,

    msgGuiControlAddButton,
    msgGuiControlDel,
    msgGuiControlEvent,

    msgEndEnum,
};

inline const char* MessageType_print(const enMessageType type)
{
    switch (type)
    {
    case msgNone                    : return "None";
    case msgNewObjectGameplay       : return "NewObjectGameplay";
    case msgDelObjectGameplay       : return "NewObjectGameplay";
    case msgNewObjectPhysics        : return "NewObjectPhysics";
    case msgDelObjectPhysics        : return "DelObjectPhysics";
    case msgNewObjectBehavior       : return "NewObjectBehavior";
    case msgDelObjectBehavior       : return "DelObjectBehavior";
    case msgNewObjectShow           : return "NewObjectShow";
    case msgDelObjectShow           : return "DelObjectShow";
    case msgEndAnimations           : return "EndAnimations";
    case msgDelObject               : return "DelObject";
    case msgSetVelocity             : return "SetVelocity";
    case msgSetPhysicsState         : return "SetPhysicsState";
    case msgSetPosition             : return "SetPosition";
    case msgSetKinematicParam       : return "SetKinematicParam";
    case msgSetPhysicsParam         : return "SetPhysicsParam";
    case msgSetShape                : return "SetShape";
    case msgSetPhysicsGoal          : return "SetPhysicsGoal";
    case msgSetHealth               : return "SetHealth";
    case msgRequestPhysicsParamAll  : return "RequestPhysicsParamAll";
    case msgWindowClose             : return "WindowClose";
    case msgWindowMouseEvent        : return "WindowMouseEvent";
    case msgWindowKeyboardEvent     : return "WindowKeyboardEvent";
    case msgViewChange              : return "ViewChange";
    case msgGameplayModeChange      : return "GameplayModeChange";
    case msgGlobalPhysicsParam      : return "GlobalPhysicsParam";
    case msgGameActionAttack        : return "GameActionAttack";
    case msgCreateProjectile        : return "CreateProjectile";
    case msgStatisticGp             : return "StatisticGp";
    case msgStatisticPh             : return "StatisticPh";
    case msgStatisticSh             : return "StatisticSh";
    case msgStatisticBh             : return "StatisticBh";
    case msgCoreShowParam           : return "CoreShowParam";
    case msgCoreShowParamFlag       : return "CoreShowParamFlag";
    case msgGoalEvent               : return "GoalEvent";
    case msgSoundEvent              : return "SoundEvent";
    case msgObjectAction            : return "ObjectAction";
    case msgEndEnum                 : return "EndEnum";
    case msgGuiControlAddButton     : return "GuiControlAddButton";
    case msgGuiControlDel           : return "GuiControlDel";
    case msgGuiControlEvent         : return "GuiControlEvent";
    }
    assert(false);
}

struct sDispatcherMsgStatisticPlain
{
    void FromStat(const sDispatcherMsgStatistic &stat)
    {
        msgs_sent = stat.msgs_sent;
        for (auto &s : msg_sent)
        {
            s.first = 0;
            s.second = 0;
        }
        uint i = 0;
        for (const auto &s : stat.msg_sent)
        {
            msg_sent[i] = s;
            i++;
        }
    }

    void ToStat(sDispatcherMsgStatistic &stat) const
    {
        stat.msgs_sent = msgs_sent;
        stat.msg_sent.clear();

        for (const auto &s : msg_sent)
        {
            if (s.first != 0)
                stat.msg_sent[s.first] = s.second;
        }
    }

    uint                                    msgs_sent = 0;
    std::array<std::pair<uint, uint>, 256>  msg_sent;
};

struct sCoreStatisticCommon
{
    std::string print() const
    {
        char s[256];
        snprintf(s, sizeof(s), "objects=%u; msg_sent=%u;",
                 objects_count, msg_stat.msgs_sent);
        return s;
    }

    sDispatcherMsgStatisticPlain    msg_stat;
    uint_32                         objects_count = 0;
};

struct sCoreStatisticGp
{
    std::string print() const
    {
        char s[256];
        snprintf(s, sizeof(s), "%s", common.print().c_str());
        return s;
    }

    sCoreStatisticCommon    common;
};

struct sCoreStatisticPh
{
    std::string print() const
    {
        char s[256];
        snprintf(s, sizeof(s), "%s", common.print().c_str());
        return s;
    }

    sCoreStatisticCommon    common;
};

struct sCoreStatisticSh
{
    std::string print() const
    {
        char s[256];
        snprintf(s, sizeof(s), "%s %s",
                 common.print().c_str(), sound.print().c_str());
        return s;
    }

    sCoreStatisticCommon    common;
    sCoreStatisticShSound   sound;
};

struct sCoreStatisticBh
{
    std::string print() const
    {
        char s[256];
        snprintf(s, sizeof(s), "%s", common.print().c_str());
        return s;
    }

    sCoreStatisticCommon    common;
};

struct sMsgNewObjectGameplay
{
    sObjGpParam         param;
    sObjSpec            spec;
};

struct sMsgDelObjectGameplay
{
    ObjID_t         obj_id = OBJ_ID_NONE;
};

struct sMsgNewObjectPhysics
{
    ObjID_t             obj_id = OBJ_ID_NONE;
    sObjSpec            spec;
    sPhysicsParamMsg    param;
};

struct sMsgDelObjectPhysics
{
    ObjID_t         obj_id = OBJ_ID_NONE;
};

struct sMsgNewObjectBehavior
{
    ObjID_t             obj_id = OBJ_ID_NONE;
    sObjSpec            spec;
    sObjBhParam         param;
    sPhysicsParamMsg    ph_param;
};

struct sMsgDelObjectBehavior
{
    ObjID_t         obj_id = OBJ_ID_NONE;
};

struct sMsgNewObjectShow
{
    ObjID_t             obj_id = OBJ_ID_NONE;
    sObjSpec            spec;
    sShowParam          sh_param;
    sPhysicsParamMsg    ph_param;
};

struct sMsgDelObject
{
    ObjID_t         obj_id = OBJ_ID_NONE;
};

struct sMsgDelObjectShow
{
    ObjID_t         obj_id = OBJ_ID_NONE;
};

struct sMsgEndAnimations
{
    ObjID_t         obj_id = OBJ_ID_NONE;
};

struct sMsgSetPhysicsState
{
    ObjID_t         obj_id = OBJ_ID_NONE;
    sPhysicsState   state;
};

struct sMsgSetVelocity
{
    ObjID_t         obj_id = OBJ_ID_NONE;
    sVect2          velocity;
    sVect2          direction;
};

struct sMsgSetHealth
{
    ObjID_t         obj_id = OBJ_ID_NONE;
    sHealthParam    health;
};

struct sMsgSetPosition
{
    std::string print() const
    {
        char s[64];
        snprintf(s, sizeof (s), "MsgSetPosition-%u-%s",
                 obj_id, position.print().c_str());
        return s;
    }

    ObjID_t         obj_id = OBJ_ID_NONE;
    sCoord2         position;
};

struct sMsgSetKinematicParam
{
    ObjID_t         obj_id = OBJ_ID_NONE;
    sKinematicParam ph_kn_param;
};

struct sMsgSetHealthParam
{
    ObjID_t         obj_id = OBJ_ID_NONE;
    sHealthParam    ph_hl_param;
};

struct sMsgSetShape
{
    ObjID_t         obj_id = OBJ_ID_NONE;
    CFigure         figures[4];
};

struct sMsgSetPhysicsGoal
{
    ObjID_t         obj_id = OBJ_ID_NONE;
    sPhysicsGoal    goal;
};

struct sMsgSetPhysicsParam
{
    ObjID_t             obj_id = OBJ_ID_NONE;
    sPhysicsParamMsg    ph_param;
};

struct sMsgGameActionAttack
{
    ObjID_t         obj_id = OBJ_ID_NONE;
    sCoord2         obj_pos;
    sCoord2         target_pos;
};

struct sMsgRequestPhysicsParamAll
{
    enum enRequestFlags
    {
        rfKinematic     = 1,
        rfMaterial      = 2,
    };

    uint_8          flags = 0;
};

struct sMsgWindowClose
{
    bool    close = true;
};

struct sMsgWindowMouseEvent
{
    enum enWindowMouseEvent
    {
        evButtonLeftDown,
        evButtonLeftUp,
        evButtonRightDown,
        evButtonRightUp,
    };
    enWindowMouseEvent  event;
    sCoord2             pos;
};

struct sMsgWindowKeyboardEvent
{
    enum enWindowKeyboardEvent
    {
        evKeyDown,
        evKeyUp,
    };
    enWindowKeyboardEvent   event;
    int                     key;
    bool                    shift_pressed = false;
    bool                    control_pressed = false;
    bool                    alt_pressed = false;
    sCoord2                 pos;
};

struct sMsgViewChange
{
    sVect2      center_move;
    Real_t      scale = 1.0;
};

struct sMsgGameplayModeChange
{
    sGameplayMode   gp_mode;
};

struct sMsgGlobalPhysicsParam
{
    sGlobalPhysicsParam ph_global;
};

struct sMsgCreateProjectile
{
    sCoord2     position;
    sVect2      velocity;
};

struct sMsgStatisticGp
{
    sCoreStatisticGp    gp_stat;
};

struct sMsgStatisticPh
{
    sCoreStatisticPh    ph_stat;
};

struct sMsgStatisticSh
{
    sCoreStatisticSh    sh_stat;
};

struct sMsgStatisticBh
{
    sCoreStatisticBh    bh_stat;
};

struct sMsgCoreShowParam
{
    sCoreShowParam      param;
};

struct sMsgSetCoreShowParamFlag
{
    sCoreShowParam::enFlag  flag;
    bool                    value;
};

struct sMsgGoalEvent
{
    enum enGoalEvent
    {
        peNone,
        peGoalAchieved,
        peGoalUnattainable,
    };

    ObjID_t         obj_id = OBJ_ID_NONE;
    sPhysicsGoal    goal;
};

struct sMsgHookEvent
{
    ObjID_t         obj_id = OBJ_ID_NONE;
    sPhysicsHook    hook;
};

struct sMsgSoundEvent
{
    sSoundParam     sound_param;
};

enum enObjectActionType
{
    oaNone,
    oaAttack,
    oaTake,
};

struct sObjectAction
{
    std::string Print() const
    {
        char s[128];
        snprintf(s, sizeof(s), "%u; %u; %s; %u;", obj_id, type, pos.print().c_str(), targ_obj_id);
        return s;
    }

    void SetAttack(const ObjID_t o_id, const sCoord2 &target_pos) { type = oaAttack; obj_id = o_id; pos = target_pos;}
    void SetTake(const ObjID_t o_id, const ObjID_t target_obj) { type = oaTake; obj_id = o_id; targ_obj_id = target_obj;}

    ObjID_t             obj_id = OBJ_ID_NONE;
    enObjectActionType  type = oaNone;
    sCoord2             pos;
    ObjID_t             targ_obj_id = OBJ_ID_NONE;
};

struct sMsgObjectAction
{
    sObjectAction       action;
};

struct sMsgGuiControlAddButton
{
    GuiControlId_t      id = GuiControlId_NONE;
    sGuiButtonParam     param;
};

struct sMsgGuiControlDel
{
    GuiControlId_t      id;
};

struct sMsgGuiControlEvent
{
    GuiControlId_t      id;
    enGuiControlEvent   event;
};

#endif // MESSAGES_H
