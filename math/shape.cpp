#include <cassert>
#include "shape.h"

void CShape::Clear()
{
    m_figures.clear();
    m_rect_axes.Clear();
}

void CShape::AddPoint(const sCoord2 &c)
{
    CFigure fig;
    fig.MakePoint(c);
    AddFigure(fig);
}

void CShape::AddCircle(const sCoord2 &c, const Real_t r)
{
    CFigure fig;
    fig.MakeCircle(c, r);
    AddFigure(fig);
}

void CShape::AddLine(const sCoord2 &p1, const sCoord2 &p2)
{
    CFigure fig;
    fig.MakeLine(p1, p2);
    AddFigure(fig);
}

void CShape::AddFigure(const CFigure &fig)
{
    if (fig.IsNone()) return;
    m_figures.push_back(fig);
    MakeRectAxes();
}

void CShape::ToMsg(sShapeMsg &msg) const
{
    auto i = 0;
    for (const auto &f : m_figures)
    {
        msg.figures[i] = f;
        i++;
    }
}

void CShape::FromMsg(const sShapeMsg &msg)
{
    m_figures.clear();
    for (uint i = 0; i < sShapeMsg::MAX_FIGURES_IN_SHAPE; ++i)
    {

        AddFigure(msg.figures[i]);
    }
}

bool CShape::PotentiallyCollide(const CShape* shape_other, const sCoord2 &pos, const sCoord2 &pos_other) const
{
    return CollisionRectanglesAxes(m_rect_axes.pnt[0] + pos, m_rect_axes.pnt[1] + pos,
            shape_other->RectAxes().pnt[0] + pos_other, shape_other->RectAxes().pnt[1] + pos_other,
            nullptr, true);
}

bool CShape::Collision(const CShape* shape_other, const sCoord2 &pos, const sCoord2 &pos_other, sCollision* collision) const
{
    for (const auto &fig : m_figures)
    {
        for (const auto &fig_other : shape_other->m_figures)
        {
            if (Collision(fig, fig_other, pos, pos_other, collision))
                return true;
        }
    }
    return false;
}

bool CShape::Collision(const CFigure &figure, const sCoord2 &pos, const sCoord2 &pos_figure, sCollision* collision) const
{
    for (const auto &fig : m_figures)
    {
        if (Collision(fig, figure, pos, pos_figure, collision))
            return true;
    }
    return false;
}

bool CShape::PotentiallyCollideMovingPoint(const sCoord2 &pos, const sCoord2 &pos_point, const sVect2 &v_point, sCollision* collision) const
{
    return CollisionMovingPointRectAxes(m_rect_axes.pnt[0] + pos, m_rect_axes.pnt[1] + pos, pos_point, v_point, collision);
}

bool CShape::CollisionMovingPoint(const sCoord2 &pos, const sCoord2 &pos_point, const sVect2 &v_point, sCollision* collision)
{
    for (const auto &fig : m_figures)
    {
        if (CollisionMovingPoint(fig, pos, pos_point, v_point, collision))
            return true;
    }
    return false;
}

bool CShape::CollisionMovingPoint(const CFigure &fig, const sCoord2 &pos, const sCoord2 &pos_point, const sVect2 &v_point, sCollision* collision)
{
    if (fig.IsCircle())
        return CollisionMovingPointCircle(fig.center + pos, fig.radius, pos_point, v_point, collision);

    if (fig.IsLine())
        return CollisionMovingPointLine(fig.pnt[0] + pos, fig.pnt[1] + pos, pos_point, v_point, collision);

    if (fig.IsRectangleAxes())
        return CollisionMovingPointRectAxes(fig.pnt[0] + pos, fig.pnt[1] + pos, pos_point, v_point, collision);

    return false;
}

bool CShape::CollisionMovingPointLine(const sCoord2 &p0, const sCoord2 &p1, const sCoord2 &pos_point, const sVect2 &v_point, sCollision* collision)
{
    sVect2 v = p1 - p0;
    sVect2 u = pos_point - p0;
    Real_t sm = v.MultiplySkew(v_point);
    Real_t tp = u.MultiplySkew(v_point)/sm;
    Real_t tv = u.MultiplySkew(v)/sm;

    if ((tp >= 0.0) and (tp <= 1.0) and (tv >= 0.0) and (tv <= 1.0))
    {
        if (collision)
        {
            collision->collided = true;
            collision->depth = 0.0;
            v.Scale(tp);
            collision->point = p0 + v;
            collision->direction = v.Normal();
        }
        return true;
    }
    if (collision)
        collision->collided = false;

    return false;
}

