#ifndef OBJECTSHOW_H
#define OBJECTSHOW_H

#include <list>

#include "cores/common_show.h"
#include "../logger/i_logger.h"
#include "object.h"
#include "cores/messages/messagesender.h"
#include "objects/show_obj_classes/ishobjclass.h"
#include "media/sound.h"
#include "media/mediagraphics.h"
#include "cores/draw/drawshape.h"
#include "cores/draw/drawtrace.h"
#include "cores/show_gui/guielementprogressbar.h"

class CObjectShow
        : public CObject
        , public CCoresParam
        , public CPhysicsParam
        , public CShowParam
        , private CLogged
        , public CMessageSender
{
public:
    CObjectShow(const ILoggerPtr log, const IDispatcherPtr disp,
                sGlobalPhysicsParam* ph_global, sGlobalShowParam* sh_global,
                CMediaGraphicsPtr graphics, sCoreShowParam* core_show_param, const sObjSpec &spec, sDispatcherMsgStatistic *msg_stat);
    CObjectShow(const CObjectShow &other) = delete;

    virtual ~CObjectShow() = default;

    void SetUtil(CShowUtilPtr show_util, CSoundUtilPtr sound_util, CShowViewCoord* view, IDrawEnginePtr draw)
    {
        m_show_util = show_util;
        m_sound_util = sound_util;
        m_view = view;
        m_draw = draw;
        m_draw_shape = CDrawShapePtr(new CDrawShape(m_draw));
        m_draw_figure_ra = CDrawFigurePtr(new CDrawFigure(m_draw));
        m_draw_trace = CDrawTracePtr(new CDrawTrace(m_draw));
    }

    void SetKinematic(const sKinematicParam &ph_kn_param);
    void SetPhysicsState(const sPhysicsState &ph_state);
    void SetPhysicsParam(const sPhysicsParamMsg &ph_param);

    bool Draw();
protected:
    virtual void DrawPhysics();
    void DrawTrace();

    CMediaGraphicsPtr               m_graphics;
    sGlobalPhysicsParam*            m_ph_global = nullptr;
    sGlobalShowParam*               m_sh_global = nullptr;

private:
    bool DrawOpenGL();
    virtual void DrawGraphics() {}
    void DrawShape();
    void DrawVelocity();
    void DrawSight();
    void DrawHealth();
    void ProcTrace();
    void onEndAnimateTrace();
    void onEndAnimations();
    CShowUtilPtr                    m_show_util = nullptr;
    IDrawEnginePtr                  m_draw;
    CSoundUtilPtr                   m_sound_util = nullptr;
    CShowViewCoord*                 m_view = nullptr;
    std::shared_ptr<IShObjClass>    m_class;

    struct sTracePoint
    {
        sTracePoint(const sCoord2 p, const double t) {point = p; add_time = t;}

        Real_t      add_time = 0.0;
        sCoord2     point;
    };

    std::list<sTracePoint>          m_trace;

    sCoreShowParam*                 m_core_show_param;
    CDrawShapePtr                   m_draw_shape;
    CDrawFigurePtr                  m_draw_figure_ra;
    CDrawTracePtr                   m_draw_trace;

    CGuiElementProgressBarPtr       m_gui_bar_health;
};
using CObjectShowPtr = std::shared_ptr<CObjectShow>;

#endif // OBJECTSHOW_H
