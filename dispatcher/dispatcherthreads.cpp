#include "dispatcherthreads.h"

CDispatcherThreads::CDispatcherThreads()
{
    m_threads_count.store(0);
}

CDispatcherThreadPtr CDispatcherThreads::FindThread(const DispatcherThreadID_t thr_id)
{
    std::lock_guard<std::mutex> locker(m_mtx_threads);
    auto thr_it = m_threads.find(thr_id);
    if (thr_it == m_threads.end()) return nullptr;
    return thr_it->second;
}

CDispatcherThreadPtr CDispatcherThreads::FindThreadActor(const ActID_t act_id)
{
    for (auto &thr_it : m_threads)
    {
        auto thr = thr_it.second;
        if (thr->GetActorId() == act_id)
            return thr;
    }
    return nullptr;
}

CDispatcherThreadPtr CDispatcherThreads::AddThread()
{
    std::lock_guard<std::mutex> locker(m_mtx_threads);
    m_max_thread_id++;
    CDispatcherThreadPtr t(new CDispatcherThread(m_max_thread_id));
    m_threads[m_max_thread_id] = t;
    return t;
}

bool CDispatcherThreads::DelThread(const DispatcherThreadID_t thr_id)
{
    std::lock_guard<std::mutex> locker(m_mtx_threads);
    auto thr_it = m_threads.find(thr_id);
    if (thr_it == m_threads.end()) return false;
    m_threads.erase(thr_it);
    return true;
}

void CDispatcherThreads::RunAllWorkers()
{
    for (auto t_el : m_threads)
        t_el.second->Run();
}

void CDispatcherThreads::StopAllWorkers()
{
    for (auto t_el : m_threads)
        t_el.second->Stop();
}
