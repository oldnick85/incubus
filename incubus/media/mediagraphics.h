#ifndef MEDIAGRAPHICS_H
#define MEDIAGRAPHICS_H

#include "incubus_common.h"
#include "cores/common_show.h"

struct sGraphicsDrawParam
{
    Real_t          size = 0.0;
    sKinematicParam kinematic;
    Real_t          time_state = 0.0;
};

class CMediaGraphics
{
public:
    CMediaGraphics(CShowUtilPtr util, IDrawEnginePtr draw);

    void DrawPhysicsPart(enPhysicsPartType const type, const sGraphicsDrawParam &draw_param);

    bool DrawGraphicsSpiderLegs(const sGraphicsDrawParam &draw_param);
    bool DrawGraphicsHumanLegs(const sGraphicsDrawParam &draw_param);
    bool DrawGraphicsHumanTorso(const sGraphicsDrawParam &draw_param);

private:
    CShowUtilPtr    m_util;
    IDrawEnginePtr  m_draw;
};

using CMediaGraphicsPtr = std::shared_ptr<CMediaGraphics>;

#endif // MEDIAGRAPHICS_H
