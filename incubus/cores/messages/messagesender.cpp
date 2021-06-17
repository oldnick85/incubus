#include "messagesender.h"

CMessageSender::CMessageSender(IDispatcherPtr disp, sDispatcherMsgStatistic* stat)
    : m_msg_stat(stat)
    , m_disp(disp)
{

}

std::string CMessageSender::PrintSenderStatistic() const
{
    std::string stat_str;
    char s[128];
    for (uint i = msgNone; i < msgEndEnum; ++i)
    {
        uint v = m_msg_stat->msg_sent[i];
        if (v != 0)
        {
            snprintf(s, sizeof(s), "%s : %u\n", MessageType_print(static_cast<enMessageType>(i)), v);
            stat_str.append(s);
        }
    }
    return stat_str;
}

bool CMessageSender::Send_SetVelocity(const sActorID &sender, const sActorID &destination, const sVect2 &velocity, const sVect2 &direction)
{
    StatMsgSent(msgSetVelocity);
    sMsgSetVelocity msg_sv;
    msg_sv.obj_id = m_my_obj_id;
    msg_sv.velocity = velocity;
    msg_sv.direction = direction;
    SendMessage(sender, destination, msgSetVelocity, msg_sv);
    return true;
}

bool CMessageSender::Send_GameplayModeChange(const sActorID &destination, const sGameplayMode &gp_mode)
{
    StatMsgSent(msgGameplayModeChange);
    sMsgGameplayModeChange msg_pl;
    msg_pl.gp_mode = gp_mode;
    SendMessage(destination, msgGameplayModeChange, msg_pl);
    return true;
}

bool CMessageSender::Send_GlobalPhysicsParam(const sActorID &destination, const sGlobalPhysicsParam &ph_global)
{
    StatMsgSent(msgGlobalPhysicsParam);
    sMsgGlobalPhysicsParam msg_pl;
    msg_pl.ph_global = ph_global;
    SendMessage(destination, msgGlobalPhysicsParam, msg_pl);
    return true;
}

bool CMessageSender::Send_ViewChange(const sActorID &destination, const sVect2 &center_move, const Real_t scale)
{
    StatMsgSent(msgViewChange);
    sMsgViewChange msg_pl;
    msg_pl.center_move = center_move;
    msg_pl.scale = scale;
    SendMessage(destination, msgViewChange, msg_pl);
    return true;
}

bool CMessageSender::Send_SetKinematicParam(const sActorID &destination, const ObjID_t obj_id, const sKinematicParam &kinematic_param)
{
    StatMsgSent(msgSetKinematicParam);
    sMsgSetKinematicParam msg_pl;
    msg_pl.obj_id = obj_id;
    msg_pl.ph_kn_param = kinematic_param;
    SendMessage(destination, msgSetKinematicParam, msg_pl);
    return true;
}

bool CMessageSender::Send_SetPhysicsState(const sActorID &destination, const ObjID_t obj_id, const sPhysicsState &ph_state)
{
    StatMsgSent(msgSetPhysicsState);
    sMsgSetPhysicsState msg_pl;
    msg_pl.obj_id = obj_id;
    msg_pl.state = ph_state;
    SendMessage(destination, msgSetPhysicsState, msg_pl);
    return true;
}

bool CMessageSender::Send_SetHealth(const sActorID &destination, const ObjID_t obj_id, const sHealthParam &health_param)
{
    StatMsgSent(msgSetHealth);
    sMsgSetHealth msg_pl;
    msg_pl.obj_id = obj_id;
    msg_pl.health = health_param;
    SendMessage(destination, msgSetHealth, msg_pl);
    return true;
}

bool CMessageSender::Send_WindowMouseEvent(const sActorID &destination, const sMsgWindowMouseEvent::enWindowMouseEvent event, const sCoord2 &position)
{
    StatMsgSent(msgWindowMouseEvent);
    sMsgWindowMouseEvent msg_pl;
    msg_pl.event = event;
    msg_pl.pos = position;
    SendMessage(destination, msgWindowMouseEvent, msg_pl);
    return true;
}

bool CMessageSender::Send_WindowKeyboardEvent(const sActorID &destination, const int key, const sMsgWindowKeyboardEvent::enWindowKeyboardEvent event, const sCoord2 &position,
                              const bool shift_pressed, const bool control_pressed, const bool alt_pressed)
{
    StatMsgSent(msgWindowKeyboardEvent);
    sMsgWindowKeyboardEvent msg_pl;
    msg_pl.key = key;
    msg_pl.event = event;
    msg_pl.pos = position;
    msg_pl.shift_pressed = shift_pressed;
    msg_pl.control_pressed = control_pressed;
    msg_pl.alt_pressed = alt_pressed;
    SendMessage(destination, msgWindowKeyboardEvent, msg_pl);
    return true;
}

bool CMessageSender::Send_WindowClose(const sActorID &destination, const bool close)
{
    StatMsgSent(msgWindowClose);
    sMsgWindowClose msg_pl;
    msg_pl.close = close;
    SendMessage(destination, msgWindowClose, msg_pl);
    return true;
}

