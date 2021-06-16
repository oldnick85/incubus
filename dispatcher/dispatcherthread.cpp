#include "dispatcherthread.h"

#include <thread>

CDispatcherThread::CDispatcherThread(const DispatcherThreadID_t i)
    : id(i)
{
    m_should_stop.store(true);
}

std::string CDispatcherThread::print() const
{
    char s[128];
    snprintf(s, sizeof(s), "id=%u;", id);
    return s;
}

bool CDispatcherThread::Run()
{
    m_should_stop.store(false);
    std::thread working_thread(WorkingThread, this);
    working_thread.detach();
    return true;
}

bool CDispatcherThread::Stop()
{
    m_should_stop.store(true);
    return true;
}

void CDispatcherThread::SetActor(CDispatcherActorPtr actor)
{
    std::lock_guard<std::mutex> locker(m_mtx_actor);
    m_actor = actor;
}

void CDispatcherThread::WorkingThread(CDispatcherThread* thr)
{
    while(not thr->m_should_stop.load())
    {
        thr->m_mtx_actor.lock();
        if (not thr->m_actor) thr->WaitForWork();
        bool has_work = false;

        if (thr->m_actor->IsNew())
        {
            thr->m_actor->SetNormal();
            thr->m_actor->Actor()->onStart();
            has_work = true;
        }

        auto tm_id = thr->m_actor->PopTimerToWork();
        if (tm_id != TIMER_ID_NONE)
        {
            thr->m_actor->Actor()->onTimer(tm_id);
            has_work = true;
        }
        else
        {
            auto msg = thr->m_actor->PopMsg();
            if (msg != nullptr)
            {
                thr->m_actor->Actor()->SetMessage(msg);
                thr->m_actor->Actor()->onMessage();
                has_work = true;
            }
        }

        if (not has_work) thr->WaitForWork();

        thr->m_mtx_actor.unlock();
    }
}

void CDispatcherThread::WakeUpForWork()
{
    m_cv_wait_for_work.notify_all();
}

void CDispatcherThread::WaitForWork()
{
    m_mtx_actor.unlock();
    std::unique_lock<std::mutex> work_locker(m_mtx_wait_for_work);
    m_cv_wait_for_work.wait(work_locker);
    m_mtx_actor.lock();
}
