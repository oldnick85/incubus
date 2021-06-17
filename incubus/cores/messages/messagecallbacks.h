#ifndef ACTORCALLBACKS_H
#define ACTORCALLBACKS_H

#include "../dispatcher/actor.h"
#include "../logger/i_logger.h"
#include "messages.h"

/**
 * @brief CActorCallbacks - класс актора, содержащий колбеки на все приходящие сообщения.
 */
class CMessageCallbacks
        : public CActor
{
public:
    CMessageCallbacks(const ILoggerPtr log, const std::string &act_name) : CActor(act_name), m_logobj(log) {}
    virtual ~CMessageCallbacks() {}

    uint_32 MessagesReceived() const {return m_msgs_recv;}

    template<class Tmsg>
    const Tmsg* FromData(const void* data)
    {
        return reinterpret_cast<const Tmsg*>(data);
    }

    bool ActorCallbacks_onMessage();
protected:
    virtual bool onWindowClose(const bool close);
    virtual bool onMouseEvent(const sMsgWindowMouseEvent::enWindowMouseEvent event, const sCoord2 &pos, bool shift_pressed, bool control_pressed, bool alt_pressed);
    virtual bool onKeyEvent(const int key, const sMsgWindowKeyboardEvent::enWindowKeyboardEvent event, const sCoord2 &pos, bool shift_pressed, bool control_pressed, bool alt_pressed);
    virtual bool onNewObjectPhysics(const ObjID_t obj_id, const sObjSpec &spec, const sPhysicsParamMsg &ph_param);
    virtual bool onDelObjectPhysics(const ObjID_t obj_id);
    virtual bool onSetVelocity(const ObjID_t obj_id, const sVect2 &velocity, const sVect2 &direction);
    virtual bool onSetPhysicsState(const ObjID_t obj_id, const sPhysicsState &state);
    virtual bool onSetHealth(const ObjID_t obj_id, const sHealthParam &health);
    virtual bool onRequestPhysicsParamAll(const uint_8 flags);
    virtual bool onNewObjectBehavior(const ObjID_t obj_id, const sObjSpec &spec, const sObjBhParam &bh_param, const sPhysicsParamMsg &ph_param);
    virtual bool onDelObjectBehavior(const ObjID_t obj_id);
    virtual bool onSetKinematicParam(const ObjID_t obj_id, const sKinematicParam &ph_kn_param);
    virtual bool onNewObjectShow(const ObjID_t obj_id, const sObjSpec &spec, const sShowParam &sh_param, const sPhysicsParamMsg &ph_param);
    virtual bool onDelObjectShow(const ObjID_t obj_id);
    virtual bool onEndAnimations(const ObjID_t obj_id);
    virtual bool onDelObject(const ObjID_t obj_id);
    virtual bool onSetPosition(const ObjID_t obj_id, const sCoord2 &position);
    virtual bool onViewChange(const sVect2 &center_move, const Real_t &scale);
    virtual bool onGameplayModeChange(const sGameplayMode &gp_mode);
    virtual bool onGlobalPhysicsParam(const sGlobalPhysicsParam &ph_global);
    virtual bool onSetPhysicsGoal(const ObjID_t obj_id, const sPhysicsGoal &ph_goal);
    virtual bool onGameActionAttack(const ObjID_t obj_id, const sCoord2 &obj_pos, const sCoord2 &target_pos);
    virtual bool onCreateProjectile(const sCoord2 &position, const sVect2 &velocity);
    virtual bool onStatisticGp(const sCoreStatisticGp &gp_stat);
    virtual bool onStatisticPh(const sCoreStatisticPh &ph_stat);
    virtual bool onStatisticSh(const sCoreStatisticSh &sh_stat);
    virtual bool onStatisticBh(const sCoreStatisticBh &bh_stat);
    virtual bool onCoreShowParam(const sCoreShowParam &param);
    virtual bool onGoalEvent(const ObjID_t obj_id, const sPhysicsGoal &ph_goal);
    virtual bool onSoundEvent(const sSoundParam &sound_param);
    virtual bool onObjectAction(const sObjectAction &object_action);
    virtual bool onSetPhysicsParam(const ObjID_t obj_id, const sPhysicsParamMsg &ph_param);
    virtual bool onGuiControlAddButton(const GuiControlId_t id, const sGuiButtonParam &param);
    virtual bool onGuiControlDel(const GuiControlId_t id);
    virtual bool onGuiControlEvent(const GuiControlId_t id, const enGuiControlEvent event);
private:
    uint_32     m_msgs_recv;
    ILoggerPtr  m_logobj;
};

#endif // ACTORCALLBACKS_H
