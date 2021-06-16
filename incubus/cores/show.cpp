#include "show.h"

void GLFW_Error_Callback(int error, const char* description)
{
    fprintf(stderr, "Error: %u %s\n", error, description);
}

CShow::CShow(const ILoggerPtr log, const IDispatcherPtr disp)
    : CMessageCallbacks(log, "SHOW")
    , CLogged(log)
    , CMessageSender(disp, &GetMessagesStatistic())
{
    log_debug1("CShow::CShow");
    m_id.type = enActor::actShow;
    assert(Dispatcher()->RegisterActor(this));
    SetSelfActor(GetActorID());

    m_draw = std::make_shared<CDrawOpenGL3>();
    if (not m_draw->Init())
        assert(false);

    m_show_util = CShowUtilPtr(new CShowUtil(m_draw));
    m_sound_util = CSoundUtilPtr(new CSoundUtil());

    m_last_fps_calc_time = std::chrono::high_resolution_clock::now();

    m_graphics.reset(new CMediaGraphics(m_show_util, m_draw));
    m_sound.reset(new CMediaSound(&m_ph_global));

    m_gui = CShowGuiPtr(new CShowGui(m_log, m_show_util, m_sound_util, m_draw));
    m_draw_fps = CDrawTextPtr(new CDrawText(m_draw));
    m_draw_fps->SetPosition({-0.97, 0.92});
    m_draw_fps->SetSymbolWidth(0.03);
}

CShow::~CShow()
{
    log_debug1("CShow::~CShow");
    m_draw->Release();
    Dispatcher()->UnregisterActor(this);
}

void CShow::DrawFPS()
{
    if (not m_core_show_param.ShowFPS()) return;
    char c[64];
    snprintf(c, sizeof (c), "%.2f FPS (%s) %.2fsec",
             m_fps.Val(), (m_gp_mode.mode == gmEdit) ? "edit" : "play",
             m_ph_global.physics_time.Val());
    m_draw_fps->SetText(std::string(c));
    m_draw_fps->Draw();
}

void CShow::DrawStatistic()
{
    if (not m_core_show_param.ShowStatistic()) return;
    char c[64];
    Real_t x = -0.98;
    Real_t y = 0.88;

    snprintf(c, sizeof (c), "GP_STAT: %s", m_cores_stat.gp_stat.print().c_str());
    m_show_util->DrawText(c, x, y);

    x = -0.98;
    y -= 0.10;
    snprintf(c, sizeof (c), "PH_STAT: %s", m_cores_stat.ph_stat.print().c_str());
    m_show_util->DrawText(c, x, y);

    x = -0.98;
    y -= 0.10;
    snprintf(c, sizeof (c), "SH_STAT: %s", m_cores_stat.sh_stat.print().c_str());
    m_show_util->DrawText(c, x, y);

    x = -0.98;
    y -= 0.10;
    snprintf(c, sizeof (c), "BH_STAT: %s", m_cores_stat.bh_stat.print().c_str());
    m_show_util->DrawText(c, x, y);
}


/*void CShow::DrawOpenGL_back()
{
    m_d += 1;
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_POINTS);
    glColor3d(1.0, 1.0, 1.0);
    glVertex2d(0.94 + 0.05*sin(m_d.Val()/8), 0.94 + 0.05*cos(m_d.Val()/8));
    glColor3d(1.0, 0.0, 0.0);
    glVertex2d(0.94 + 0.045*sin(m_d.Val()/9), 0.94 + 0.045*cos(m_d.Val()/9));
    glColor3d(0.0, 1.0, 0.0);
    glVertex2d(0.94 + 0.04*sin(m_d.Val()/10), 0.94 + 0.04*cos(m_d.Val()/10));
    glColor3d(0.0, 0.0, 1.0);
    glVertex2d(0.94 + 0.035*sin(m_d.Val()/11), 0.94 + 0.035*cos(m_d.Val()/11));
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor4d(0.0, 0.0, 0.0, 0.1);
    sCoord2 c_m1m1 = m_view.PhysicalToView({-10.0, -10.0});
    sCoord2 c_p1m1 = m_view.PhysicalToView({+10.0, -10.0});
    sCoord2 c_m1p1 = m_view.PhysicalToView({-10.0, +10.0});
    sCoord2 c_p1p1 = m_view.PhysicalToView({+10.0, +10.0});
    glVertex2d(c_m1m1.x, c_m1m1.y);
    glVertex2d(c_m1p1.x, c_m1p1.y);
    glVertex2d(c_p1p1.x, c_p1p1.y);
    glVertex2d(c_p1m1.x, c_p1m1.y);
    glEnd();

    glBegin(GL_LINES);
    glColor4d(0.0, 0.0, 0.0, 0.1);
    c_m1m1 = m_view.PhysicalToView({-5.0, -5.0});
    c_p1m1 = m_view.PhysicalToView({+5.0, -5.0});
    c_m1p1 = m_view.PhysicalToView({-5.0, +5.0});
    c_p1p1 = m_view.PhysicalToView({+5.0, +5.0});
    glVertex2d(c_m1m1.x, c_m1m1.y);
    glVertex2d(c_p1p1.x, c_p1p1.y);
    glVertex2d(c_m1p1.x, c_m1p1.y);
    glVertex2d(c_p1m1.x, c_p1m1.y);
    glEnd();

    //GLuint VertexArrayID;
    //glGenVertexArrays(1, &VertexArrayID);
    //glBindVertexArray(VertexArrayID);

}*/

