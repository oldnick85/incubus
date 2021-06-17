#include "messagecallbacks.h"

bool CMessageCallbacks::ActorCallbacks_onMessage()
{
    m_msgs_recv++;
    const auto data = m_message->msg.data();
    switch (m_message->msg_type)
    {
    case msgWindowClose:
    {
        const sMsgWindowClose* msg_pl = FromData<sMsgWindowClose>(data);
        return onWindowClose(msg_pl->close);
    }
    case msgWindowMouseEvent:
    {
        const sMsgWindowMouseEvent* msg_pl = FromData<sMsgWindowMouseEvent>(data);
        return onMouseEvent(msg_pl->event, msg_pl->pos, false, false, false);
    }
    case msgWindowKeyboardEvent:
    {
        const sMsgWindowKeyboardEvent* msg_pl = FromData<sMsgWindowKeyboardEvent>(data);
        return onKeyEvent(msg_pl->key, msg_pl->event, msg_pl->pos, msg_pl->shift_pressed, msg_pl->control_pressed, msg_pl->alt_pressed);
    }
    case msgNewObjectPhysics:
    {
        const sMsgNewObjectPhysics* msg_pl = FromData<sMsgNewObjectPhysics>(data);
        return onNewObjectPhysics(msg_pl->obj_id, msg_pl->spec, msg_pl->param);
    }
    case msgDelObjectPhysics:
    {
        const sMsgDelObjectPhysics* msg_pl = FromData<sMsgDelObjectPhysics>(data);
        return onDelObjectPhysics(msg_pl->obj_id);
    }
    case msgSetVelocity:
    {
        const sMsgSetVelocity* msg_pl = FromData<sMsgSetVelocity>(data);
        return onSetVelocity(msg_pl->obj_id, msg_pl->velocity, msg_pl->direction);
    }
    case msgSetPhysicsState:
    {
        const sMsgSetPhysicsState* msg_pl = FromData<sMsgSetPhysicsState>(data);
        return onSetPhysicsState(msg_pl->obj_id, msg_pl->state);
    }
    case msgSetHealth:
    {
        const sMsgSetHealth* msg_pl = FromData<sMsgSetHealth>(data);
        return onSetHealth(msg_pl->obj_id, msg_pl->health);
    }
    case msgRequestPhysicsParamAll:
    {
        const sMsgRequestPhysicsParamAll* msg_pl = FromData<sMsgRequestPhysicsParamAll>(data);
        return onRequestPhysicsParamAll(msg_pl->flags);
    }
    case msgNewObjectBehavior:
    {
        const sMsgNewObjectBehavior* msg_pl = FromData<sMsgNewObjectBehavior>(data);
        return onNewObjectBehavior(msg_pl->obj_id, msg_pl->spec, msg_pl->param, msg_pl->ph_param);
    }
    case msgDelObjectBehavior:
    {
        const sMsgDelObjectBehavior* msg_pl = FromData<sMsgDelObjectBehavior>(data);
        return onDelObjectBehavior(msg_pl->obj_id);
    }
    case msgSetKinematicParam:
    {
        const sMsgSetKinematicParam* msg_pl = FromData<sMsgSetKinematicParam>(data);
        return onSetKinematicParam(msg_pl->obj_id, msg_pl->ph_kn_param);
    }
    case msgNewObjectShow:
    {
        const sMsgNewObjectShow* msg_pl = FromData<sMsgNewObjectShow>(data);
        return onNewObjectShow(msg_pl->obj_id, msg_pl->spec, msg_pl->sh_param, msg_pl->ph_param);
    }
    case msgDelObjectShow:
    {
        const sMsgDelObjectShow* msg_pl = FromData<sMsgDelObjectShow>(data);
        return onDelObjectShow(msg_pl->obj_id);
    }
    case msgEndAnimations:
    {
        const sMsgEndAnimations* msg_pl = FromData<sMsgEndAnimations>(data);
        return onEndAnimations(msg_pl->obj_id);
    }
    case msgDelObject:
    {
        const sMsgDelObject* msg_pl = FromData<sMsgDelObject>(data);
        return onDelObject(msg_pl->obj_id);
    }
    case msgSetPosition:
    {
        const sMsgSetPosition* msg_pl = FromData<sMsgSetPosition>(data);
        return onSetPosition(msg_pl->obj_id, msg_pl->position);
    }
    case msgViewChange:
    {
        const sMsgViewChange* msg_pl = FromData<sMsgViewChange>(data);
        return onViewChange(msg_pl->center_move, msg_pl->scale);
    }
    case msgGameplayModeChange:
    {
        const sMsgGameplayModeChange* msg_pl = FromData<sMsgGameplayModeChange>(data);
        return onGameplayModeChange(msg_pl->gp_mode);
    }
    case msgGlobalPhysicsParam:
    {
        const sMsgGlobalPhysicsParam* msg_pl = FromData<sMsgGlobalPhysicsParam>(data);
        return onGlobalPhysicsParam(msg_pl->ph_global);
    }
    case msgSetPhysicsGoal:
    {
        const sMsgSetPhysicsGoal* msg_pl = FromData<sMsgSetPhysicsGoal>(data);
        return onSetPhysicsGoal(msg_pl->obj_id, msg_pl->goal);
    }
    case msgGameActionAttack:
    {
        const sMsgGameActionAttack* msg_pl = FromData<sMsgGameActionAttack>(data);
        return onGameActionAttack(msg_pl->obj_id, msg_pl->obj_pos, msg_pl->target_pos);
    }
    case msgCreateProjectile:
    {
        const sMsgCreateProjectile* msg_pl = FromData<sMsgCreateProjectile>(data);
        return onCreateProjectile(msg_pl->position, msg_pl->velocity);
    }
    case msgStatisticGp:
    {
        const sMsgStatisticGp* msg_pl = FromData<sMsgStatisticGp>(data);
        return onStatisticGp(msg_pl->gp_stat);
    }
    case msgStatisticPh:
    {
        const sMsgStatisticPh* msg_pl = FromData<sMsgStatisticPh>(data);
        return onStatisticPh(msg_pl->ph_stat);
    }
    case msgStatisticSh:
    {
        const sMsgStatisticSh* msg_pl = FromData<sMsgStatisticSh>(data);
        return onStatisticSh(msg_pl->sh_stat);
    }
    case msgStatisticBh:
    {
        const sMsgStatisticBh* msg_pl = FromData<sMsgStatisticBh>(data);
        return onStatisticBh(msg_pl->bh_stat);
    }
    case msgCoreShowParam:
    {
        const sMsgCoreShowParam* msg_pl = FromData<sMsgCoreShowParam>(data);
        return onCoreShowParam(msg_pl->param);
    }
    case msgGoalEvent:
    {
        const sMsgGoalEvent* msg_pl = FromData<sMsgGoalEvent>(data);
        return onGoalEvent(msg_pl->obj_id, msg_pl->goal);
    }
    case msgSoundEvent:
    {
        const sMsgSoundEvent* msg_pl = FromData<sMsgSoundEvent>(data);
        return onSoundEvent(msg_pl->sound_param);
    }
    case msgObjectAction:
    {
        const sMsgObjectAction* msg_pl = FromData<sMsgObjectAction>(data);
        return onObjectAction(msg_pl->action);
    }
    case msgSetPhysicsParam:
    {
        const sMsgSetPhysicsParam* msg_pl = FromData<sMsgSetPhysicsParam>(data);
        return onSetPhysicsParam(msg_pl->obj_id, msg_pl->ph_param);
    }
    case msgGuiControlAddButton:
    {
        const sMsgGuiControlAddButton* msg_pl = FromData<sMsgGuiControlAddButton>(data);
        return onGuiControlAddButton(msg_pl->id, msg_pl->param);
    }
    case msgGuiControlDel:
    {
        const sMsgGuiControlDel* msg_pl = FromData<sMsgGuiControlDel>(data);
        return onGuiControlDel(msg_pl->id);
    }
    case msgGuiControlEvent:
    {
        const sMsgGuiControlEvent* msg_pl = FromData<sMsgGuiControlEvent>(data);
        return onGuiControlEvent(msg_pl->id, msg_pl->event);
    }
    default:
        return false;
    }
}

