#include "dispatcher.h"

CDispatcher::CDispatcher(ILoggerPtr log)
    : m_log(log)
{
    m_log->LogMsgDebug1("CDispatcher::CDispatcher");
    m_should_stop_dispatcher.store(true);
    m_waiting_for_workers.store(false);
    m_waiting_for_actions.store(false);
}

std::string CDispatcher::print_stat() const
{
    char s[128];
    snprintf(s, sizeof(s), "actors=%u;", ActorsCount());
    return s;
}

bool CDispatcher::RegisterActor(IActor* actor)
{
    if (not actor) return false;
    m_log->LogMsgDebug3("CDispatcher::RegisterActor");
    auto actr = AddActor(actor);
    if (not actr) return false;
    auto thr = AddThread();
    if (not thr) return false;
    thr->SetActor(actr);
    if (not m_should_stop_dispatcher.load())
        thr->Run();
    return true;
}

bool CDispatcher::UnregisterActor(IActor* actor)
{
    if (not actor) return false;
    m_log->LogMsgDebug3("CDispatcher::UnregisterActor");
    DelActor(actor->GetActorID().id);
    return true;
}

bool CDispatcher::SendMessage(const sMsg &message)
{
    log_debug3("CDispatcher::SendMessage %s", message.print().c_str());
    m_stat.messages_sent++;
    auto res = SendMsg(message);
    auto thr = FindThreadActor(message.destination.id);
    if (thr) thr->WakeUpForWork();
    return res;
}

void CDispatcher::Run()
{
    m_log->LogMsgDebug1("CDispatcher::Run");
    m_should_stop_dispatcher.store(false);
    std::thread dispatcher_thread(DispatcherCycle, this);
    RunAllWorkers();
    dispatcher_thread.join();
}

void CDispatcher::Stop()
{
    m_should_stop_dispatcher.store(true);
    StopAllWorkers();
    m_cv_timers_wait.notify_all();
}

void CDispatcher::LogDispatcherState(uint_64 i)
{
    char s[128];
    snprintf(s, sizeof(s), "i=%lu; %s",
             i, m_stat.print().c_str());
    m_log->LogMsgWarning(s);
}

void CDispatcher::DispatcherCycle(CDispatcher* dsp)
{
    auto disp_start_time = std::chrono::system_clock::now();
    while ((not dsp->m_should_stop_dispatcher.load()))
    {
        TimePoint_t tm_g;
        std::unordered_set<ActID_t> actors_timer_event;
        dsp->ProcTimers(tm_g, actors_timer_event);
#ifdef INCUBUS_LOG_DEBUG_DISPATCHER
        TimePoint_t now = std::chrono::system_clock::now();
        auto tm_ms = std::chrono::duration_cast<std::chrono::microseconds>(tm_g - now).count();
        char s[128];
        snprintf(s, sizeof (s), "disp cycle tm_ms=%ld;\n", tm_ms);
        dsp->m_log->LogMsgDebug3(s);
#endif
        for (ActID_t act_id : actors_timer_event)
        {
            auto thr = dsp->FindThreadActor(act_id);
            if (thr) thr->WakeUpForWork();
        }
        std::unique_lock<std::mutex> lk(dsp->m_mtx_timers_wait);
        dsp->m_cv_timers_wait.wait_until(lk, tm_g);
    }
    dsp->m_log->LogMsgWarning(dsp->m_stat.print());
    auto disp_end_time = std::chrono::system_clock::now();
    uint64_t msc = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(disp_end_time-disp_start_time).count());
    char s[128];
    double cycle_duration = 1000*static_cast<double>(msc)/static_cast<double>(dsp->m_stat.cycles);
    double cycles_per_sec = static_cast<double>(dsp->m_stat.cycles)/(static_cast<double>(msc)*0.001);
    snprintf(s, sizeof(s), "time=%lums; one_cycle=%fus; per_sec=%f;",
             msc, cycle_duration, cycles_per_sec);
    dsp->m_log->LogMsgWarning(s);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}


TimerID_t CDispatcher::GetTimer(const ActID_t a_id, const enTimerType type)
{
    log_debug3("CDispatcher::GetTimer act_id=%d;", a_id);
    return AddTmr(a_id, type);
}

bool CDispatcher::StartTimer(const ActID_t a_id, const TimerID_t timer_id, const uint_32 time_ms, const int_32 count)
{
    log_debug3("CDispatcher::StartTimer act_id=%d; tmr_id=%d; ms=%u; cnt=%d;", a_id, timer_id, time_ms, count);
    auto res = StartTmr(a_id, timer_id, time_ms, count);
    m_cv_timers_wait.notify_all();
    return res;
}

bool CDispatcher::StartTimerOnce(const ActID_t a_id, const TimerID_t timer_id, const uint_32 time_ms)
{
    return StartTimer(a_id, timer_id, time_ms, 1);
}

bool CDispatcher::StartTimerPeriodical(const ActID_t a_id, const TimerID_t timer_id, const uint_32 time_ms)
{
    return StartTimer(a_id, timer_id, time_ms, -1);
}

bool CDispatcher::StopTimer(const ActID_t a_id, const TimerID_t timer_id)
{
    auto res = StopTmr(a_id, timer_id);
    m_cv_timers_wait.notify_all();
    return res;
}

bool CDispatcher::KillTimer(const ActID_t a_id, const TimerID_t timer_id)
{
    return DelTimer(a_id, timer_id);
}
