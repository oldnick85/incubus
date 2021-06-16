#ifndef PHYSICS_PARAM_H
#define PHYSICS_PARAM_H

#include <memory>
#include "shape.h"
#include "../param/physics_param_common.h"

/**
 * @brief sPhysicsParamMsg - физические параметры для передачи через диспетчер
 */
struct sPhysicsParamMsg
{
    std::string print() const
    {
        char s[128];
        snprintf(s, sizeof(s), "%s %s",
                 kinematic.print().c_str(), material.print().c_str());
        return s;
    }

    sPhysicsState           ph_state;
    sKinematicParam         kinematic;
    sMaterialParam          material;
    sPerseptionParam        perseption;
    sHealthParam            health;
    sPhysicsContainerMsg    container;
    sPhysicsAbilities       abilities;
    sPhysicsParts           parts;
    sPhysicsContained       contained;
    sShapeMsg               shape;
};

/**
 * @brief sPhysicsParam - физические параметры
 */
struct sPhysicsParam
{
    std::string print() const
    {
        char s[128];
        snprintf(s, sizeof(s), "%s %s",
                 kinematic.print().c_str(), material.print().c_str());
        return s;
    }

    void ToMsg(sPhysicsParamMsg &msg) const
    {
        msg.ph_state = ph_state;
        msg.kinematic = kinematic;
        msg.material = material;
        msg.perseption = perseption;
        msg.health = health;
        msg.abilities = abilities;
        msg.parts = parts;
        msg.contained = contained;
        shape.ToMsg(msg.shape);
        container.ToMsg(msg.container);
    }

    void FromMsg(const sPhysicsParamMsg &msg)
    {
        ph_state = msg.ph_state;
        kinematic = msg.kinematic;
        material = msg.material;
        perseption = msg.perseption;
        health = msg.health;
        abilities = msg.abilities;
        parts = msg.parts;
        contained = msg.contained;
        shape.FromMsg(msg.shape);
        container.FromMsg(msg.container);
    }

    /**
     * @brief CollisionsInvolved - предикат участия в столкновениях
     * @return участвует ли объект в столкновениях
     */
    bool CollisionsInvolved() const {return (not ph_state.IsDestroyed());}

    sPhysicsState       ph_state;
    sKinematicParam     kinematic;
    sMaterialParam      material;
    sPerseptionParam    perseption;
    sHealthParam        health;
    sPhysicsContainer   container;
    sPhysicsContained   contained;
    sPhysicsAbilities   abilities;
    sPhysicsParts       parts;
    CShape              shape;
};

/**
 * @brief sGlobalPhysicsParam - структура глобальных физических параметров
 */
struct sGlobalPhysicsParam
{
    std::string Print() const
    {
        char s[128];
        snprintf(s, sizeof(s), "%f; %f; %f; %s;", physics_time.Val(), time_delta.Val(), time_speed.Val(), max_coord.print().c_str());
        return s;
    }

    Real_t          physics_time = 0.0; ///< текущее физическое время (сек)
    Real_t          time_delta = 0.01;  ///< базовая длительность кванта времени (сек)
    Real_t          time_speed = 1.0;   ///< коэффициент скорости физических процессов
    sCoord2         max_coord = {100.0, 100.0};
};

class CObjectPhysics;
class IPhysicsOtherObjects
{
public:
    virtual ~IPhysicsOtherObjects() = default;
    virtual bool GetPhysicsParam(const ObjID_t obj_id, sPhysicsParam &ph_param) = 0;
    virtual bool GetPhysicsMaterialParam(const ObjID_t obj_id, sMaterialParam &mt_param) = 0;
    virtual bool GetPhysicsObject(const ObjID_t obj_id, CObjectPhysics** ph_obj) = 0;
};

using IPhysicsOtherObjectsPtr = std::shared_ptr<IPhysicsOtherObjects>;

#endif // PHYSICS_PARAM_H