bool CShow::onNewObjectShow(const ObjID_t obj_id, const sObjSpec &spec, const sShowParam &sh_param, const sPhysicsParamMsg &ph_param)
{
    sPhysicsParam ph_p;
    ph_p.FromMsg(ph_param);
    AddObject(obj_id, spec, sh_param, ph_p);
    return true;
}

bool CShow::onDelObjectShow(const ObjID_t obj_id)
{
    DelObject(obj_id);
    return true;
}

bool CShow::onSetPosition(const ObjID_t obj_id, const sCoord2 &position)
{
    auto obj_it = m_objects.find(obj_id);
    if (obj_it != m_objects.end())
    {
        auto obj = obj_it->second;
        obj->Position() = position;
    }
    return true;
}

bool CShow::onSetHealth(const ObjID_t obj_id, const sHealthParam &health)
{
    auto obj_it = m_objects.find(obj_id);
    if (obj_it == m_objects.end()) return true;
    auto obj = obj_it->second;
    obj->Health() = health;
    return true;
}

bool CShow::onSetPhysicsState(const ObjID_t obj_id, const sPhysicsState &state)
{
    auto obj_it = m_objects.find(obj_id);
    if (obj_it == m_objects.end()) return false;
    auto obj = obj_it->second;
    obj->SetPhysicsState(state);
    return true;
}

bool CShow::onSetPhysicsParam(const ObjID_t obj_id, const sPhysicsParamMsg &ph_param)
{
    auto obj_it = m_objects.find(obj_id);
    if (obj_it == m_objects.end()) return false;
    auto obj = obj_it->second;
    obj->SetPhysicsParam(ph_param);
    return true;
}

bool CShow::onStatisticGp(const sCoreStatisticGp &gp_stat)
{
    m_cores_stat.gp_stat = gp_stat;
    return true;
}

bool CShow::onStatisticPh(const sCoreStatisticPh &ph_stat)
{
    m_cores_stat.ph_stat = ph_stat;
    return true;
}

bool CShow::onStatisticSh(const sCoreStatisticSh &sh_stat)
{
    m_cores_stat.sh_stat = sh_stat;
    return true;
}

bool CShow::onStatisticBh(const sCoreStatisticBh &bh_stat)
{
    m_cores_stat.bh_stat = bh_stat;
    return true;
}

bool CShow::onSetKinematicParam(const ObjID_t obj_id, const sKinematicParam &ph_kn_param)
{
    auto obj_it = m_objects.find(obj_id);
    if (obj_it != m_objects.end())
    {
        auto obj = obj_it->second;
        obj->SetKinematic(ph_kn_param);
    }
    return true;
}

bool CShow::onViewChange(const sVect2 &center_move, const Real_t &scale)
{
    m_view.Center() += center_move;
    auto new_scale = m_view.Scale()*scale;
    if ((new_scale < 0.5) and (new_scale > 0.02))
        m_view.Scale() = new_scale;
    m_sound->SetListenerPos(m_view.Center());
    return true;
}

bool CShow::onGameplayModeChange(const sGameplayMode &gp_mode)
{
    m_gp_mode = gp_mode;
    return true;
}

bool CShow::onGlobalPhysicsParam(const sGlobalPhysicsParam &ph_global)
{
    m_ph_global = ph_global;
    return true;
}

bool CShow::onCoreShowParam(const sCoreShowParam &param)
{
    m_core_show_param = param;
    return true;
}

bool CShow::onSoundEvent(const sSoundParam &sound_param)
{
    m_sound->PlaySound(sound_param);
    return true;
}

void CShow::onMessage()
{
    std::lock_guard<std::mutex> lk(m_mtx);
    log_debug3("CShow::onMessage %s", m_message->print().c_str());
    bool callback_defined = ActorCallbacks_onMessage();
    if (not callback_defined)
        log_warning("msg %s", m_message->print().c_str());
    assert(callback_defined);
}

