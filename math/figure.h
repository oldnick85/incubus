#ifndef FIGURE_H
#define FIGURE_H

#include <algorithm>
#include <memory>
#include "coordinates.h"

/**
 * @brief enFigure - типы фигур
 */
enum enFigure : uint8_t
{
    figNone,            ///< ничего
    figPoint,           ///< точка
    figCircle,          ///< окружность
    figLine,            ///< отрезок
    figTriangle,        ///< треугольник
    figRectangle,       ///< прямоугольник
    figRectangleAxes,   ///< прямоугольник со сторонами, параллельными координатным осям
};

/**
 * @brief CFigure - геометрическая фигура
 */
struct CFigure
{
    bool operator==(const CFigure &other) const
    {
        if (type != other.type) return false;
        if (center != other.center) return false;
        if (radius != other.radius) return false;
        if (radius1 != other.radius1) return false;
        if (angle != other.angle) return false;
        if (pnt[0] != other.pnt[0]) return false;
        if (pnt[1] != other.pnt[1]) return false;
        if (pnt[2] != other.pnt[2]) return false;
        return true;
    }

    bool operator!=(const CFigure &other) const
    {
        return (not (*this == other));
    }

    bool IsNone() const {return (type == figNone);}
    bool IsPoint() const {return (type == figPoint);}
    bool IsCircle() const {return (type == figCircle);}
    bool IsLine() const {return (type == figLine);}
    bool IsRectangle() const {return (type == figRectangle);}
    bool IsRectangleAxes() const {return (type == figRectangleAxes);}

    /**
     * @brief GetRectAxesCorners - получить угловые точки ограничивающего прямоугольника
     * @details Гарантируется, что координаты p1 не превосходят соответствующие координаты p2.
     * @param[out] p1 - здесь будет левая нижняя точка
     * @param[out] p2 - здесь будет правая верхняя точка или неназначенная точка (в случае вырожденного в точку прямоугольника)
     * @return успех операции
     */
    bool GetRectAxesCorners(sCoord2 &p1, sCoord2 &p2) const
    {
        switch (type)
        {
        case figNone:
            return false;
        case figPoint:
            p1 = center;
            p2.Unplace();
            return true;
        case figCircle:
            p1.Place(center.x - radius, center.y - radius);
            p2.Place(center.x + radius, center.y + radius);
            return true;
        case figLine:
        {
            Real_t x1, x2, y1, y2;
            if (pnt[0].x > pnt[1].x)
            {
                x1 = pnt[1].x;
                x2 = pnt[0].x;
            }
            else
            {
                x1 = pnt[0].x;
                x2 = pnt[1].x;
            }
            if (pnt[0].y > pnt[1].y)
            {
                y1 = pnt[1].y;
                y2 = pnt[0].y;
            }
            else
            {
                y1 = pnt[0].y;
                y2 = pnt[1].y;
            }
            p1.Place(x1, y1);
            p2.Place(x2, y2);
            return true;
        }
        case figRectangle:
            return GetRectAxesRectangle(p1, p2);
        case figTriangle:
            return GetRectAxesTriangle(p1, p2);
        case figRectangleAxes:
        {
            p1 = pnt[0];
            p2 = pnt[1];
            return true;
        }
        }
        return true;
    }

    /**
     * @brief Clear - очистить фигуру (сделать пустой)
     */
    void Clear() {type = figNone;}
    /**
     * @brief MakePoint - инициализировать как точку
     * @param[in] pnt - координаты точки
     */
    void MakePoint(const sCoord2 &pnt) {type = figPoint; center = pnt;}
    /**
     * @brief MakeCircle - инициализировать как окружность
     * @param[in] cent - центр окружности
     * @param[in] r - радиус окружности
     */
    void MakeCircle(const sCoord2 &cent, const Real_t r) {type = figCircle; center = cent; radius = r;}
    /**
     * @brief MakeLine - нициализировать как отрезок
     * @param[in] p1 - один конец отрезка
     * @param[in] p2 - другой конец отрезка
     */
    void MakeLine(const sCoord2 &p1, const sCoord2 &p2) {type = figLine; pnt[0] = p1; pnt[1] = p2;}
    /**
     * @brief MakeRectangle - инициализировать как прямоугольник
     * @param[in] cent - центр прямоугольника (точка пересечения диагоналей)
     * @param[in] a - ширина прямоугольника
     * @param[in] b - высота прямоугольника
     * @param[in] ang - угол, который ширина составляет с осью абсцисс
     */
    void MakeRectangle(const sCoord2 &cent, const Real_t a, const Real_t b, const Real_t ang = 0)
    {
        type = figRectangle;
        center = cent;
        radius = a;
        radius1 = b;
        angle = ang;
    }

    void MakeRectangleAxes(const sCoord2 &p0, const sCoord2 &p1)
    {
        type = figRectangleAxes;
        Real_t x1, x2, y1, y2;
        if (p0.x > p1.x) {x1 = p1.x; x2 = p0.x;} else {x1 = p0.x; x2 = p1.x;}
        if (p0.y > p1.y) {y1 = p1.y; y2 = p0.y;} else {y1 = p0.y; y2 = p1.y;}
        pnt[0].Place(x1, y1);
        pnt[1].Place(x2, y2);
        sVect2 v = (pnt[1] - pnt[0]);
        v.Scale(0.5);
        center = pnt[0] + v;
        radius = abs((pnt[1].x - pnt[0].x).Val())/2;
        radius1 = abs((pnt[1].y - pnt[0].y).Val())/2;
    }

    bool GetRectAxesRectangle(sCoord2 &p1, sCoord2 &p2) const
    {
        sCoord2 pp1(center.x+radius, center.y+radius1);
        sCoord2 pp2(center.x+radius, center.y-radius1);
        sCoord2 pp3(center.x-radius, center.y-radius1);
        sCoord2 pp4(center.x-radius, center.y+radius1);
        pp1.Rotate(center, angle);
        pp2.Rotate(center, angle);
        pp3.Rotate(center, angle);
        pp4.Rotate(center, angle);
        const auto vx = {pp1.x, pp2.x, pp3.x, pp4.x};
        const auto vy = {pp1.y, pp2.y, pp3.y, pp4.y};
        const auto [xmin, xmax] = std::minmax_element(begin(vx), end(vx));
                const auto [ymin, ymax] = std::minmax_element(begin(vy), end(vy));
                p1.Place(*xmin, *ymin);
                p2.Place(*xmax, *ymax);
                return true;
    }

                bool GetRectAxesTriangle(sCoord2 &p1, sCoord2 &p2) const
        {
            const auto vx = {pnt[0].x, pnt[1].x, pnt[2].x};
            const auto vy = {pnt[0].y, pnt[1].y, pnt[2].y};
            const auto [xmin, xmax] = std::minmax_element(begin(vx), end(vx));
                    const auto [ymin, ymax] = std::minmax_element(begin(vy), end(vy));
                    p1.Place(*xmin, *ymin);
                    p2.Place(*xmax, *ymax);
                    return true;
        }

                    enFigure        type = figNone;
                    sCoord2         center;
                    Real_t          radius = 0.0;
                    Real_t          radius1 = 0.0;
                    Real_t          angle = 0.0;
                    sCoord2         pnt[3];
        };
                    using CFigurePtr = std::shared_ptr<CFigure>;

        #endif // FIGURE_H