bool CMessageSender::Send_RequestPhysicsParamAll(const sActorID &destination, const bool kinematic)
{
    StatMsgSent(msgRequestPhysicsParamAll);
    sMsgRequestPhysicsParamAll msg_pl;
    msg_pl.flags = 0;
    if (kinematic)
        msg_pl.flags |= sMsgRequestPhysicsParamAll::enRequestFlags::rfKinematic;
    SendMessage(destination, msgRequestPhysicsParamAll, msg_pl);
    return true;
}

bool CMessageSender::Send_NewObjectPhysics(const sActorID &destination, const ObjID_t obj_id, const sObjSpec &spec, const sPhysicsParam &ph_param)
{
    StatMsgSent(msgNewObjectPhysics);
    sMsgNewObjectPhysics msg_pl_ph;
    msg_pl_ph.obj_id = obj_id;
    msg_pl_ph.spec = spec;
    ph_param.ToMsg(msg_pl_ph.param);
    SendMessage(destination, msgNewObjectPhysics, msg_pl_ph);
    return true;
}

bool CMessageSender::Send_NewObjectBehavior(const sActorID &destination, const ObjID_t obj_id, const sObjSpec &spec, const sBehaviorParam &bh_param, const sPhysicsParam &ph_param)
{
    StatMsgSent(msgNewObjectBehavior);
    sMsgNewObjectBehavior msg_pl_bh;
    msg_pl_bh.obj_id = obj_id;
    msg_pl_bh.spec = spec;
    msg_pl_bh.param.bh_param = bh_param;
    ph_param.ToMsg(msg_pl_bh.ph_param);
    SendMessage(destination, msgNewObjectBehavior, msg_pl_bh);
    return true;
}

bool CMessageSender::Send_NewObjectShow(const sActorID &destination, const ObjID_t obj_id, const sObjSpec &spec, const sShowParam &sh_param, const sPhysicsParam &ph_param)
{
    StatMsgSent(msgNewObjectShow);
    sMsgNewObjectShow msg_pl_ph;
    msg_pl_ph.obj_id = obj_id;
    msg_pl_ph.spec = spec;
    msg_pl_ph.sh_param = sh_param;
    ph_param.ToMsg(msg_pl_ph.ph_param);
    SendMessage(destination, msgNewObjectShow, msg_pl_ph);
    return true;
}

bool CMessageSender::Send_DelObjectPhysics(const sActorID &destination, const ObjID_t obj_id)
{
    StatMsgSent(msgDelObjectPhysics);
    sMsgDelObjectPhysics msg_pl_ph;
    msg_pl_ph.obj_id = obj_id;
    SendMessage(destination, msgDelObjectPhysics, msg_pl_ph);
    return true;
}

bool CMessageSender::Send_DelObjectBehavior(const sActorID &destination, const ObjID_t obj_id)
{
    StatMsgSent(msgDelObjectBehavior);
    sMsgDelObjectBehavior msg_pl_bh;
    msg_pl_bh.obj_id = obj_id;
    SendMessage(destination, msgDelObjectBehavior, msg_pl_bh);
    return true;
}

bool CMessageSender::Send_DelObject(const sActorID &destination, const ObjID_t obj_id)
{
    StatMsgSent(msgDelObject);
    sMsgDelObject msg_pl_ph;
    msg_pl_ph.obj_id = obj_id;
    SendMessage(destination, msgDelObject, msg_pl_ph);
    return true;
}

bool CMessageSender::Send_DelObjectShow(const sActorID &destination, const ObjID_t obj_id)
{
    StatMsgSent(msgDelObjectShow);
    sMsgDelObjectShow msg_pl_ph;
    msg_pl_ph.obj_id = obj_id;
    SendMessage(destination, msgDelObjectShow, msg_pl_ph);
    return true;
}

bool CMessageSender::Send_EndAnimations(const sActorID &destination, const ObjID_t obj_id)
{
    StatMsgSent(msgEndAnimations);
    sMsgEndAnimations msg_pl_ph;
    msg_pl_ph.obj_id = obj_id;
    SendMessage(destination, msgEndAnimations, msg_pl_ph);
    return true;
}

bool CMessageSender::Send_SetPhysicsGoal(const sActorID &destination, const ObjID_t obj_id, const sPhysicsGoal &goal)
{
    StatMsgSent(msgSetPhysicsGoal);
    sMsgSetPhysicsGoal msg_pl_ph;
    msg_pl_ph.obj_id = obj_id;
    msg_pl_ph.goal = goal;
    SendMessage(destination, msgSetPhysicsGoal, msg_pl_ph);
    return true;
}

bool CMessageSender::Send_SetPhysicsParam(const sActorID &destination, const ObjID_t obj_id, const sPhysicsParam &ph_param)
{
    StatMsgSent(msgSetPhysicsParam);
    sMsgSetPhysicsParam msg_pl_ph;
    msg_pl_ph.obj_id = obj_id;
    ph_param.ToMsg(msg_pl_ph.ph_param);
    SendMessage(destination, msgSetPhysicsParam, msg_pl_ph);
    return true;
}

