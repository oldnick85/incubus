#ifndef CDRAWSHAPE_H
#define CDRAWSHAPE_H

#include <list>

#include "idrawengine.h"
#include "../math/shape.h"
#include "drawfigure.h"

class CDrawShape
        : protected CShape
{
public:
    CDrawShape(IDrawEnginePtr draw);
    void SetShape(const CShape &shape);
    void Draw();
    bool lined = false;
    double line_width = 1.0;
private:
    IDrawEnginePtr              m_draw;
    std::list<CDrawFigurePtr>   m_draw_figures;
};
using CDrawShapePtr = std::shared_ptr<CDrawShape>;

#endif // CDRAWSHAPE_H
