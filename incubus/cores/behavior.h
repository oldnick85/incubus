#ifndef BEHAVIOR_H
#define BEHAVIOR_H
#include "../logger/i_logger.h"
#include "../dispatcher/actor.h"
#include "messages/messagecallbacks.h"
#include "messages/messagesender.h"
#include "../dispatcher/i_dispatcher.h"
#include "objects/objectbehavior.h"

class CBehavior
        : public CMessageCallbacks
        , public CLogged
        , public CCoresParam
        , public CMessageSender
{
public:
    CBehavior(const ILoggerPtr log, const IDispatcherPtr disp);
    virtual ~CBehavior() override;
    virtual void onMessage() override final;
    virtual void onTimer(const TimerID_t timer_id) override final;
    virtual void onStart()  override final;
    virtual void onStop()  override final;
    // CActorCallbacks
    virtual bool onNewObjectBehavior(const ObjID_t obj_id, const sObjSpec &spec, const sObjBhParam &bh_param, const sPhysicsParamMsg &ph_param) override final;
    virtual bool onDelObjectBehavior(const ObjID_t obj_id) override final;
    virtual bool onSetKinematicParam(const ObjID_t obj_id, const sKinematicParam &ph_kn_param) override final;
    virtual bool onGameActionAttack(const ObjID_t obj_id, const sCoord2 &obj_pos, const sCoord2 &target_pos) override final;
    virtual bool onSetHealth(const ObjID_t obj_id, const sHealthParam &health) override final;
    virtual bool onSetPhysicsState(const ObjID_t obj_id, const sPhysicsState &state) override final;
    virtual bool onObjectAction(const sObjectAction &object_action) override final;
    virtual bool onSetPhysicsParam(const ObjID_t obj_id, const sPhysicsParamMsg &ph_param) override final;
    virtual bool onGoalEvent(const ObjID_t obj_id, const sPhysicsGoal &ph_goal) override final;

private:
    bool DelObject(const ObjID_t &id);
    bool ObjectSetKinematic(const ObjID_t &id, const sKinematicParam &ph_param_kinematic);
    CObjectBehaviorPtr FindObject(const ObjID_t &id);

    TimerID_t       m_tm_1sec = TIMER_ID_NONE;
    TimerID_t       m_tm_100ms = TIMER_ID_NONE;
    uint_32         m_timer_count = 0;

    std::unordered_map<ObjID_t, CObjectBehaviorPtr> m_objects;

    sCoreStatisticBh    m_statistic;
};
#endif // BEHAVIOR_H