void CShow::GetEvents()
{
    int mouse_button_left_pressed = m_draw->GetMouseButton(GLFW_MOUSE_BUTTON_LEFT);
    bool new_mouse_button_left_pressed = (mouse_button_left_pressed == GLFW_PRESS);
    m_events_state.mouse_button_left_pressed_change = (m_events_state.mouse_button_left_pressed != new_mouse_button_left_pressed);
    m_events_state.mouse_button_left_pressed = new_mouse_button_left_pressed;

    int mouse_button_right_pressed = m_draw->GetMouseButton(GLFW_MOUSE_BUTTON_RIGHT);
    bool new_mouse_button_right_pressed = (mouse_button_right_pressed == GLFW_PRESS);
    m_events_state.mouse_button_right_pressed_change = (m_events_state.mouse_button_right_pressed != new_mouse_button_right_pressed);
    m_events_state.mouse_button_right_pressed = new_mouse_button_right_pressed;
}

void CShow::CheckMouseLeftButton()
{
    if (m_events_state.mouse_button_left_pressed_change)
    {
        auto event = m_events_state.mouse_button_left_pressed ? sMsgWindowMouseEvent::enWindowMouseEvent::evButtonLeftDown : sMsgWindowMouseEvent::enWindowMouseEvent::evButtonLeftUp;
        Send_WindowMouseEvent(GameplayActor(), event, m_events_state.ph_pos);
    }
}

void CShow::CheckMouseRightButton()
{
    if (m_events_state.mouse_button_right_pressed_change)
    {
        auto event = m_events_state.mouse_button_right_pressed ? sMsgWindowMouseEvent::enWindowMouseEvent::evButtonRightDown : sMsgWindowMouseEvent::enWindowMouseEvent::evButtonRightUp;
        Send_WindowMouseEvent(GameplayActor(), event, m_events_state.ph_pos);
    }
}

void CShow::CheckKeysPress(const Real_t mouse_x, const Real_t mouse_y)
{
    for (int key: m_keys_modifier_check)
    {
        int key_pressed = m_draw->GetKey(key);
        bool new_key_pressed = (key_pressed == GLFW_PRESS);
        m_events_state.keys_pressed[key] = new_key_pressed;
    }
    for (int key : m_keys_check)
    {
        int key_pressed = m_draw->GetKey(key);
        bool new_key_pressed = (key_pressed == GLFW_PRESS);
        if (m_events_state.keys_pressed[key] != new_key_pressed)
        {
            auto event = new_key_pressed ? sMsgWindowKeyboardEvent::enWindowKeyboardEvent::evKeyDown : sMsgWindowKeyboardEvent::enWindowKeyboardEvent::evKeyUp;
            Send_WindowKeyboardEvent(GameplayActor(), key, event, {mouse_x, mouse_y}, m_events_state.keys_pressed[GLFW_KEY_LEFT_SHIFT],
                                     m_events_state.keys_pressed[GLFW_KEY_LEFT_CONTROL], m_events_state.keys_pressed[GLFW_KEY_LEFT_ALT]);
            m_events_state.keys_pressed[key] = new_key_pressed;
        }
    }
}

void CShow::CheckEvents()
{
    glfwPollEvents();
    Real_t mouse_x;
    Real_t mouse_y;
    m_draw->GetCursorPos(mouse_x.Val(), mouse_y.Val());
    m_events_state.mouse_pos = {mouse_x, mouse_y};
    m_events_state.view_pos = m_view.WindowToView(m_events_state.mouse_pos);
    m_events_state.ph_pos = m_view.WindowToPhysical(m_events_state.mouse_pos);
    GetEvents();
    if (not m_gui->onMouse(m_events_state.view_pos))
    {
        CheckMouseLeftButton();
        CheckMouseRightButton();
    }
    CheckKeysPress(mouse_x, mouse_y);

    if (m_draw->ShouldCloseWindow())
    {
        Send_WindowClose(GameplayActor(), true);
    }
}