bool CMessageCallbacks::onWindowClose(const bool close)
{
    logobj_debug1("%s %u;", __FUNCTION__, close);
    return false;
}

bool CMessageCallbacks::onMouseEvent(const sMsgWindowMouseEvent::enWindowMouseEvent event, const sCoord2 &pos, bool shift_pressed, bool control_pressed, bool alt_pressed)
{
    logobj_debug1("%s %u; %s; %u%u%u;", __FUNCTION__, event, pos.print().c_str(), shift_pressed, control_pressed, alt_pressed);
    return false;
}

bool CMessageCallbacks::onKeyEvent(const int key, const sMsgWindowKeyboardEvent::enWindowKeyboardEvent event, const sCoord2 &pos, bool shift_pressed, bool control_pressed, bool alt_pressed)
{
    logobj_debug1("%s %d %u; %s; %u%u%u;", __FUNCTION__, key, event, pos.print().c_str(), shift_pressed, control_pressed, alt_pressed);
    return false;
}

bool CMessageCallbacks::onNewObjectPhysics(const ObjID_t obj_id, const sObjSpec &spec, const sPhysicsParamMsg &ph_param)
{
    logobj_debug1("%s %u; %s; %s;", __FUNCTION__, obj_id, spec.Print().c_str(), ph_param.print().c_str());
    return false;
}

