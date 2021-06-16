#include "gameplay.h"

CGameplay::CGameplay(const ILoggerPtr log, const IDispatcherPtr disp)
    : CMessageCallbacks(log, "GAMEPLAY")
    , CLogged(log)
    , CMessageSender(disp, &GetMessagesStatistic())
{
    log_debug1("CGameplay::CGameplay");

    m_ph.reset(new CPhysics(Log(), disp));
    m_bh.reset(new CBehavior(Log(), disp));
    m_sh.reset(new CShow(Log(), disp));

    m_id.type = enActor::actGameplay;
    Dispatcher()->RegisterActor(this);
    SetSelfActor(GetActorID());

    SetCores(GetActorID(), m_ph->GetActorID(), m_bh->GetActorID(), m_sh->GetActorID());
    m_ph->SetCores(Cores());
    m_bh->SetCores(Cores());
    m_sh->SetCores(Cores());

    m_obj_generator.reset(new CGameplayObjectGenerator(Cores(), Dispatcher(), Log(), &GetMessagesStatistic()));
}

CGameplay::~CGameplay()
{
    log_debug1("CGameplay::~CGameplay");
    auto obj_it = m_objects.begin();
    while (obj_it != m_objects.end())
    {
        auto obj_it_next = obj_it;
        obj_it_next++;
        auto obj = obj_it->second;
        DelObject(obj->GetID());
        obj_it = obj_it_next;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    Dispatcher()->UnregisterActor(this);
}

void CGameplay::onMessage()
{
    log_debug3("CGameplay::onMessage %s", m_message->print().c_str());
    bool callback_defined = ActorCallbacks_onMessage();
    assert(callback_defined);
}

bool CGameplay::onWindowClose(const bool close)
{
    if (close)
        Dispatcher()->Stop();
    return true;
}

bool CGameplay::onMouseEvent(const sMsgWindowMouseEvent::enWindowMouseEvent event, const sCoord2 &pos, bool shift_pressed, bool control_pressed, bool alt_pressed)
{
    switch (m_mode.mode)
    {
    case gmEdit:    return ProcMouseEventEdit(event, pos, shift_pressed, control_pressed, alt_pressed);
    case gmPlay:    return ProcMouseEventPlay(event, pos, shift_pressed, control_pressed, alt_pressed);
    }
    return true;
}

bool CGameplay::ProcMouseEventEdit(const sMsgWindowMouseEvent::enWindowMouseEvent event, const sCoord2 &pos, bool shift_pressed, bool control_pressed, bool alt_pressed)
{
    log_debug1("%s %u; %s; %u%u%u;", __PRETTY_FUNCTION__, event, pos.print().c_str(), shift_pressed, control_pressed, alt_pressed);
    if (event == sMsgWindowMouseEvent::enWindowMouseEvent::evButtonLeftDown)
    {
        AddObjectZombie(pos);
    }
    return true;
}

bool CGameplay::ProcMouseEventPlay(const sMsgWindowMouseEvent::enWindowMouseEvent event, const sCoord2 &pos, bool shift_pressed, bool control_pressed, bool alt_pressed)
{
    log_debug1("%s %u; %s; %u%u%u;", __PRETTY_FUNCTION__, event, pos.print().c_str(), shift_pressed, control_pressed, alt_pressed);
    if (event == sMsgWindowMouseEvent::enWindowMouseEvent::evButtonLeftDown)
    {
        sPhysicsGoal goal;
        goal.GoToPoint(pos);
        Send_SetPhysicsGoal(PhysicsActor(), m_mode.player_id, goal);
    }
    if (event == sMsgWindowMouseEvent::enWindowMouseEvent::evButtonRightDown)
    {
        auto p_o = m_objects[m_mode.player_id];
        auto p_o_pos = p_o->m_ph_param.kinematic.position;
        Send_GameActionAttack(BehaviorActor(), m_mode.player_id, p_o_pos, pos);
    }
    return true;
}

bool CGameplay::onKeyEvent(const int key, const sMsgWindowKeyboardEvent::enWindowKeyboardEvent event, const sCoord2 &pos, bool shift_pressed, bool control_pressed, bool alt_pressed)
{
    log_debug1("%s %u; %u; %s; %u%u%u;", __PRETTY_FUNCTION__, key, event, pos.print().c_str(), shift_pressed, control_pressed, alt_pressed);
    if ((key == GLFW_KEY_RIGHT) and (event == sMsgWindowKeyboardEvent::enWindowKeyboardEvent::evKeyDown))
    {
        Send_ViewChange(ShowActor(), {shift_pressed ? 0.1 : 1.0, 0.0}, 1.0);
    }
    if ((key == GLFW_KEY_LEFT) and (event == sMsgWindowKeyboardEvent::enWindowKeyboardEvent::evKeyDown))
    {
        Send_ViewChange(ShowActor(), {shift_pressed ? -0.1 : -1.0, 0.0}, 1.0);
    }
    if ((key == GLFW_KEY_DOWN) and (event == sMsgWindowKeyboardEvent::enWindowKeyboardEvent::evKeyDown))
    {
        Send_ViewChange(ShowActor(), {0.0, shift_pressed ? -0.1 : -1.0}, 1.0);
    }
    if ((key == GLFW_KEY_UP) and (event == sMsgWindowKeyboardEvent::enWindowKeyboardEvent::evKeyDown))
    {
        Send_ViewChange(ShowActor(), {0.0, shift_pressed ? 0.1 : 1.0}, 1.0);
    }
    if ((key == GLFW_KEY_KP_SUBTRACT) and (event == sMsgWindowKeyboardEvent::enWindowKeyboardEvent::evKeyDown))
    {
        Send_ViewChange(ShowActor(), {0.0, 0.0}, 0.5);
    }
    if ((key == GLFW_KEY_KP_ADD) and (event == sMsgWindowKeyboardEvent::enWindowKeyboardEvent::evKeyDown))
    {
        Send_ViewChange(ShowActor(), {0.0, 0.0}, 2.0);
    }
    if ((key == GLFW_KEY_TAB) and (event == sMsgWindowKeyboardEvent::enWindowKeyboardEvent::evKeyDown))
    {
        m_mode.mode = (m_mode.mode == gmEdit) ? gmPlay : gmEdit;
        Send_GameplayModeChange(ShowActor(), m_mode);
    }
    if ((key == GLFW_KEY_S) and (event == sMsgWindowKeyboardEvent::enWindowKeyboardEvent::evKeyDown))
    {
        if (control_pressed)
        {
            m_core_show_param.show_statistic = not m_core_show_param.show_statistic;
            Send_CoreShowParam(ShowActor(), m_core_show_param);
        }
    }
    if ((key == GLFW_KEY_G) and (event == sMsgWindowKeyboardEvent::enWindowKeyboardEvent::evKeyDown))
    {
        if (control_pressed)
        {
            m_core_show_param.show_graphics = not m_core_show_param.show_graphics;
            Send_CoreShowParam(ShowActor(), m_core_show_param);
        }
    }
    if ((key == GLFW_KEY_P) and (event == sMsgWindowKeyboardEvent::enWindowKeyboardEvent::evKeyDown))
    {
        if (control_pressed)
        {
            m_core_show_param.show_shape = not m_core_show_param.show_shape;
            m_core_show_param.show_velocity = not m_core_show_param.show_velocity;
            m_core_show_param.show_direction = not m_core_show_param.show_direction;
            Send_CoreShowParam(ShowActor(), m_core_show_param);
        }
    }
    return true;
}

bool CGameplay::onCreateProjectile(const sCoord2 &position, const sVect2 &velocity)
{
    AddObjectBullet(position, velocity);
    sSoundParam sound_param;
    sound_param.type = stShotPistol;
    sound_param.snd_pos = position;
    Send_SoundEvent(ShowActor(), sound_param);
    return true;
}

bool CGameplay::onDelObject(const ObjID_t obj_id)
{
    auto obj_it = m_objects.find(obj_id);
    if (obj_it == m_objects.end()) return true;
    auto obj = obj_it->second;
    obj->m_wait_for_delete = true;
    return true;
}

bool CGameplay::onEndAnimations(const ObjID_t obj_id)
{
    auto obj_it = m_objects.find(obj_id);
    if (obj_it == m_objects.end()) return true;
    auto obj = obj_it->second;
    if (obj->m_wait_for_delete)
        DelObject(obj_id);
    return true;
}

ObjID_t CGameplay::AddObjectBullet(const sCoord2 &pos, const sVect2 &vel)
{
    auto obj = m_obj_generator->MakeObject(opBullet);
    obj->m_ph_param.kinematic.position = pos;
    obj->m_ph_param.kinematic.velocity = vel;
    return AddObject(obj);
}

ObjID_t CGameplay::AddObjectPistol(const sCoord2 &pos)
{
    auto obj = m_obj_generator->MakeObject(opPistol);
    obj->m_ph_param.kinematic.position = pos;
    return AddObject(obj);
}

ObjID_t CGameplay::AddObjectZombie(const sCoord2 &pos)
{
    auto obj = m_obj_generator->MakeObject(opZombie);
    obj->m_ph_param.kinematic.position = pos;
    return AddObject(obj);
}

ObjID_t CGameplay::AddObjectHuman(const sCoord2 &pos)
{
    auto obj = m_obj_generator->MakeObject(opHuman);
    obj->m_ph_param.kinematic.position = pos;
    return AddObject(obj);
}

ObjID_t CGameplay::AddObjectWall()
{
    auto obj = m_obj_generator->MakeObject(opWall);
    obj->m_ph_param.kinematic.position = {0,0};
    return AddObject(obj);
}

void CGameplay::onTimer(const TimerID_t timer_id)
{
    log_debug3("CGameplay::onTimer %u;", timer_id);
    if (timer_id == m_tm_1sec)
    {
        m_statistic.common.objects_count = static_cast<uint_32>(m_objects.size());
        m_statistic.common.msg_stat.FromStat(GetMessagesStatistic());
        Send_StatisticGp(ShowActor(), m_statistic);
    }
    return;
}

void CGameplay::onStart()
{
    log_debug1("CGameplay::onStart");
    m_tm = Dispatcher()->GetTimer(GetActorID().id, IDispatcher::enTimerType::ttSkipping);
    Dispatcher()->StartTimerPeriodical(GetActorID().id, m_tm, 1000);
    m_tm_1sec = Dispatcher()->GetTimer(GetActorID().id, IDispatcher::enTimerType::ttSkipping);
    Dispatcher()->StartTimerPeriodical(GetActorID().id, m_tm_1sec, 1000);

    AddObjectWall();
    m_mode.player_id = AddObjectHuman({0.00, 1.00});
    auto obj_pistol = AddObjectPistol({0.00, 1.00});

    sObjectAction obj_act;
    obj_act.SetTake(m_mode.player_id, obj_pistol);
    Send_ObjectAction(PhysicsActor(), obj_act);

    sGuiButtonParam button_param;
    button_param.pos = {-0.95, 0.75};
    button_param.size = {0.3, 0.1};
    snprintf(button_param.caption, sizeof(button_param.caption), "Add zombie");
    Send_GuiControlAddButton(ShowActor(), 0, button_param);
}

void CGameplay::onStop()
{
    log_debug1("CGameplay::onStop");
    Dispatcher()->KillTimer(GetActorID().id, m_tm);
}

ObjID_t CGameplay::AddObject(CObjectGameplayPtr obj)
{
    m_cur_max_obj_id++;
    obj->SetID(m_cur_max_obj_id);
    m_objects[m_cur_max_obj_id] = obj;
    obj->CreateSubobjects();
    return m_cur_max_obj_id;
}

bool CGameplay::DelObject(const ObjID_t obj_id)
{
    auto obj_it = m_objects.find(obj_id);
    if (obj_it == m_objects.end()) return false;
    auto obj = obj_it->second;
    obj->DeleteSubobjects();
    m_objects.erase(obj_it);
    return true;
}
