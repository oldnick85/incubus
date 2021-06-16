#ifndef PHYSICS_H
#define PHYSICS_H
#include "incubus_common.h"
#include "../logger/i_logger.h"
#include "../dispatcher/actor.h"
#include "messages/messagecallbacks.h"
#include "messages/messagesender.h"
#include "objects/objectphysics.h"
#include "objects/physics_obj_classes/objectphysics_mob.h"
#include "objects/physics_obj_classes/objectphysics_projectile.h"
#include "objects/physics_obj_classes/objectphysics_construction.h"
#include "objects/physics_obj_classes/objectphysics_loot.h"

class CPhysics
        : public CMessageCallbacks
        , public CLogged
        , public CCoresParam
        , public CMessageSender
        , public IPhysicsOtherObjects
{
public:
    CPhysics(const ILoggerPtr log, const IDispatcherPtr disp);
    virtual ~CPhysics() override;
    virtual void onMessage() override final;
    virtual void onTimer(const TimerID_t timer_id) override final;
    virtual void onStart()  override final;
    virtual void onStop()  override final;
    // CActorCallbacks
    virtual bool onNewObjectPhysics(const ObjID_t obj_id, const sObjSpec &spec, const sPhysicsParamMsg &ph_param) override final;
    virtual bool onDelObjectPhysics(const ObjID_t obj_id) override final;
    virtual bool onSetVelocity(const ObjID_t obj_id, const sVect2 &velocity, const sVect2 &direction) override final;
    virtual bool onRequestPhysicsParamAll(const uint_8 flags) override final;
    virtual bool onSetPhysicsGoal(const ObjID_t obj_id, const sPhysicsGoal &ph_goal) override final;
    virtual bool onObjectAction(const sObjectAction &object_action) override final;
    // IPhysicsOtherObjects
    virtual bool GetPhysicsObject(const ObjID_t obj_id, CObjectPhysics** ph_obj) override final;
    virtual bool GetPhysicsParam(const ObjID_t obj_id, sPhysicsParam &ph_param) override final;
    virtual bool GetPhysicsMaterialParam(const ObjID_t obj_id, sMaterialParam &mt_param) override final;

private:
    bool AddObject(const ObjID_t &id, const sObjSpec &spec, const sPhysicsParam &param);
    bool DelObject(const ObjID_t &id);
    CObjectPhysics* FindObject(const ObjID_t obj_id);

    bool ObjectSetVelocity(const ObjID_t &id, const sVect2 &velocity);
    bool ObjectSetDirection(const ObjID_t &id, const sVect2 &direction);
    void ProcMove(const double time_delta);
    void ProcCollisions();
    void ProcCollisionsMobs();
    void ProcCollisionsProjectiles(const double time_delta);
    void ProcCollisionMob(CObjectPhysics* obj1, CObjectPhysics* obj2);
    void ProcCollisionProjectile(CObjectPhysics *obj_projectile, CObjectPhysics *obj_another, const sCollision &collision);
    void SendKinematicParamAll();
    void ProcGoals();
    void ProcGoal(CObjectPhysics* ph_obj);
    void AddToGlobalTime(const double t_d);

    TimerID_t           m_tm_1sec = TIMER_ID_NONE;
    TimerID_t           m_tm_move = TIMER_ID_NONE;

    sGlobalPhysicsParam m_ph_global;

    std::unordered_map<ObjID_t, CObjectPhysics*> m_objects;

    sCoreStatisticPh    m_statistic;
};

#endif // PHYSICS_H