bool CShape::CollisionMovingPointCircle(const sCoord2 &center, const Real_t r, const sCoord2 &pos_point, const sVect2 &v_point, sCollision* collision)
{
    auto v_l2 = v_point.Length2();
    if (v_l2 == 0.0)
        return false;
    sVect2 v_pp = center - pos_point;
    auto t = v_pp.MultiplyScalar(v_point)/v_l2;
    sCoord2 p_nearest = pos_point + v_point*t;
    auto l_nearest = (center - p_nearest).Length();
    if (l_nearest >= r)
        return false;
    auto d_begin = (pos_point - center).Length();
    auto d_end = ((pos_point + v_point) - center).Length();
    if (((t <= 1.0) and (t >= 0.0)) or (d_begin <= r) or (d_end <= r))
    {
        Real_t l_dlt = sqrt((r*r - l_nearest*l_nearest).Val());
        sVect2 v_nrm = v_point;
        v_nrm.Normalize(l_dlt);
        sCoord2 p_cls = p_nearest - v_nrm;
        if (collision)
        {
            collision->collided = true;
            collision->point = p_cls;
            collision->direction = center - p_cls;
            collision->depth = collision->direction.Normalize();
        }
        return true;
    }
    return false;
}

bool CShape::CollisionMovingPointRectAxes(const sCoord2 &p0, const sCoord2 &p1, const sCoord2 &pos_point, const sVect2 &v_point, sCollision* collision) const
{
    Real_t t_x0 = -1.0;
    Real_t t_x1 = -1.0;
    Real_t y_x0 = 0.0, y_x1 = 0.0;
    if (not v_point.vx.IsNul())
    {
        t_x0 = (p0.x - pos_point.x)/v_point.vx;
        y_x0 = pos_point.y + v_point.vy*t_x0;
        t_x1 = (p1.x - pos_point.x)/v_point.vx;
        y_x1 = pos_point.y + v_point.vy*t_x1;
    }
    Real_t t_y0 = -1.0;
    Real_t t_y1 = -1.0;
    Real_t x_y0 = 0.0, x_y1 = 0.0;
    if (not v_point.vy.IsNul())
    {
        t_y0 = (p0.y - pos_point.y)/v_point.vy;
        x_y0 = pos_point.x + v_point.vx*t_y0;
        t_y1 = (p1.y - pos_point.y)/v_point.vy;
        x_y1 = pos_point.x + v_point.vx*t_y1;
    }

    bool cldd = false;      // признак столкновения
    // первое пересечение с нижней горизонталью
    if (((t_x0 >= 0.0) and (t_x0 <= 1.0))
            and ((t_x0 <= t_x1) or (t_x1 < 0.0))
            and ((t_x0 <= t_y0) or (t_y0 < 0.0))
            and ((t_x0 <= t_y1) or (t_y1 < 0.0)))
    {
        cldd = true;
        if (collision)
        {
            collision->collided = cldd;
            collision->point.Place(p0.x, y_x0);
        }
    }
    // первое пересечение с верхней горизонталью
    if (((t_x1 >= 0.0) and (t_x1 <= 1.0))
            and ((t_x1 <= t_x0) or (t_x0 < 0.0))
            and ((t_x1 <= t_y0) or (t_y0 < 0.0))
            and ((t_x1 <= t_y1) or (t_y1 < 0.0)))
    {
        cldd = true;
        if (collision)
        {
            collision->collided = cldd;
            collision->point.Place(p1.x, y_x1);
        }
    }
    // первое пересечение с левой вертикалью
    if (((t_y0 >= 0.0) and (t_y0 <= 1.0))
            and ((t_y0 <= t_x0) or (t_x0 < 0.0))
            and ((t_y0 <= t_x1) or (t_x1 < 0.0))
            and ((t_y0 <= t_y1) or (t_y1 < 0.0)))
    {
        cldd = true;
        if (collision)
        {
            collision->collided = cldd;
            collision->point.Place(x_y0, p0.y);
        }
    }
    // первое пересечение с правой вертикалью
    if (((t_y1 >= 0.0) and (t_y1 <= 1.0))
            and ((t_y1 <= t_x0) or (t_x0 < 0.0))
            and ((t_y1 <= t_x1) or (t_x1 < 0.0))
            and ((t_y1 <= t_y0) or (t_y0 < 0.0)))
    {
        cldd = true;
        if (collision)
        {
            collision->collided = cldd;
            collision->point.Place(x_y1, p1.y);
        }
    }
    if (collision)
    {
        collision->collided = cldd;
    }
    return cldd;
}

