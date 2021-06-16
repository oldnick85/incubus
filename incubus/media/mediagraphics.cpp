#include "mediagraphics.h"

CMediaGraphics::CMediaGraphics(CShowUtilPtr util, IDrawEnginePtr draw)
    : m_util(util)
    , m_draw(draw)
{

}

bool CMediaGraphics::DrawGraphicsSpiderLegs(const sGraphicsDrawParam &draw_param)
{
    auto sz = draw_param.size;
    auto tm = draw_param.time_state;
    auto pos = draw_param.kinematic.position;
    auto dir = draw_param.kinematic.velocity;

    Real_t ct = 0.0;
    if (not dir.IsNul())
    {
        Real_t step_l = sz*2.0;
        auto t = step_l/dir.Length();
        int ct_p = static_cast<int>(trunc((tm/t).Val()));
        Real_t ct_mod = tm - Real_t(ct_p)*t;
        if ((ct_p % 2) == 0)
            ct = -t/2.0 + ct_mod;
        else {
            ct = t/2.0 - ct_mod;
        }
    }
    else
    {
        dir = draw_param.kinematic.direction;
    }
    auto dir_n = dir.Normal();
    dir_n.Normalize();
    dir.Normalize();

    sVect2 dir_v = dir;
    dir_v.Normalize(ct);

    sz *= 2.0;
    sVect2 dir14 = dir;
    dir14.Normalize(sz*2.0/sqrt(5));
    sVect2 dir23 = dir;
    dir23.Normalize(sz/sqrt(5));
    sVect2 nrm14 = dir_n;
    nrm14.Normalize(sz/sqrt(5));
    sVect2 nrm23 = dir_n;
    nrm23.Normalize(sz*2.0/sqrt(5));
    auto ps1l = pos + dir14 - nrm14 - dir_v;
    auto ps2l = pos + dir23 - nrm23 + dir_v;
    auto ps3l = pos - dir23 - nrm23 - dir_v;
    auto ps4l = pos - dir14 - nrm14 + dir_v;
    auto ps1r = pos + dir14 + nrm14 + dir_v;
    auto ps2r = pos + dir23 + nrm23 - dir_v;
    auto ps3r = pos - dir23 + nrm23 + dir_v;
    auto ps4r = pos - dir14 + nrm14 - dir_v;

    m_draw->Color4(0.8, 0.8, 0.8, 1.0);
    m_util->DrawLine(pos, ps1l);
    m_util->DrawLine(pos, ps2l);
    m_util->DrawLine(pos, ps3l);
    m_util->DrawLine(pos, ps4l);
    m_util->DrawLine(pos, ps1r);
    m_util->DrawLine(pos, ps2r);
    m_util->DrawLine(pos, ps3r);
    m_util->DrawLine(pos, ps4r);

    return true;
}

bool CMediaGraphics::DrawGraphicsHumanLegs(const sGraphicsDrawParam &draw_param)
{
    auto sz = draw_param.size;
    auto tm = draw_param.time_state;
    auto pos = draw_param.kinematic.position;
    auto dir = draw_param.kinematic.velocity;

    Real_t ct = 0.0;
    if (not dir.IsNul())
    {
        Real_t step_l = sz*2.0;
        auto t = step_l/dir.Length();
        int ct_p = static_cast<int>(trunc((tm/t).Val()));
        Real_t ct_mod = tm - Real_t(ct_p)*t;
        if ((ct_p % 2) == 0)
            ct = -t/2.0 + ct_mod;
        else {
            ct = t/2.0 - ct_mod;
        }
    }
    else
    {
        dir = draw_param.kinematic.direction;
    }
    auto dir_n = dir.Normal();
    dir_n.Normalize(sz/2.0);

    dir.Normalize(ct);

    auto ps1 = pos + dir + dir_n;
    auto ps2 = pos - dir - dir_n;

    m_draw->Color4(0.3, 0.3, 0.3, 1.0);
    m_util->DrawDisk(&ps1, sz/3.0);
    m_util->DrawDisk(&ps2, sz/3.0);

    return true;
}

void CMediaGraphics::DrawPhysicsPart(enPhysicsPartType const type, const sGraphicsDrawParam &draw_param)
{
    switch (type) {
    case ppHumanLegs:
        DrawGraphicsHumanLegs(draw_param);
        break;
    case ppHumanTorso:
        DrawGraphicsHumanTorso(draw_param);
        break;
    default:
        break;
    }
}

bool CMediaGraphics::DrawGraphicsHumanTorso(const sGraphicsDrawParam &draw_param)
{
    auto sz = draw_param.size;
    //auto tm = draw_param.time_state;
    auto pos = draw_param.kinematic.position;
    auto dir = draw_param.kinematic.velocity;

    //Real_t ct = 0.0;
    if (dir.IsNul())
    {
        dir = draw_param.kinematic.direction;
    }
    auto dir_n = dir.Normal();
    dir_n.Normalize(sz*2.0/sqrt(5.0));

    dir.Normalize(sz/sqrt(5.0));

    auto ps1 = pos + dir + dir_n;
    auto ps2 = pos + dir - dir_n;
    auto ps3 = pos - dir - dir_n;
    auto ps4 = pos - dir + dir_n;

    m_draw->Color4(0.4, 0.4, 0.4, 0.8);
    m_draw->DrawQuads2({{ps1.x, ps1.y}, {ps2.x, ps2.y}, {ps3.x, ps3.y}, {ps4.x, ps4.y}});
    return true;
}
