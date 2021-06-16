#ifndef I_OBJECT_H
#define I_OBJECT_H
#include "../dispatcher/i_actor.h"
#include "common_obj.h"
#include "../param/physics_param_common.h"
#include "../param/physics_param.h"
#include "../param/show_param.h"

enum enObjType
{
    otNone,
    otMatter,
    otBehavior,
    otShow,
};

class IObject
{
public:
    virtual ~IObject() = default;

    virtual sObjSpec& ObjSpec() = 0;
    virtual ObjID_t GetID() const = 0;
    virtual void SetID(const ObjID_t &id) = 0;
};

class CObjectKinematic
{
public:
    sKinematicParam* Kinematic() {return &m_kinematic;}
private:
    sKinematicParam   m_kinematic;
};

class CCoresParam
{
public:
    CCoresParam()
    {

    }

    virtual ~CCoresParam() = default;

    void SetCores(const sAllCores &cores) {m_cores = cores;}

    void SetCores(const sActorID &gameplay, const sActorID &physics, const sActorID &behavior, const sActorID &show)
    {
        m_cores.gameplay = gameplay;
        m_cores.physics = physics;
        m_cores.behavior = behavior;
        m_cores.show = show;
    }

    sAllCores& Cores()          {return m_cores;}
    sActorID& GameplayActor()   {return m_cores.gameplay;}
    sActorID& PhysicsActor()    {return m_cores.physics;}
    sActorID& BehaviorActor()   {return m_cores.behavior;}
    sActorID& ShowActor()       {return m_cores.show;}
private:
    sAllCores       m_cores;
};

class CPhysicsParam
{
public:
    virtual ~CPhysicsParam() = default;
    sPhysicsState& PhysicsState() {return m_physics.ph_state;}
    sPhysicsParam& Physics() {return m_physics;}
    sKinematicParam& Kinematic() {return m_physics.kinematic;}
    sCoord2& Position() {return m_physics.kinematic.position;}
    sVect2& Velocity() {return m_physics.kinematic.velocity;}
    sVect2& Direction() {return m_physics.kinematic.direction;}
    sMaterialParam& Material() {return m_physics.material;}
    Real_t& Size() {return m_physics.material.size;}
    sPerseptionParam& Perseption() {return m_physics.perseption;}
    CShape& Shape() {return m_physics.shape;}
    sHealthParam& Health() {return m_physics.health;}
    sPhysicsParts& PhysicsParts() {return m_physics.parts;}
    sPhysicsContained& PhysicsContained() {return m_physics.contained;}
private:
    sPhysicsParam   m_physics;
};

class CShowParam
{
public:
    virtual ~CShowParam() = default;
    void SetShowParam(const sShowParam &sh_param) {m_show = sh_param;}
    double& TraceTTL() {return m_show.trace_ttl;}
    sShowParam::enShowHeight& ShowHeight() {return m_show.sh_height;}
private:
    sShowParam      m_show;
};

#endif // I_OBJECT_H