bool CShape::Collision(const CFigure &f1, const CFigure &f2, const sCoord2 &pos1, const sCoord2 &pos2, sCollision* collision) const
{
    if ((f1.type == figNone) or (f2.type == figNone))
        return false;

    if (f1.IsCircle())
    {
        if (f2.IsPoint())
            return CollisionCirclePoint(f1.center + pos1, f1.radius, f2.center + pos2, collision, true);
        if (f2.IsCircle())
            return CollisionCircles(f1.center + pos1, f1.radius, f2.center + pos2, f2.radius, collision, true);
        if (f2.IsLine())
            return CollisionCircleLine(f1.center + pos1, f1.radius, f2.pnt[0] + pos2, f2.pnt[1] + pos2, collision, true);
        if (f2.IsRectangleAxes())
            return CollisionCircleRectangleAxes(f1.center + pos1, f1.radius, f2.pnt[0] + pos2, f2.pnt[1] + pos2, collision, true);
    }

    if (f1.IsLine())
    {
        if (f2.IsPoint())
            return CollisionLinePoint(f1.pnt[0] + pos1, f1.pnt[1] + pos1, f2.center + pos2, collision, true);
        if (f2.IsCircle())
            return CollisionCircleLine(f2.center + pos2, f2.radius, f1.pnt[0] + pos1, f1.pnt[1] + pos1, collision, false);
        if (f2.IsLine())
            return CollisionLines(f1.pnt[0] + pos1, f1.pnt[1] + pos1, f2.pnt[0] + pos2, f2.pnt[1] + pos2, collision, true);
        if (f2.IsRectangleAxes())
            return CollisionLineRectangleAxes(f1.pnt[0] + pos1, f1.pnt[1] + pos1, f2.pnt[0] + pos2, f2.pnt[1] + pos2, collision, true);
    }

    if (f1.IsPoint())
    {
        if (f2.IsPoint())
            return CollisionPoints(f1.center + pos1, f2.center + pos2, collision, true);
        if (f2.IsCircle())
            return CollisionCirclePoint(f2.center + pos2, f2.radius, f1.center + pos1, collision, false);
        if (f2.IsLine())
            return CollisionLinePoint(f2.pnt[0] + pos2, f2.pnt[1] + pos2, f1.center + pos1, collision, false);
        if (f2.IsRectangleAxes())
            return CollisionPointRectangleAxes(f1.pnt[0] + pos1, f2.pnt[0] + pos2, f2.pnt[1] + pos2, collision, true);
    }

    if (f1.IsRectangleAxes())
    {
        if (f2.IsPoint())
            return CollisionPointRectangleAxes(f1.pnt[0] + pos1, f2.pnt[0] + pos2, f2.pnt[1] + pos2, collision, false);
        if (f2.IsCircle())
            return CollisionCircleRectangleAxes(f1.center + pos1, f1.radius, f2.pnt[0] + pos2, f2.pnt[1] + pos2, collision, false);
        if (f2.IsLine())
            return CollisionLineRectangleAxes(f1.pnt[0] + pos1, f1.pnt[1] + pos1, f2.pnt[0] + pos2, f2.pnt[1] + pos2, collision, false);
    }

    assert(false);
    return false;
}

bool CShape::CollisionPoints(const sCoord2 &p, const sCoord2 &pp, sCollision *collision, const bool figures_order) const
{
    bool res = (p == pp);
    if (collision)
    {
        collision->collided = res;
        collision->direction.Nullify();
        collision->direction.vx = figures_order ? 1.0 : -1.0;
    }
    return res;
}

