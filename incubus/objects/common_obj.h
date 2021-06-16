#ifndef COMMON_OBJ_H
#define COMMON_OBJ_H

#include "../incubus_common.h"

#define BITS_FOR_OBJ_CLASS_ENUM 8
#define BITS_FOR_OBJ_TYPE_ENUM 8
#define BITS_FOR_OBJ_SPEC_ENUM 16

enum enObjClass : uint_32
{
    ocNone,
    ocMob,
    ocConstruction,
    ocProjectile,
    ocLoot,
};

enum enObjSort : uint_32
{
    osNone,
    //Mob
    osMob               = (ocMob << BITS_FOR_OBJ_TYPE_ENUM),
    osHuman,
    osSpider,
    //Construction,
    osConstruction      = (ocConstruction << BITS_FOR_OBJ_TYPE_ENUM),
    osWall,
    //Projectile,
    osProjectile        = (ocProjectile << BITS_FOR_OBJ_TYPE_ENUM),
    osBullet,
    //Loot,
    osLoot              = (ocLoot << BITS_FOR_OBJ_TYPE_ENUM),
    osFirearm,
};

enum enObjSpec : uint_32
{
    opNone,
    //Human
    opHuman             = (osHuman << BITS_FOR_OBJ_SPEC_ENUM),
    opZombie,
    //Bullet
    opBullet            = (osProjectile << BITS_FOR_OBJ_SPEC_ENUM),
    opBullet_9mm,
    //Firearm
    opFirearm           = (osFirearm << BITS_FOR_OBJ_SPEC_ENUM),
    opPistol,
    opRifle,
    //Wall
    opWall              = (osWall << BITS_FOR_OBJ_SPEC_ENUM),
};

struct sObjSpec
{
    sObjSpec() = default;

    sObjSpec(const enObjSpec spec)
        : obj_spec(spec)
    {

    }

    std::string PrintClass() const
    {
        switch (Class())
        {
        case ocNone:            return "None";
        case ocMob:             return "Mob";
        case ocConstruction:    return "Construction";
        case ocProjectile:      return "Projectile";
        case ocLoot:            return "Loot";
        }
        return "UNKNOWN";
    }

    std::string PrintSort() const
    {
        switch (Sort())
        {
        case osNone:            return "None";
        case osMob:             return "None";
        case osHuman:           return "Human";
        case osSpider:          return "Spider";
        case osConstruction:    return "None";
        case osWall:            return "Wall";
        case osProjectile:      return "None";
        case osBullet:          return "Bullet";
        case osLoot:            return "None";
        case osFirearm:         return "Firearm";
        }
        return "UNKNOWN";
    }

    std::string Print() const
    {
        char s[128];
        snprintf(s, sizeof(s), "%s:%s", PrintClass().c_str(), PrintSort().c_str());
        return s;
    }

    bool IsMob() const {return (Class() == ocMob);}
    bool IsProjectile() const {return (Class() == ocProjectile);}
    bool IsConstruction() const {return (Class() == ocConstruction);}
    bool IsLoot() const {return (Class() == ocLoot);}

    enObjClass Class() const
    {
        return static_cast<enObjClass>(obj_spec >> (BITS_FOR_OBJ_SPEC_ENUM + BITS_FOR_OBJ_TYPE_ENUM));
    }

    enObjSort Sort() const
    {
        return static_cast<enObjSort>(obj_spec >> (BITS_FOR_OBJ_SPEC_ENUM));
    }

    enObjSpec Spec() const
    {
        return obj_spec;
    }

    void SetObjSpec(const enObjSpec spec)
    {
        obj_spec = spec;
    }

    enObjSpec       obj_spec = opNone;
};

#endif // COMMON_OBJ_H
