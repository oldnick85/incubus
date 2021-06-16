#ifndef I_ACTOR_H
#define I_ACTOR_H

#include <stdint.h>
#include <vector>
#include <string>
#include <chrono>
#include <memory>
#include <unordered_map>
#include "dispatcher_common.h"

struct sMsg
{
    std::string print() const
    {
        char s[128];
        snprintf(s, sizeof(s), "[MSG%d:%s%s %u]",
                 id, sender.print().c_str(), destination.print().c_str(), msg_type);
        return s;
    }

    int_32              id = -1;
    sActorID            sender;
    sActorID            destination;
    uint_16             msg_type = 0;
    std::vector<uint_8> msg;
};
typedef std::shared_ptr<sMsg> sMsgPtr;

struct sDispatcherMsgStatistic
{
    uint GetTotalMsgSent() const {return msgs_sent;}
    void MsgSent(uint msg_type)
    {
        msgs_sent++;
        msg_sent[msg_type]++;
    }

    uint                                msgs_sent = 0;
    std::unordered_map<uint, uint>      msg_sent;
};

/**
 * @brief IActor - интерфейс класса актора для реализации модели акторов
 */
class IActor
{
public:
    virtual ~IActor() = default;
    /**
     * @brief onMessage - колбек получения сообщения
     */
    virtual void onMessage() = 0;
    /**
     * @brief onTimer - колбек срабатывания таймера
     * @param[in] timer_id - номер сработавшего таймера
     */
    virtual void onTimer(const TimerID_t timer_id) = 0;
    /**
     * @brief onStart - колбек, вызываемый первым после создания актора
     */
    virtual void onStart() = 0;
    /**
     * @brief onStop - колбек, вызываемый последним перед удалением актора
     */
    virtual void onStop() = 0;
    /**
     * @brief SetMessage - установить последний полученный пакет
     * @param[in] msg - пакет с сообщением
     */
    virtual void SetMessage(sMsgPtr msg) = 0;
    /**
     * @brief GetActorID - получить идентификатор актора
     * @return идентификатор актора
     */
    virtual sActorID& GetActorID() = 0;
    /**
     * @brief GetActorName - получить имя актора
     * @return строка с именем актора
     */
    virtual const std::string& GetActorName() = 0;
};

#endif // I_ACTOR_H