bool CMessageCallbacks::onDelObjectPhysics(const ObjID_t obj_id)
{
    logobj_debug1("%s %u;", __FUNCTION__, obj_id);
    return false;
}

bool CMessageCallbacks::onSetVelocity(const ObjID_t obj_id, const sVect2 &velocity, const sVect2 &direction)
{
    logobj_debug1("%s %u; %s; %s;", __FUNCTION__, obj_id, velocity.print().c_str(), direction.print().c_str());
    return false;
}

bool CMessageCallbacks::onSetPhysicsState(const ObjID_t obj_id, const sPhysicsState &state)
{
    logobj_debug1("%s %u; %s;", __FUNCTION__, obj_id, state.print().c_str());
    return false;
}

bool CMessageCallbacks::onSetHealth(const ObjID_t obj_id, const sHealthParam &health)
{
    logobj_debug1("%s %u; %s;", __FUNCTION__, obj_id, health.Print().c_str());
    return false;
}

bool CMessageCallbacks::onRequestPhysicsParamAll(const uint_8 flags)
{
    logobj_debug1("%s %u;", __FUNCTION__, flags);
    return false;
}

bool CMessageCallbacks::onNewObjectBehavior(const ObjID_t obj_id, const sObjSpec &spec, const sObjBhParam &bh_param, const sPhysicsParamMsg &ph_param)
{
    logobj_debug1("%s %u; %s; %s; %s;",
               __FUNCTION__, obj_id, spec.Print().c_str(), bh_param.print().c_str(), ph_param.print().c_str());
    return false;
}

bool CMessageCallbacks::onDelObjectBehavior(const ObjID_t obj_id)
{
    logobj_debug1("%s %u;", __FUNCTION__, obj_id);
    return false;
}

bool CMessageCallbacks::onSetKinematicParam(const ObjID_t obj_id, const sKinematicParam &ph_kn_param)
{
    logobj_debug1("%s %u; %s;", __FUNCTION__, obj_id, ph_kn_param.print().c_str());
    return false;
}

bool CMessageCallbacks::onNewObjectShow(const ObjID_t obj_id, const sObjSpec &spec, const sShowParam &sh_param, const sPhysicsParamMsg &ph_param)
{
    logobj_debug1("%s %u; %s; %s; %s;",
               __FUNCTION__, obj_id, spec.Print().c_str(), sh_param.print().c_str(), ph_param.print().c_str());
    return false;
}

bool CMessageCallbacks::onDelObjectShow(const ObjID_t obj_id)
{
    logobj_debug1("%s %u;", __FUNCTION__, obj_id);
    return false;
}

bool CMessageCallbacks::onSetPosition(const ObjID_t obj_id, const sCoord2 &position)
{
    logobj_debug1("%s %u; %s;", __FUNCTION__, obj_id, position.print().c_str());
    return false;
}

bool CMessageCallbacks::onViewChange(const sVect2 &center_move, const Real_t &scale)
{
    logobj_debug1("%s %s; %f;", __FUNCTION__, center_move.print().c_str(), scale.Val());
    return false;
}

