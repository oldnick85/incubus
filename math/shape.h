#ifndef SHAPE_H
#define SHAPE_H
#include <vector>

#include "figure.h"

struct sShapeMsg
{
    enum
    {
        MAX_FIGURES_IN_SHAPE = 4,
    };
    CFigure     figures[MAX_FIGURES_IN_SHAPE];
};

/**
 * @brief sCollision - результат столкновения
 */
struct sCollision
{
    bool        collided = false;   ///< касание
    sCoord2     point;              ///< точка соприкосновения
    sVect2      direction;          ///< нормаль к линии касания
    Real_t      depth = 0.0;        ///< глубина пересечения
};

/**
 * @brief CShape - класс, содержащий форму чего-либо
 * @details Форма хранится в виде объединения фигур
 */
class CShape
{
public:
    CShape() = default;

    bool operator==(const CShape &other) const
    {
        return std::equal(m_figures.begin(), m_figures.end(), other.m_figures.begin(), other.m_figures.end());
    }

    bool operator!=(const CShape &other) const
    {
        return (not (*this == other));
    }

    void operator=(const CShape &other)
    {
        m_figures = other.m_figures;
        m_rect_axes = other.m_rect_axes;
    }

    void operator=(const sShapeMsg &right)
    {
        FromMsg(right);
    }
    /**
     * @brief Clear - очистить список фигур
     */
    void Clear();
    /**
     * @brief AddFigure - добавить фигуру
     * @param[in] fig - добавляемая фигурв
     */
    void AddFigure(const CFigure &fig);
    /**
     * @brief AddPoint - добавить точку
     * @param[in] c - координаты точки
     */
    void AddPoint(const sCoord2 &c);
    /**
     * @brief AddCircle - добавить круг
     * @param[in] c - центр круга
     * @param[in] r - радиус круга
     */
    void AddCircle(const sCoord2 &c, const Real_t r);
    /**
     * @brief AddLine - добавить отрезок
     * @param[in] p1 - один конец
     * @param[in] p2 - другой конец
     */
    void AddLine(const sCoord2 &p1, const sCoord2 &p2);

    void ToMsg(sShapeMsg &msg) const;
    void FromMsg(const sShapeMsg &msg);

    std::vector<CFigure> Figures() {return m_figures;}
    const CFigure& RectAxes() const {return m_rect_axes;}

    bool PointInside(const sCoord2 &pos_point) const;

    bool PotentiallyCollideMovingPoint(const sCoord2 &pos, const sCoord2 &pos_point, const sVect2 &v_point, sCollision* collision) const;
    bool CollisionMovingPoint(const sCoord2 &pos, const sCoord2 &pos_point, const sVect2 &v_point, sCollision* collision = nullptr);
    bool CollisionMovingPoint(const CFigure &fig, const sCoord2 &pos, const sCoord2 &pos_point, const sVect2 &v_point, sCollision* collision  = nullptr);
    bool CollisionMovingPointCircle(const sCoord2 &center, const Real_t r, const sCoord2 &pos_point, const sVect2 &v_point, sCollision* collision = nullptr);
    bool CollisionMovingPointLine(const sCoord2 &p0, const sCoord2 &p1, const sCoord2 &pos_point, const sVect2 &v_point, sCollision* collision = nullptr);
    bool CollisionMovingPointRectAxes(const sCoord2 &p0, const sCoord2 &p1, const sCoord2 &pos_point, const sVect2 &v_point, sCollision* collision = nullptr) const;

    bool PotentiallyCollide(const CShape* shape_other, const sCoord2 &pos, const sCoord2 &pos_other) const;
    bool Collision(const CShape* shape_other, const sCoord2 &pos, const sCoord2 &pos_other, sCollision* collision = nullptr) const;
    bool Collision(const CFigure &figure, const sCoord2 &pos, const sCoord2 &pos_figure, sCollision* collision = nullptr) const;
    bool Collision(const CFigure &f1, const CFigure &f2, const sCoord2 &pos1, const sCoord2 &pos2, sCollision* collision = nullptr) const;
protected:
    std::vector<CFigure>    m_figures;      ///< список фигур
    CFigure                 m_rect_axes;    ///< прямоугольник, содержащий форму (для ускорения обнаружения столкновений)

private:
    bool CollisionPoints(const sCoord2 &p, const sCoord2 &pp, sCollision *collision, const bool figures_order) const;
    bool CollisionCirclePoint(const sCoord2 &c1, const Real_t r1, const sCoord2 &p, sCollision *collision, const bool figures_order) const;
    bool CollisionCircles(const sCoord2 &c1, const Real_t r1, const sCoord2 &c2, const Real_t r2, sCollision *collision, const bool figures_order) const;
    bool CollisionLines(const sCoord2 &l1_1, const sCoord2 &l1_2, const sCoord2 &l2_1, const sCoord2 &l2_2, sCollision *collision, const bool figures_order) const;
    bool CollisionCircleLine(const sCoord2 &c, const Real_t r, const sCoord2 &p1, const sCoord2 &p2, sCollision *collision, const bool figures_order) const;
    bool CollisionPointRectangleAxes(const sCoord2 &p, const sCoord2 &p1, const sCoord2 &p2, sCollision *collision, const bool figures_order) const;
    bool CollisionRectanglesAxes(const sCoord2 &p1, const sCoord2 &p2, const sCoord2 &pp1, const sCoord2 &pp2, sCollision *collision, const bool point_first) const;
    bool CollisionCircleRectangleAxes(const sCoord2 &c, const Real_t r, const sCoord2 &p1, const sCoord2 &p2, sCollision *collision, const bool figures_order) const;
    bool CollisionLineRectangleAxes(const sCoord2 &l1, const sCoord2 &l2, const sCoord2 &p1, const sCoord2 &p2, sCollision *collision, const bool figures_order) const;
    bool CollisionLinePoint(const sCoord2 &l1, const sCoord2 &l2, const sCoord2 &p, sCollision *collision, const bool figures_order) const;

    void MakeRectAxes();

};

#endif // SHAPE_H