bool CMessageSender::Send_GameActionAttack(const sActorID &destination, const ObjID_t obj_id, const sCoord2 &obj_pos, const sCoord2 &target_pos)
{
    StatMsgSent(msgGameActionAttack);
    sMsgGameActionAttack msg_pl;
    msg_pl.obj_id = obj_id;
    msg_pl.obj_pos = obj_pos;
    msg_pl.target_pos = target_pos;
    SendMessage(destination, msgGameActionAttack, msg_pl);
    return true;
}

bool CMessageSender::Send_CreateProjectile(const sActorID &destination, const sCoord2 &pos, const sVect2 &vel)
{
    StatMsgSent(msgCreateProjectile);
    sMsgCreateProjectile msg_pl;
    msg_pl.position = pos;
    msg_pl.velocity = vel;
    SendMessage(destination, msgCreateProjectile, msg_pl);
    return true;
}

bool CMessageSender::Send_StatisticGp(const sActorID &destination, const sCoreStatisticGp &gp_stat)
{
    StatMsgSent(msgStatisticGp);
    sMsgStatisticGp msg_pl;
    msg_pl.gp_stat = gp_stat;
    SendMessage(destination, msgStatisticGp, msg_pl);
    return true;
}

bool CMessageSender::Send_StatisticPh(const sActorID &destination, const sCoreStatisticPh &ph_stat)
{
    StatMsgSent(msgStatisticPh);
    sMsgStatisticPh msg_pl;
    msg_pl.ph_stat = ph_stat;
    SendMessage(destination, msgStatisticPh, msg_pl);
    return true;
}

bool CMessageSender::Send_StatisticSh(const sActorID &destination, const sCoreStatisticSh &sh_stat)
{
    StatMsgSent(msgStatisticSh);
    sMsgStatisticSh msg_pl;
    msg_pl.sh_stat = sh_stat;
    SendMessage(destination, msgStatisticSh, msg_pl);
    return true;
}

bool CMessageSender::Send_StatisticBh(const sActorID &destination, const sCoreStatisticBh &bh_stat)
{
    StatMsgSent(msgStatisticBh);
    sMsgStatisticBh msg_pl;
    msg_pl.bh_stat = bh_stat;
    SendMessage(destination, msgStatisticBh, msg_pl);
    return true;
}

bool CMessageSender::Send_CoreShowParam(const sActorID &destination, const sCoreShowParam param)
{
    StatMsgSent(msgCoreShowParam);
    sMsgCoreShowParam msg_pl;
    msg_pl.param = param;
    SendMessage(destination, msgCoreShowParam, msg_pl);
    return true;
}

bool CMessageSender::Send_GoalEvent(const sActorID &destination, const ObjID_t obj_id, const sPhysicsGoal &ph_goal)
{
    StatMsgSent(msgGoalEvent);
    sMsgGoalEvent msg_pl;
    msg_pl.obj_id = obj_id;
    msg_pl.goal = ph_goal;
    SendMessage(destination, msgGoalEvent, msg_pl);
    return true;
}

bool CMessageSender::Send_SoundEvent(const sActorID &destination, const sSoundParam &sound_param)
{
    StatMsgSent(msgSoundEvent);
    sMsgSoundEvent msg_pl;
    msg_pl.sound_param = sound_param;
    SendMessage(destination, msgSoundEvent, msg_pl);
    return true;
}

bool CMessageSender::Send_ObjectAction(const sActorID &destination, const sObjectAction &obj_act)
{
    StatMsgSent(msgObjectAction);
    sMsgObjectAction msg_pl;
    msg_pl.action = obj_act;
    SendMessage(destination, msgObjectAction, msg_pl);
    return true;
}

bool CMessageSender::Send_GuiControlAddButton(const sActorID &destination, const GuiControlId_t id, const sGuiButtonParam &param)
{
    StatMsgSent(msgGuiControlAddButton);
    sMsgGuiControlAddButton msg_pl;
    msg_pl.id = id;
    msg_pl.param = param;
    SendMessage(destination, msgGuiControlAddButton, msg_pl);
    return true;
}

bool CMessageSender::Send_GuiControlDel(const sActorID &destination, const GuiControlId_t id)
{
    StatMsgSent(msgGuiControlDel);
    sMsgGuiControlDel msg_pl;
    msg_pl.id = id;
    SendMessage(destination, msgGuiControlDel, msg_pl);
    return true;
}

bool CMessageSender::Send_GuiControlEvent(const sActorID &destination, const GuiControlId_t id, const enGuiControlEvent event)
{
    StatMsgSent(msgGuiControlEvent);
    sMsgGuiControlEvent msg_pl;
    msg_pl.id = id;
    msg_pl.event = event;
    SendMessage(destination, msgGuiControlEvent, msg_pl);
    return true;
}

void CMessageSender::StatMsgSent(const enMessageType msg_type)
{
    if (not m_msg_stat)
        assert(false);
    m_msg_stat->MsgSent(msg_type);
}

