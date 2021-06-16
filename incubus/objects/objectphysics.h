#ifndef OBJECTPHYSICS_H
#define OBJECTPHYSICS_H
#include "../logger/i_logger.h"
#include "object.h"
#include "cores/messages/messages.h"
#include "cores/messages/messagesender.h"
#include "objects/physics_obj_classes/iphobjclass.h"

class CObjectPhysics
        : public CObject
        , public CCoresParam
        , public CPhysicsParam
        , private CLogged
        , public CMessageSender
{
public:
    CObjectPhysics(const ILoggerPtr log, const IDispatcherPtr disp, sGlobalPhysicsParam* ph_global, IPhysicsOtherObjectsPtr other_objects, const sObjSpec &spec, sDispatcherMsgStatistic *msg_stat);
    CObjectPhysics(const CObjectPhysics &other) = delete;

    void SetNewPosition(const sCoord2 &pos);
    void SetHealthDamage(const Real_t damage_points);
    void SetNewHealth(const double health_points);
    virtual void onPositionChange(const sCoord2 &old_pos, const sCoord2 &new_pos);
    void onHealthChange();
    void onPhysicsStateChange();
    void onPhysicsParamChange();
    void SetGoal(const sPhysicsGoal &goal) {m_goal = goal;}
    sPhysicsGoal& Goal() {return m_goal;}
    void Destroy();
    void CheckGoal();

    void SendNewKinematic();
    void DoAction(const sObjectAction &object_action);

protected:
    IPhysicsOtherObjectsPtr         m_other_objects;

private:
    virtual void DoActionAttack(const sCoord2 &target_pos);
    void DoActionTake(const ObjID_t target_obj_id);

    std::shared_ptr<IPhObjClass>    m_class;
    sPhysicsGoal                    m_goal;
    sGlobalPhysicsParam*            m_ph_global;

    sKinematicParam                 m_kinematic_last_sent;
};

#endif // OBJECTPHYSICS_H
