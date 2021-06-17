#ifndef CMESSAGESENDER_H
#define CMESSAGESENDER_H

#include <i_dispatcher.h>
#include "messages.h"

/**
 * @brief CMessageSender - класс, реализующий отправку сообщений через диспетчер
 */
class CMessageSender
{
public:
    CMessageSender(IDispatcherPtr disp, sDispatcherMsgStatistic* stat);
    virtual ~CMessageSender() = default;
    IDispatcherPtr Dispatcher() {return m_disp;}
private:
    template <typename T>
    bool SendMessage(const sActorID &sender, const sActorID &destination, const uint_16 msg_type, const T payload)
    {
        sMsg msg;
        msg.sender = sender;
        msg.destination = destination;
        msg.msg_type = msg_type;
        msg.msg.resize(sizeof(payload));
        memcpy(msg.msg.data(), &payload, sizeof(payload));
        m_disp->SendMessage(msg);
        return true;
    }

    template <typename T>
    bool SendMessage(const sActorID &destination, const uint_16 msg_type, const T payload)
    {
        return SendMessage(m_my_actor, destination, msg_type, payload);
    }

public:
    void SetSelfActor(const sActorID &self_act) {m_my_actor = self_act;}
    void SetSelfObjId(const ObjID_t my_obj_id) {m_my_obj_id = my_obj_id;}

    std::string PrintSenderStatistic() const;
    bool Send_SetVelocity(const sActorID &sender, const sActorID &destination, const sVect2 &velocity, const sVect2 &direction);
    bool Send_GameplayModeChange(const sActorID &destination, const sGameplayMode &gp_mode);
    bool Send_GlobalPhysicsParam(const sActorID &destination, const sGlobalPhysicsParam &ph_global);
    bool Send_ViewChange(const sActorID &destination, const sVect2 &center_move, const Real_t scale);
    bool Send_SetKinematicParam(const sActorID &destination, const ObjID_t obj_id, const sKinematicParam &kinematic_param);
    bool Send_SetPhysicsState(const sActorID &destination, const ObjID_t obj_id, const sPhysicsState &ph_state);
    bool Send_SetHealth(const sActorID &destination, const ObjID_t obj_id, const sHealthParam &health_param);
    bool Send_WindowMouseEvent(const sActorID &destination, const sMsgWindowMouseEvent::enWindowMouseEvent event, const sCoord2 &position);
    bool Send_WindowKeyboardEvent(const sActorID &destination, const int key, const sMsgWindowKeyboardEvent::enWindowKeyboardEvent event, const sCoord2 &position,
                                  const bool shift_pressed, const bool control_pressed, const bool alt_pressed);
    bool Send_WindowClose(const sActorID &destination, const bool close);
    bool Send_RequestPhysicsParamAll(const sActorID &destination, const bool kinematic);
    bool Send_NewObjectPhysics(const sActorID &destination, const ObjID_t obj_id, const sObjSpec &spec, const sPhysicsParam &ph_param);
    bool Send_NewObjectBehavior(const sActorID &destination, const ObjID_t obj_id, const sObjSpec &spec, const sBehaviorParam &bh_param, const sPhysicsParam &ph_param);
    bool Send_NewObjectShow(const sActorID &destination, const ObjID_t obj_id, const sObjSpec &spec, const sShowParam &sh_param, const sPhysicsParam &ph_param);
    bool Send_DelObjectPhysics(const sActorID &destination, const ObjID_t obj_id);
    bool Send_DelObjectBehavior(const sActorID &destination, const ObjID_t obj_id);
    bool Send_DelObject(const sActorID &destination, const ObjID_t obj_id);
    bool Send_DelObjectShow(const sActorID &destination, const ObjID_t obj_id);
    bool Send_EndAnimations(const sActorID &destination, const ObjID_t obj_id);
    bool Send_SetPhysicsGoal(const sActorID &destination, const ObjID_t obj_id, const sPhysicsGoal &goal);
    bool Send_SetPhysicsParam(const sActorID &destination, const ObjID_t obj_id, const sPhysicsParam &ph_param);
    bool Send_GameActionAttack(const sActorID &destination, const ObjID_t obj_id, const sCoord2 &obj_pos, const sCoord2 &target_pos);
    bool Send_CreateProjectile(const sActorID &destination, const sCoord2 &pos, const sVect2 &vel);
    bool Send_StatisticGp(const sActorID &destination, const sCoreStatisticGp &gp_stat);
    bool Send_StatisticPh(const sActorID &destination, const sCoreStatisticPh &ph_stat);
    bool Send_StatisticSh(const sActorID &destination, const sCoreStatisticSh &sh_stat);
    bool Send_StatisticBh(const sActorID &destination, const sCoreStatisticBh &bh_stat);
    bool Send_CoreShowParam(const sActorID &destination, const sCoreShowParam param);
    bool Send_GoalEvent(const sActorID &destination, const ObjID_t obj_id, const sPhysicsGoal &ph_goal);
    bool Send_SoundEvent(const sActorID &destination, const sSoundParam &sound_param);
    bool Send_ObjectAction(const sActorID &destination, const sObjectAction &obj_act);
    bool Send_GuiControlAddButton(const sActorID &destination, const GuiControlId_t id, const sGuiButtonParam &param);
    bool Send_GuiControlDel(const sActorID &destination, const GuiControlId_t id);
    bool Send_GuiControlEvent(const sActorID &destination, const GuiControlId_t id, const enGuiControlEvent event);

private:
    void StatMsgSent(const enMessageType msg_type);

    sDispatcherMsgStatistic*    m_msg_stat;
    IDispatcherPtr              m_disp;
    sActorID                    m_my_actor;
    ObjID_t                     m_my_obj_id;
};

#endif // CMESSAGESENDER_H
