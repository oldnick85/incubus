#ifndef PHYSICS_PARAM_COMMON_H
#define PHYSICS_PARAM_COMMON_H
#include <list>
#include "../param/object_param_common.h"
#include "figure.h"

/**
 * @brief sPhysicsState - физическое состояние
 */
struct sPhysicsState
{
    enum enState
    {
        psIdle,         ///< стоит
        psDestroyed,    ///< разрушен
    };

    std::string print() const
    {
        switch (state)
        {
        case psIdle:        return "Idle";
        case psDestroyed:   return "Destroyed";
        }
        return "UNKNOWN";
    }

    void SetDestroyed() {state = psDestroyed;}
    bool IsDestroyed() const {return (state == psDestroyed);}

    enState     state = psIdle;
};

/**
 * @brief sKinematicParam - кинетические параметры
 */
struct sKinematicParam
{
#define POSITION_TOLERANCE 0.01F
#define VELOCITY_TOLERANCE 0.01F

    std::string print() const
    {
        char s[128];
        snprintf(s, sizeof(s), "pos=%s; v=%s; dir=%s;",
                 position.print().c_str(), velocity.print().c_str(), direction.print().c_str());
        return s;
    }

    bool SignificantlyDifferent(const sKinematicParam &other) const
    {
        if ((velocity-other.velocity).Length2() > VELOCITY_TOLERANCE*VELOCITY_TOLERANCE)
            return true;
        if ((direction-other.direction).Length2() > VELOCITY_TOLERANCE*VELOCITY_TOLERANCE)
            return true;
        if ((position - other.position).Length2() > POSITION_TOLERANCE*POSITION_TOLERANCE)
            return true;
        return false;
    }

    sCoord2         position;           ///< положение
    sVect2          velocity;           ///< скорость
    sVect2          direction = {1, 0}; ///< ориентация в пространстве

#undef VELOCITY_TOLERANCE
#undef POSITION_TOLERANCE
};

/**
         * @brief sMaterialParam - параметры материала объекта
         */
struct sMaterialParam
{
    std::string print() const
    {
        char s[64];
        snprintf(s, sizeof(s), "sz=%.03f;", size.Val());
        return s;
    }

    Real_t          size = 0.03;    ///< размер, характерный радиус (м)
    Real_t          mass = 100;     ///< масса (кг)
};

/**
         * @brief sPerseptionParam - параметры восприятия
         */
struct sPerseptionParam
{
    Real_t      sight_distance = 0.1;       ///< дальность обзора
    Real_t      sight_angle = M_PI_2;       ///< угол обзора
};

/**
         * @brief sHealthParam параметры здоровья
         * @details Очки жизни измеряются в джоулях - это энергия разрушения, которую объект может выдержать
         */
struct sHealthParam
{
    std::string Print() const
    {
        char s[128];
        snprintf(s, sizeof(s), "%f; %f; %f;",
                 health_points.Val(), health_points_max.Val(), max_distance_move.Val());
        return s;
    }

    bool UsesHealth() {return (health_points_max > 0.0);}
    Real_t Percent()
    {
        return UsesHealth() ? health_points/health_points_max : Real_t(0.0);
    }
    bool Alive() {return (health_points > 0.0);}
    void Damage(double damage_energy)
    {
        health_points -= damage_energy;
        if (health_points < 0.0)
            health_points = 0.0;
    }

    Real_t      health_points_max = 100;    ///< максимальое количество очков жизни
    Real_t      health_points = 100;        ///< текущее количество очков жизни
    Real_t      max_distance_move = 0.0;    ///< максимальная дальность перемещения (по прохождении объект разрушается)
};

/**
         * @brief enPhysicsGoal - тип цели, которую пытается достичь физический движок объекта
         */
enum enPhysicsGoal : uint_32
{
    pgNone,             ///< нет цели
    pgGoToPoint,        ///< идти в точку
    pgGoByDirection,    ///< идти в направлении
    pgFollowToObject,   ///< следовать за объектом
};

