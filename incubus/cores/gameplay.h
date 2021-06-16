#ifndef GAMEPLAY_H
#define GAMEPLAY_H
#include "incubus_common.h"
#include "../logger/i_logger.h"
#include "../dispatcher/actor.h"
#include "messages/messagecallbacks.h"
#include "messages/messagesender.h"
#include "physics.h"
#include "behavior.h"
#include "show.h"
#include "objects/objectgameplay.h"
#include "objects/gameplayobjectgenerator.h"
#include "coresstatistic.h"

/**
 * @brief CGameplay - класс, реализующий базовую игровую логику.
 * @details Этот класс выполняет основные действия по созданию и удалению объектов, интерпретирует пользовательские события,
 *          а также выполняет скрипты и сценарии.
 */

class CGameplay
        : private CMessageCallbacks
        , private CLogged
        , public CCoresParam
        , public CMessageSender
{
public:
    CGameplay(const ILoggerPtr log, const IDispatcherPtr disp);
    virtual ~CGameplay() override;
    // IActor
    virtual void onMessage() override final;
    virtual void onTimer(const TimerID_t timer_id) override final;
    virtual void onStart()  override final;
    virtual void onStop()  override final;
    // CActorCallbacks
    virtual bool onWindowClose(const bool close) override final;
    virtual bool onMouseEvent(const sMsgWindowMouseEvent::enWindowMouseEvent event, const sCoord2 &pos,
                              bool shift_pressed, bool control_pressed, bool alt_pressed) override final;
    virtual bool onKeyEvent(const int key, const sMsgWindowKeyboardEvent::enWindowKeyboardEvent event, const sCoord2 &pos,
                            bool shift_pressed, bool control_pressed, bool alt_pressed) override final;
    virtual bool onCreateProjectile(const sCoord2 &position, const sVect2 &velocity) override final;
    virtual bool onDelObject(const ObjID_t obj_id) override final;
    virtual bool onEndAnimations(const ObjID_t obj_id) override final;

    ObjID_t AddObject(CObjectGameplayPtr obj);
    //ObjID_t AddObject(const sObjGpParam &param, const sShowParam &sh_param, const sPhysicsParam &ph_param);
    bool DelObject(const ObjID_t obj_id);
private:
    ObjID_t AddObjectZombie(const sCoord2 &pos = sCoord2());
    ObjID_t AddObjectHuman(const sCoord2 &pos = sCoord2());
    ObjID_t AddObjectWall();
    ObjID_t AddObjectBullet(const sCoord2 &pos, const sVect2 &vel);
    ObjID_t AddObjectPistol(const sCoord2 &pos);
    bool ProcMouseEventEdit(const sMsgWindowMouseEvent::enWindowMouseEvent event, const sCoord2 &pos, bool shift_pressed, bool control_pressed, bool alt_pressed);
    bool ProcMouseEventPlay(const sMsgWindowMouseEvent::enWindowMouseEvent event, const sCoord2 &pos, bool shift_pressed, bool control_pressed, bool alt_pressed);

    TimerID_t       m_tm = TIMER_ID_NONE;
    TimerID_t       m_tm_1sec = TIMER_ID_NONE;
    ObjID_t         m_cur_max_obj_id = 0;

    std::shared_ptr<CPhysics>   m_ph = nullptr;
    std::shared_ptr<CBehavior>  m_bh = nullptr;
    std::shared_ptr<CShow>      m_sh = nullptr;

    std::unordered_map<ObjID_t, CObjectGameplayPtr> m_objects;

    enum enGameControlMode
    {
        cmModify,
        cmControlObject,
    };

    enGameControlMode           m_control = cmModify;
    ObjID_t                     m_control_obj = OBJ_ID_NONE;
    sGameplayMode               m_mode;
    CGameplayObjectGeneratorPtr m_obj_generator;

    sCoreStatisticGp            m_statistic;

    sCoreShowParam              m_core_show_param;
};

#endif // GAMEPLAY_H
