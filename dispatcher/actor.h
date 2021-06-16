#ifndef ACTOR_H
#define ACTOR_H
#include "i_actor.h"

class CActor
        : public IActor
{
public:
    CActor(const std::string &act_name) {m_name = act_name;}
    virtual ~CActor() override = default;
    virtual void SetMessage(sMsgPtr msg) override final {m_message = msg;}
    virtual sActorID& GetActorID() override final {return m_id;}
    virtual const std::string& GetActorName() override final {return m_name;}
    sDispatcherMsgStatistic& GetMessagesStatistic() {return m_msg_stat;}

protected:
    sActorID                m_id;
    std::string             m_name;
    sMsgPtr                 m_message;
    sDispatcherMsgStatistic m_msg_stat;
};

#endif // ACTOR_H
