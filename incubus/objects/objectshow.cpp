#include "objectshow.h"

CObjectShow::CObjectShow(const ILoggerPtr log, const IDispatcherPtr disp,
                         sGlobalPhysicsParam *ph_global, sGlobalShowParam *sh_global,
                         CMediaGraphicsPtr graphics, sCoreShowParam* core_show_param,
                         const sObjSpec &spec, sDispatcherMsgStatistic* msg_stat)
    : CObject(spec)
    , CLogged(log)
    , CMessageSender(disp, msg_stat)
    , m_graphics(graphics)
    , m_ph_global(ph_global)
    , m_sh_global(sh_global)
    , m_core_show_param(core_show_param)
{

}

bool CObjectShow::Draw()
{
    if (PhysicsContained().IsContained())
        return false;
    bool in_view = m_view->PhysicalInView(Shape(), Position());
    if (not in_view)
    {
        if (m_trace.empty())
            return false;
        for (const sTracePoint &tp : m_trace)
        {
            CShape sh;
            sh.AddPoint(tp.point);
            in_view = m_view->PhysicalInView(sh, {0.0, 0.0});
            if (in_view) break;
        }
        return false;
    }
    return DrawOpenGL();
}

bool CObjectShow::DrawOpenGL()
{
    m_draw->PushMatrix();
    m_draw->Scale(m_view->Scale(), m_view->Scale(), 1.0);
    m_draw->Translate(-m_view->Center().x, -m_view->Center().y, 0.0);
    {
        DrawPhysics();
        bool draw_graphics = ((m_sh_global->current_sh_height == ShowHeight()) and m_core_show_param->show_graphics);
        if (draw_graphics) DrawGraphics();
    }
    m_draw->PopMatrix();
    return true;
}

void CObjectShow::DrawPhysics()
{
    bool draw_obj = not PhysicsState().IsDestroyed();
    bool draw_shape = (m_sh_global->current_sh_height == ShowHeight()) and draw_obj and m_core_show_param->show_shape;
    bool draw_sight = (m_sh_global->HeightGround()) and draw_obj;
    bool draw_velocity = (m_sh_global->HeightGround()) and draw_obj and m_core_show_param->show_velocity;
    bool draw_health = (m_sh_global->HeightTop()) and draw_obj;

    if (draw_shape) DrawShape();
    if (draw_velocity) DrawVelocity();
    if (draw_sight) DrawSight();
    if (draw_health)
    {
        DrawHealth();
    }
    else
    {
        m_gui_bar_health = nullptr;
    }

    if (PhysicsState().IsDestroyed() and (m_trace.empty()))
        onEndAnimations();
}

void CObjectShow::DrawShape()
{
    m_draw->PushMatrix();
    m_draw->Translate(Position().x, Position().y, 0.0);
    m_draw->Color4(0.8, 0.0, 0.0, 0.2);
    //m_show_util->Draw(Shape().RectAxes());
    const auto shape = Shape();
    m_draw_figure_ra->SetFigure(shape.RectAxes());
    m_draw_figure_ra->lined = true;
    m_draw_figure_ra->line_width = 1.0;
    m_draw_figure_ra->Draw();
    m_draw->Color4(0.4, 0.8, 0.4, 0.8);
    m_draw_shape->SetShape(shape);
    m_draw_shape->lined = true;
    m_draw_shape->line_width = 2.0;
    m_draw_shape->Draw();
    m_draw->PopMatrix();
}

void CObjectShow::DrawVelocity()
{
    auto vl = Velocity();
    auto ps = Position();
    auto ps_end = ps + vl;
    m_draw->Color4(1.0, 0.0, 1.0, 0.1);
    m_draw->DrawLines2({{ps.x, ps.y}, {ps_end.x, ps_end.y}});
}

void CObjectShow::DrawSight()
{
    auto ps = Position();
    auto dr = Direction();
    auto sd = Perseption().sight_distance;
    Real_t dr_angle = atan2(dr.vy.Val(), dr.vx.Val());
    auto start_angle = dr_angle - Perseption().sight_angle/2.0;
    //auto end_angle = dr_angle + Perseption().sight_angle/2.0;

    if (not Perseption().sight_distance.IsNul())
    {
        //m_draw->BeginTriangleFan();
        std::vector<std::array<Real_t, 2>> vertexes;
        m_draw->Color4(0.0, 1.0, 0.0, 0.05);
        //m_draw->Vertex2(ps.x, ps.y);
        vertexes.push_back({ps.x, ps.y});
        Real_t delta_angle = Perseption().sight_angle/12.0;
        auto a = start_angle;
        for (auto i = 0; i < 12; ++i)
        {
            //m_draw->Vertex2(ps.x + sd*cos(a.Val()), ps.y + sd*sin(a.Val()));
            vertexes.push_back({ps.x + sd*cos(a.Val()), ps.y + sd*sin(a.Val())});
            a += delta_angle;
        }
        m_draw->DrawTriangleFan2(vertexes);
        //m_draw->End();
    }
}