bool CShape::CollisionCirclePoint(const sCoord2 &c, const Real_t r, const sCoord2 &p, sCollision *collision, const bool figures_order) const
{
    sVect2 v = p - c;
    Real_t d = r - v.Normalize();
    bool res = (d > 0.0);
    if (collision)
    {
        collision->depth = d;
        collision->point = {(c.x+p.x)/2.0, (c.y+p.y)/2.0};
        collision->collided = res;
        collision->direction = figures_order ? v : -v;
    }
    return res;
}

bool CShape::CollisionCircles(const sCoord2 &c1, const Real_t r1, const sCoord2 &c2, const Real_t r2, sCollision *collision, const bool figures_order) const
{
    sVect2 v = c2 - c1;
    Real_t d = r1 + r2 - v.Normalize();
    bool res = (d > 0.0);
    if (collision)
    {
        collision->depth = d;
        collision->point = {(c1.x+c2.x)/2.0, (c1.y+c2.y)/2.0};
        collision->collided = res;
        collision->direction = figures_order ? v : -v;
    }
    return res;
}

bool CShape::CollisionLines(const sCoord2 &l1_1, const sCoord2 &l1_2, const sCoord2 &l2_1, const sCoord2 &l2_2, sCollision *collision, const bool figures_order) const
{
    sVect2 v = l1_2 - l1_1;
    sVect2 w = l2_2 - l2_1;
    sVect2 u = l2_1 - l1_1;
    Real_t sm = v.MultiplySkew(w);
    Real_t tp = u.MultiplySkew(w)/sm;
    Real_t tv = - u.MultiplySkew(v)/sm;

    if ((tp >= 0.0) and (tp <= 1.0) and (tv >= 0.0) and (tv <= 1.0))
    {
        if (collision)
        {
            collision->collided = true;
            collision->depth = 0.0;
            v.Scale(tp);
            collision->point = l1_1 + v;
            collision->direction = figures_order ? v.Normal() : -v.Normal();
        }
        return true;
    }
    if (collision)
        collision->collided = false;

    return false;
}

bool CShape::CollisionCircleLine(const sCoord2 &c, const Real_t r, const sCoord2 &p1, const sCoord2 &p2, sCollision *collision, const bool figures_order) const
{
    sVect2 v = p2 - p1;
    Real_t l = v.Normalize();
    sVect2 v1 = c - p1;
    Real_t l1 = v.vx*v1.vx + v.vy*v1.vy;
    sVect2 n(v.vy, -v.vx);
    if ((n.vx*v1.vx + n.vy*v1.vy) < 0.0)
        n.Scale(-1);
    Real_t d = abs((v.vx*v1.vy - v.vy*v1.vx).Val());

    bool res = false;
    sCoord2 pnt;
    Real_t dlt;
    if ((l1 >= 0.0) and (l1 <= l))
    {
        dlt = r - d;
        res = (dlt > 0.0);
        pnt.Place(p1.x+v.vx*l1, p1.y+v.vy*l1);
    }
    else
    {
        if (l1 < 0.0) pnt = p1;
        if (l1 > l) pnt = p2;
        n = c - pnt;
        d = n.Normalize();
        dlt = r - d;
        res = (dlt > 0.0);
    }

    if (collision)
    {
        collision->depth = dlt;
        collision->point = pnt;
        if (figures_order) n.Scale(-1.0);
        collision->direction = n;
        collision->collided = res;
    }
    return res;
}

bool CShape::CollisionPointRectangleAxes(const sCoord2 &p, const sCoord2 &p1, const sCoord2 &p2, sCollision *collision, const bool point_first) const
{
    //printf("%s %s %s %s %u\n", __FUNCTION__, p.print().c_str(), p1.print().c_str(), p2.print().c_str(), point_first);
    bool res = false;

    res = ((p.x - p1.x)*(p2.x - p.x) > 0.0) and ((p.y - p1.y)*(p2.y - p.y) > 0.0);
    if (res and (collision))
    {
        // TODO заполнение структуры collision
        collision->collided = res;
        if (point_first)
            collision->direction = collision->direction*(-1.0);
    }
    return res;
}