void CShow::Show()
{
    m_draw->ContextHold();
    m_draw->Clear();
    CheckEvents();

    for (int i = sShowParam::enShowHeight::shGround; i <= sShowParam::enShowHeight::shTop; ++i)
    {
        m_sh_global.current_sh_height = static_cast<sShowParam::enShowHeight>(i);
        m_sh_global.current_z = static_cast<double>(m_sh_global.current_sh_height);
        for (auto &o : m_objects)
        {
            auto obj = o.second;
            obj->Draw();
        }
    }

    m_gui->Draw();
    DrawFPS();
    //DrawStatistic();

    m_draw->SwapBuffers();
    m_draw->ContextRelease();

    m_frames_count++;
    if ((m_frames_count % 25) == 0)
    {
        auto now = std::chrono::high_resolution_clock::now();
        uint_32 d_ms = static_cast<uint_32>(std::chrono::duration_cast<std::chrono::milliseconds>(now-m_last_fps_calc_time).count());
        m_fps = (static_cast<Real_t>(m_frames_count)*1000.0)/static_cast<Real_t>(d_ms);
        m_frames_count = 0;
        m_last_fps_calc_time = now;
        //log_warning("FPS %f", m_fps);
    }
}

void CShow::onTimer(const TimerID_t timer_id)
{
    std::lock_guard<std::mutex> lk(m_mtx);
    log_debug3("CShow::onTimer %u; start", timer_id);
    if (timer_id == m_tm_show)
    {
        Show();
    }
    else if (timer_id == m_tm_1sec)
    {
        m_sound->ClearSources();
        m_cores_stat.sh_stat.common.objects_count = static_cast<uint_32>(m_objects.size());
        m_cores_stat.sh_stat.common.msg_stat.FromStat(GetMessagesStatistic());
        m_sound->GetStatistic(m_cores_stat.sh_stat.sound);
    }

    Send_RequestPhysicsParamAll(PhysicsActor(), true);
    log_debug3("CShow::onTimer %u; end", timer_id);
}

void CShow::onStart()
{
    log_debug1("CShow::onStart");
    //InitOpenGL();
    m_tm_show = Dispatcher()->GetTimer(GetActorID().id, IDispatcher::enTimerType::ttSkipping);
    Dispatcher()->StartTimerPeriodical(GetActorID().id, m_tm_show, 40);
    m_tm_1sec = Dispatcher()->GetTimer(GetActorID().id, IDispatcher::enTimerType::ttContinuing);
    Dispatcher()->StartTimerPeriodical(GetActorID().id, m_tm_1sec, 1000);
}

void CShow::onStop()
{
    log_debug1("CShow::onStop");
}

bool CShow::AddObject(const ObjID_t &id, const sObjSpec &spec, const sShowParam &param, const sPhysicsParam &ph_param)
{
    log_debug3("CShow::AddObject id=%u; %s;", id, param.print().c_str());

    CObjectShowPtr obj;
    if (spec.IsMob())
        obj = CObjectShowPtr(new CObjectShow_Mob(m_log, Dispatcher(), &m_ph_global, &m_sh_global, m_graphics, &m_core_show_param, spec, &GetMessagesStatistic()));
    else if (spec.IsProjectile())
        obj = CObjectShowPtr(new CObjectShow_Projectile(m_log, Dispatcher(), &m_ph_global, &m_sh_global, m_graphics, &m_core_show_param, spec, &GetMessagesStatistic()));
    else if (spec.IsLoot())
        obj = CObjectShowPtr(new CObjectShow_Loot(m_log, Dispatcher(), &m_ph_global, &m_sh_global, m_graphics, &m_core_show_param, spec, &GetMessagesStatistic()));
    else if (spec.IsConstruction())
        obj = CObjectShowPtr(new CObjectShow_Construction(m_log, Dispatcher(), &m_ph_global, &m_sh_global, m_graphics, &m_core_show_param, spec, &GetMessagesStatistic()));
    else
        assert(false);

    obj->SetID(id);
    obj->SetCores(GameplayActor(), PhysicsActor(), BehaviorActor(), GetActorID());
    obj->SetShowParam(param);
    obj->Physics() = ph_param;
    obj->SetUtil(m_show_util, m_sound_util, &m_view, m_draw);
    obj->SetKinematic(ph_param.kinematic);
    m_objects[id] = obj;
    return true;
}

bool CShow::DelObject(const ObjID_t &id)
{
    log_debug3("CShow::DelObject id=%u;", id);
    auto obj_it = m_objects.find(id);
    if (obj_it == m_objects.end()) return false;
    CObjectShowPtr obj = obj_it->second;
    m_objects.erase(obj_it);
    obj = nullptr;
    return true;
}

bool CShow::onGuiControlAddButton(const GuiControlId_t id, const sGuiButtonParam &param)
{
    log_debug3("%s %u %s", __FUNCTION__, id, param.print().c_str());
    m_gui->AddButton(id, param);
    return true;
}

bool CShow::onGuiControlDel(const GuiControlId_t id)
{
    log_debug3("%s %u", __FUNCTION__, id);
    m_gui->Del(id);
    return true;
}
