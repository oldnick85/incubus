#ifndef OBJECT_H
#define OBJECT_H
#include "i_object.h"
#include "common_obj.h"

class CObject
        : public IObject
{
public:
    CObject(const sObjSpec &spec);
    virtual ~CObject() override = default;

    virtual sObjSpec& ObjSpec() override final {return m_spec;}
    virtual ObjID_t GetID() const override final {return m_id;}
    virtual void SetID(const ObjID_t &id) override final {m_id = id;}
private:
    sObjSpec    m_spec;
    ObjID_t     m_id = OBJ_ID_NONE;
};

#endif // OBJECT_H
