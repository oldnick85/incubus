#ifndef SHOW_H
#define SHOW_H

#include <mutex>
#include <thread>
#include "incubus_common.h"
#include "common_show.h"
#include "../logger/i_logger.h"
#include "../dispatcher/actor.h"
#include "cores/messages/messagecallbacks.h"
#include "cores/messages/messagesender.h"
#include "objects/objectshow.h"
#include "objects/show_obj_classes/objectshow_mob.h"
#include "objects/show_obj_classes/objectshow_projectile.h"
#include "objects/show_obj_classes/objectshow_loot.h"
#include "objects/show_obj_classes/objectshow_construction.h"
#include "media/sound.h"
#include "media/mediagraphics.h"
#include "media/mediasound.h"
#include "cores/coresstatistic.h"
#include "show_gui/showgui.h"
#include "draw/drawopengl3.h"
#include "draw/drawtext.h"

class CShow
        : public CMessageCallbacks
        , private CLogged
        , public CCoresParam
        , public CMessageSender
{
public:
    CShow(const ILoggerPtr log, const IDispatcherPtr disp);
    virtual ~CShow() override;
    virtual void onMessage() override final;
    virtual void onTimer(const TimerID_t timer_id) override final;
    virtual void onStart()  override final;
    virtual void onStop()  override final;
    // CActorCallbacks
    virtual bool onNewObjectShow(const ObjID_t obj_id, const sObjSpec &spec, const sShowParam &sh_param, const sPhysicsParamMsg &ph_param) override final;
    virtual bool onDelObjectShow(const ObjID_t obj_id) override final;
    virtual bool onSetPosition(const ObjID_t obj_id, const sCoord2 &position) override final;
    virtual bool onSetHealth(const ObjID_t obj_id, const sHealthParam &health) override final;
    virtual bool onSetKinematicParam(const ObjID_t obj_id, const sKinematicParam &ph_kn_param) override final;
    virtual bool onViewChange(const sVect2 &center_move, const Real_t &scale) override final;
    virtual bool onGameplayModeChange(const sGameplayMode &gp_mode) override final;
    virtual bool onGlobalPhysicsParam(const sGlobalPhysicsParam &ph_global) override final;
    virtual bool onSetPhysicsState(const ObjID_t obj_id, const sPhysicsState &state) override final;
    virtual bool onSetPhysicsParam(const ObjID_t obj_id, const sPhysicsParamMsg &ph_param) override final;
    virtual bool onStatisticGp(const sCoreStatisticGp &gp_stat) override final;
    virtual bool onStatisticPh(const sCoreStatisticPh &ph_stat) override final;
    virtual bool onStatisticSh(const sCoreStatisticSh &sh_stat) override final;
    virtual bool onStatisticBh(const sCoreStatisticBh &bh_stat) override final;
    virtual bool onCoreShowParam(const sCoreShowParam &param) override final;
    virtual bool onSoundEvent(const sSoundParam &sound_param) override final;
    virtual bool onGuiControlAddButton(const GuiControlId_t id, const sGuiButtonParam &param) override final;
    virtual bool onGuiControlDel(const GuiControlId_t id) override final;
private:
    bool AddObject(const ObjID_t &id, const sObjSpec &spec, const sShowParam &param, const sPhysicsParam &ph_param);
    bool DelObject(const ObjID_t &id);
    void CheckEvents();
    void GetEvents();
    void CheckMouseLeftButton();
    void CheckMouseRightButton();
    void CheckKeysPress(const Real_t mouse_x, const Real_t mouse_y);
    //void DrawOpenGL_back();
    void DrawFPS();
    void DrawStatistic();
    void Show();

    std::mutex      m_mtx;
    TimerID_t       m_tm_1sec = TIMER_ID_NONE;
    TimerID_t       m_tm_show = TIMER_ID_NONE;

    uint_32         m_frames_count = 0;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_last_fps_calc_time;
    Real_t          m_fps;
    Real_t          m_d = 0.0;

    std::unordered_map<ObjID_t, CObjectShowPtr> m_objects;

    CShowUtilPtr        m_show_util;
    CSoundUtilPtr       m_sound_util;
    CMediaGraphicsPtr   m_graphics;
    CMediaSoundPtr      m_sound;
    CShowGuiPtr         m_gui;

    struct sEventsState
    {
        sEventsState() = default;
        int     window_height = 640;
        int     window_width = 640;
        sCoord2 mouse_pos;
        sCoord2 view_pos;
        sCoord2 ph_pos;
        bool    mouse_button_left_pressed_change = false;
        bool    mouse_button_right_pressed_change = false;
        bool    mouse_button_left_pressed = false;
        bool    mouse_button_right_pressed = false;
        bool    keys_pressed[GLFW_KEY_LAST];
    };
    std::array<int,10>   m_keys_check = {GLFW_KEY_RIGHT, GLFW_KEY_LEFT, GLFW_KEY_DOWN, GLFW_KEY_UP, GLFW_KEY_KP_SUBTRACT, GLFW_KEY_KP_ADD,
                                        GLFW_KEY_TAB, GLFW_KEY_S, GLFW_KEY_G, GLFW_KEY_P};
    std::array<int,3>   m_keys_modifier_check = {GLFW_KEY_LEFT_SHIFT, GLFW_KEY_LEFT_CONTROL, GLFW_KEY_LEFT_ALT};
    sEventsState        m_events_state;

    CShowViewCoord      m_view;
    sGameplayMode       m_gp_mode;

    sGlobalPhysicsParam m_ph_global;
    sGlobalShowParam    m_sh_global;

    sCoresStatistic     m_cores_stat;
    sCoreShowParam      m_core_show_param;

    IDrawEnginePtr      m_draw;
    CDrawTextPtr        m_draw_fps;
};
#endif // SHOW_H