bool CShape::CollisionRectanglesAxes(const sCoord2 &p1, const sCoord2 &p2, const sCoord2 &pp1, const sCoord2 &pp2, sCollision *collision, const bool point_first) const
{
    //printf("%s %s %s %s %s %u\n", __FUNCTION__, p1.print().c_str(), p2.print().c_str(), pp1.print().c_str(), pp2.print().c_str(), point_first);
    bool res = false;

    bool cldd_x = (not (pp1.x > p2.x) or (pp2.x < p1.x));
    bool cldd_y = (not (pp1.y > p2.y) or (pp2.y < p1.y));
    res = (cldd_x and cldd_y);
    if (collision)
    {
        // TODO заполнение структуры collision
        collision->collided = res;
        if (point_first)
            collision->direction = collision->direction*(-1.0);
    }
    return res;
}

bool CShape::CollisionCircleRectangleAxes(const sCoord2 &c, const Real_t r, const sCoord2 &p1, const sCoord2 &p2, sCollision *collision, const bool figures_order) const
{
    sVect2 v = (p2 - p1);
    v.Normalize(r/2);
    auto pp1 = p1 - v;
    auto pp2 = p2 + v;

    return CollisionPointRectangleAxes(c, pp1, pp2, collision, figures_order);
}

bool CShape::CollisionLinePoint(const sCoord2 &l1, const sCoord2 &l2, const sCoord2 &p, sCollision *collision, const bool figures_order) const
{
    //printf("%s %s %s %s %u\n", __FUNCTION__, l1.print().c_str(), l2.print().c_str(), p.print().c_str(), figures_order);
   bool collided = false;
   collided = ((p - l1).MultiplySkew(l2 - l1) == 0.0);
    if (collision)
    {
        collision->collided = collided;
        if (figures_order)
            collision->direction = collision->direction*(-1.0);
    }
    return false;
}

bool CShape::CollisionLineRectangleAxes(const sCoord2 &l1, const sCoord2 &l2, const sCoord2 &p1, const sCoord2 &p2, sCollision *collision, const bool figures_order) const
{
    bool res = false;

    res = CollisionPointRectangleAxes(l1, p1, p2, collision, figures_order);
    if (res) return res;
    res = CollisionPointRectangleAxes(l2, p1, p2, collision, figures_order);
    if (res) return res;

    sCoord2 p3 = {p1.x, p2.y};
    sCoord2 p4 = {p2.x, p1.y};

    res = CollisionLines(l1, l2, p1, p3, collision, true);
    if (res) return res;
    res = CollisionLines(l1, l2, p1, p4, collision, true);
    if (res) return res;
    res = CollisionLines(l1, l2, p2, p3, collision, true);
    if (res) return res;
    res = CollisionLines(l1, l2, p2, p4, collision, true);
    if (res) return res;

    return res;
}

void CShape::MakeRectAxes()
{
    Real_t x1 = 1.0E10, x2 = -1.0E10, y1 = 1.0E10, y2 = -1.0E10;
    sCoord2 p1;
    sCoord2 p2;
    for (const auto &f : m_figures)
    {
        if (f.GetRectAxesCorners(p1, p2))
        {
            if (p1.x < x1) x1 = p1.x;
            if (p1.x > x2) x2 = p1.x;
            if (p1.y < y1) y1 = p1.y;
            if (p1.y > y2) y2 = p1.y;
            if (p2.IsPlaced())
            {
                if (p2.x < x1) x1 = p2.x;
                if (p2.x > x2) x2 = p2.x;
                if (p2.y < y1) y1 = p2.y;
                if (p2.y > y2) y2 = p2.y;
            }
        }
    }
    m_rect_axes.Clear();
    m_rect_axes.MakeRectangleAxes({x1, y1}, {x2, y2});
}

bool CShape::PointInside(const sCoord2 &pos_point) const
{
    for (const auto &f : m_figures)
    {
        if (f.IsRectangleAxes())
        {
            if ((f.pnt[0].x < pos_point.x) and (pos_point.x < f.pnt[1].x) and (f.pnt[0].y < pos_point.y) and (pos_point.y < f.pnt[1].y))
                return true;
        }
    }
    return false;
}
