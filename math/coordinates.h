#ifndef COORDINATES_H
#define COORDINATES_H
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <math.h>
#include <cstring>
#include "real_number.h"

/**
 * @brief sVect2 - вектор на плоскости
 */
struct sVect2
{
    sVect2() {vx = 0.0; vy = 0.0;}
    sVect2(const Real_t vxx, const Real_t vyy) {vx = vxx; vy = vyy;}

    std::string print() const
    {
        char s[64];
        snprintf(s, sizeof(s), "v(%f;%f)", vx.Val(), vy.Val());
        return s;
    }

    sVect2 operator*(const Real_t k) const {return {k*vx, k*vy};}

    Real_t MultiplyScalar(const sVect2 &v) const {return vx*v.vx+vy*v.vy;}
    Real_t MultiplySkew(const sVect2 &v) const {return vx*v.vy-vy*v.vx;}
    Real_t Length2() const {return vx*vx+vy*vy;}
    Real_t Length() const {return sqrt((vx*vx+vy*vy).Val());}
    bool IsNul() const {return (vx.IsNul() and vy.IsNul());}
    void Nullify() {vx = 0.0; vy = 0.0;}
    sVect2 operator+(const sVect2 &v) const {return(sVect2(vx+v.vx, vy+v.vy));}
    sVect2 operator-(const sVect2 &v) const {return(sVect2(vx-v.vx, vy-v.vy));}
    sVect2 operator-() const {return(sVect2(-vx, -vy));}

    void Scale(const Real_t k) {vx *= k; vy *= k;}

    sVect2 Normal() const
    {
        if (IsNul()) return {0.0, 0.0};
        return {-vy.Val(), vx.Val()};
    }

    Real_t Normalize(const Real_t scale = 1.0)
    {
        if (IsNul()) return 0.0;
        auto l = Length();
        vx *= scale/l;
        vy *= scale/l;
        return l;
    }

    void Rotate(const Real_t angle)
    {
        Real_t v_sin = sin(angle.Val());
        Real_t v_cos = cos(angle.Val());
        auto vxx = v_cos*vx - v_sin*vy;
        auto vyy = v_sin*vx + v_cos*vy;
        vx = vxx;
        vy = vyy;
    }

    Real_t vx = 0.0;
    Real_t vy = 0.0;
};

/**
 * @brief sCoord2 - координаты точки на плоскости
 */
struct sCoord2
{
    sCoord2(const sCoord2 &c) {*this = c;}
    sCoord2() {x = 0; y = 0; placed = false;}
    sCoord2(const Real_t xx, const Real_t yy) {Place(xx, yy);}

    void Place(const Real_t xx, const Real_t yy) {x = xx; y = yy; placed = true;}
    void Unplace() {placed = false;}
    bool IsPlaced() {return placed;}
    sCoord2 operator+(const sVect2 &v) const {return {x + v.vx, y + v.vy};}
    sCoord2 operator-(const sVect2 &v) const {return {x - v.vx, y - v.vy};}
    sCoord2 operator+(const sCoord2 &p) const {return {x + p.x, y + p.y};}
    void operator+=(const sVect2 &v) {x += v.vx; y += v.vy;}
    sVect2 operator-(const sCoord2 &c) const {return(sVect2(x-c.x, y-c.y));}
    void operator=(const sCoord2 &c) {x = c.x; y = c.y; placed = c.placed;}
    bool operator==(const sCoord2 &c) const {return ((x == c.x) and (y == c.y) and (placed == c.placed));}
    bool operator!=(const sCoord2 &c) const {return (not (*this == c));}

    void Rotate(const sCoord2 &center, const Real_t angle)
    {
        Real_t sn = sin(angle.Val());
        Real_t cs = cos(angle.Val());
        Real_t xx = x - center.x;
        Real_t yy = y - center.y;
        x = cs*xx - sn*yy + center.x;
        y = sn*xx + cs*yy + center.y;
    }

    std::string print() const
    {
        char s[64];
        snprintf(s, sizeof(s), "p(%f;%f)", x.Val(), y.Val());
        return s;
    }

    Real_t x = 0.0;
    Real_t y = 0.0;
    bool placed = false;
};

#endif // COORDINATES_H