void CObjectShow::DrawHealth()
{
    if (not Health().UsesHealth()) return;
    if (ObjSpec().IsProjectile()) return;
    auto ps = Position();
    Real_t h_p = Health().Percent();
    //Real_t bar_width = Size()*2.0;
    Real_t x0 = ps.x - Size();
    //Real_t x1 = x0 + bar_width*h_p;
    //Real_t x2 = x0 + bar_width;
    Real_t y = ps.y - Size()*1.25;
    /*m_draw->Color4(0.0, 1.0, 0.0, 1.0);
    m_draw->DrawLines2({{x0, y}, {x1, y}});
    m_draw->Color4(1.0, 0.0, 0.0, 1.0);
    m_draw->DrawLines2({{x1, y}, {x2, y}});*/

    if (not m_gui_bar_health)
        m_gui_bar_health = CGuiElementProgressBarPtr(new CGuiElementProgressBar(m_draw));
    m_gui_bar_health->SetUseTransform(true);
    m_gui_bar_health->SetSize(Size().Val()*2.0, Size().Val()*0.2);
    std::array<float, 2> position {{static_cast<float>(x0.Val()), static_cast<float>(y.Val())}};
    m_gui_bar_health->SetPosition(position);
    m_gui_bar_health->SetProgress(h_p.Val());
    m_gui_bar_health->Draw();
}

void CObjectShow::DrawTrace()
{
    if (not ObjSpec().IsProjectile()) return;
    ProcTrace();
    if (m_trace.size() < 2) return;
    std::vector<std::array<Real_t, 3>> points;
    double cntr = 0;
    //double cnt_all = static_cast<double>(m_trace.size());
    for (const sTracePoint &tp : m_trace)
    {
        const sCoord2 &pos = tp.point;
        auto cur_time = m_ph_global->physics_time;
        points.push_back({pos.x, pos.y, cur_time - tp.add_time});
        cntr++;
    }
    m_draw->Color4(1.0, 1.0, 1.0, 1.0);
    m_draw_trace->SetTrace(points);
    m_draw_trace->Draw();
}

void CObjectShow::ProcTrace()
{
    //printf("ProcTrace size=%ld;\n", m_trace.size());
    if (m_trace.empty()) return;
    auto cur_time = m_ph_global->physics_time;
    //if ((PhysicsState().IsDestroyed()) and (m_trace.size() <= 11))
    //    cur_time += 0;
    auto t_it = m_trace.begin();
    while (t_it != m_trace.end())
    {
        if ((cur_time - (*t_it).add_time) > TraceTTL())
        {
            t_it = m_trace.erase(t_it);
            continue;
        }
        t_it++;
    }
    if (m_trace.empty())
        onEndAnimateTrace();
}

void CObjectShow::onEndAnimateTrace()
{
    if (PhysicsState().IsDestroyed())
        onEndAnimations();
}

void CObjectShow::onEndAnimations()
{
    Send_EndAnimations(GameplayActor(), GetID());
}

void CObjectShow::SetKinematic(const sKinematicParam &ph_kn_param)
{
    Kinematic() = ph_kn_param;
    if (ObjSpec().IsProjectile())
    {
        if (not m_trace.empty())
        {
            if (m_trace.back().point == ph_kn_param.position)
                return;
        }
        m_trace.push_back({ph_kn_param.position, m_ph_global->physics_time});
    }
}

void CObjectShow::SetPhysicsState(const sPhysicsState &ph_state)
{
    PhysicsState() = ph_state;
    if ((PhysicsState().IsDestroyed()) and (m_trace.empty()))
        onEndAnimations();
}

void CObjectShow::SetPhysicsParam(const sPhysicsParamMsg &ph_param)
{
    Physics().FromMsg(ph_param);
    if ((PhysicsState().IsDestroyed()) and (m_trace.empty()))
        onEndAnimations();
}