/**
         * @brief sPhysicsGoal - цель, которую пытается достичь физический движок объекта
         */
struct sPhysicsGoal
{
    std::string Print() const
    {
        char s[128];
        snprintf(s, sizeof(s), "%u; %s; %s; %u;", goal, destination.print().c_str(), direction.print().c_str(), object);
        return s;
    }

    void DoNothing() {goal = pgNone;}
    void GoToPoint(const sCoord2 &point) {goal = pgGoToPoint; destination = point;}

    enPhysicsGoal   goal = pgNone;  ///< цель
    sCoord2         destination;    ///< целевая позиция
    sVect2          direction;      ///< целевое направление
    ObjID_t         object;         ///< целевой объект
};

using HookID_t = uint_32;
enum
{
    HOOK_ID_NONE = static_cast<HookID_t>(-1),
};

/**
         * @brief sPhysicsHook - хук на физическое событие
         */
struct sPhysicsHook
{
    enum enPhysicsHook
    {
        hkNone,
        hkObjectInSight,
    };

    HookID_t    hook_id = HOOK_ID_NONE;
    // TODO
};

struct sPhysicsContainerMsg
{
    enum
    {
        MAX_CONTAINED = 16,
    };

    sPhysicsContainerMsg() {for (auto i = 0; i < MAX_CONTAINED; ++i) contained_by_me[i] = OBJ_ID_NONE; }

    bool                can_be_container = false;           ///< может ли объект содержать другие объекты
    ObjID_t             contained_by_me[MAX_CONTAINED];     ///< объекты, содержащиеся в данном объекте
};

/**
         * @brief sPhysicsContainer - свойства быть контейнером и содержаться в контейнере
         */
struct sPhysicsContainer
{
    void FromMsg(const sPhysicsContainerMsg &msg)
    {
        can_be_container = msg.can_be_container;
        contained_by_me.clear();
        for (uint i = 0; i < sPhysicsContainerMsg::MAX_CONTAINED; ++i)
        {
            auto c = msg.contained_by_me[i];
            if (c != OBJ_ID_NONE)
                contained_by_me.push_back(c);
        }
    }

    void ToMsg(sPhysicsContainerMsg &msg) const
    {
        msg.can_be_container = can_be_container;
        auto c_it = contained_by_me.begin();
        for (uint i = 0; i < sPhysicsContainerMsg::MAX_CONTAINED; ++i)
        {
            if (c_it == contained_by_me.end()) break;
            msg.contained_by_me[i] = *c_it;
        }
    }

    bool                can_be_container = false;   ///< может ли объект содержать другие объекты
    std::list<ObjID_t>  contained_by_me;            ///< объекты, содержащиеся в данном объекте
};

struct sPhysicsContained
{
    bool ContainBy(const ObjID_t o_id)
    {
        if (not can_be_contained) return false;
        contain_me = o_id;
        return true;
    }

    bool Free()
    {
        contain_me = OBJ_ID_NONE;
        return true;
    }

    bool IsContained() const {return (contain_me != OBJ_ID_NONE);}

    bool                can_be_contained = false;   ///< может ли объект содержаться в других объектах
    ObjID_t             contain_me = OBJ_ID_NONE;   ///< объект, содержащий данный объект
};

/**
         * @brief sPhysicsAbilities - физические способности
         */
struct sPhysicsAbilities
{
    bool        can_manipulate = true;      ///< способность манипулировать другими объектами
    ObjID_t     hold_obj = OBJ_ID_NONE;     ///< удерживаемый объект
};

enum enPhysicsPartType : uint_32
{
    ppNone,
    ppHumanTorso,
    ppHumanHead,
    ppHumanLegs,
    ppHumanArms,
};

struct sPhysicsPart
{
    enPhysicsPartType   type = ppNone;
    ObjID_t             obj_uses = OBJ_ID_NONE;
};

struct sPhysicsParts
{
    sPhysicsPart body;
    sPhysicsPart chassis;
    sPhysicsPart manipulator;
    sPhysicsPart ganglion;
};


#endif // PHYSICS_PARAM_COMMON_H