bool CMessageCallbacks::onGameplayModeChange(const sGameplayMode &gp_mode)
{
    logobj_debug1("%s %s;", __FUNCTION__, gp_mode.Print().c_str());
    return false;
}

bool CMessageCallbacks::onGlobalPhysicsParam(const sGlobalPhysicsParam &ph_global)
{
    logobj_debug1("%s; %s;", __FUNCTION__, ph_global.Print().c_str());
    return false;
}

bool CMessageCallbacks::onSetPhysicsGoal(const ObjID_t obj_id, const sPhysicsGoal &ph_goal)
{
    logobj_debug1("%s; %u; %s;", __FUNCTION__, obj_id, ph_goal.Print().c_str());
    return false;
}

bool CMessageCallbacks::onGameActionAttack(const ObjID_t obj_id, const sCoord2 &obj_pos, const sCoord2 &target_pos)
{
    logobj_debug1("%s; %u; %s; %s;", __FUNCTION__, obj_id, obj_pos.print().c_str(), target_pos.print().c_str());
    return false;
}

bool CMessageCallbacks::onStatisticGp(const sCoreStatisticGp &gp_stat)
{
    logobj_debug1("%s; %s;", __FUNCTION__, gp_stat.print().c_str());
    return false;
}

bool CMessageCallbacks::onStatisticPh(const sCoreStatisticPh &ph_stat)
{
    logobj_debug1("%s; %s;", __FUNCTION__, ph_stat.print().c_str());
    return false;
}

bool CMessageCallbacks::onStatisticSh(const sCoreStatisticSh &sh_stat)
{
    logobj_debug1("%s; %s;", __FUNCTION__, sh_stat.print().c_str());
    return false;
}

bool CMessageCallbacks::onStatisticBh(const sCoreStatisticBh &bh_stat)
{
    logobj_debug1("%s; %s;", __FUNCTION__, bh_stat.print().c_str());
    return false;
}

bool CMessageCallbacks::onCoreShowParam(const sCoreShowParam &param)
{
    logobj_debug1("%s; %s;", __FUNCTION__, param.Print().c_str());
    return false;
}

bool CMessageCallbacks::onGoalEvent(const ObjID_t obj_id, const sPhysicsGoal &ph_goal)
{
    logobj_debug1("%s %u; %s;", __FUNCTION__, obj_id, ph_goal.Print().c_str());
    return false;
}

bool CMessageCallbacks::onSoundEvent(const sSoundParam &sound_param)
{
    logobj_debug1("%s %s;", __FUNCTION__, sound_param.Print().c_str());
    return false;
}

bool CMessageCallbacks::onObjectAction(const sObjectAction &object_action)
{
    logobj_debug1("%s %s;", __FUNCTION__, object_action.Print().c_str());
    return false;
}

bool CMessageCallbacks::onSetPhysicsParam(const ObjID_t obj_id, const sPhysicsParamMsg &ph_param)
{
    logobj_debug1("%s %u; %s;", __FUNCTION__, obj_id, ph_param.print().c_str());
    return false;
}

bool CMessageCallbacks::onEndAnimations(const ObjID_t obj_id)
{
    logobj_debug1("%s %u;", __FUNCTION__, obj_id);
    return false;
}

bool CMessageCallbacks::onDelObject(const ObjID_t obj_id)
{
    logobj_debug1("%s %u;", __FUNCTION__, obj_id);
    return false;
}

bool CMessageCallbacks::onCreateProjectile(const sCoord2 &position, const sVect2 &velocity)
{
    logobj_debug1("%s %s; %s", __FUNCTION__, position.print().c_str(), velocity.print().c_str());
    return false;
}

bool CMessageCallbacks::onGuiControlAddButton(const GuiControlId_t id, const sGuiButtonParam &param)
{
    logobj_debug1("%s %u; %s;", __FUNCTION__, id, param.print().c_str());
    return false;
}

bool CMessageCallbacks::onGuiControlDel(const GuiControlId_t id)
{
    logobj_debug1("%s %u;", __FUNCTION__, id);
    return false;
}

bool CMessageCallbacks::onGuiControlEvent(const GuiControlId_t id, const enGuiControlEvent event)
{
    logobj_debug1("%s %u; %u;", __FUNCTION__, id, event);
    return false;
}
