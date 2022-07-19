//
// Generated file, do not edit! Created by opp_msgtool 6.0 from applicationpkts/downstreampkt.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "downstreampkt_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

Register_Class(DownstreamPkt)

DownstreamPkt::DownstreamPkt(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

DownstreamPkt::DownstreamPkt(const DownstreamPkt& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

DownstreamPkt::~DownstreamPkt()
{
}

DownstreamPkt& DownstreamPkt::operator=(const DownstreamPkt& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void DownstreamPkt::copy(const DownstreamPkt& other)
{
    this->upstreamSendTime = other.upstreamSendTime;
    this->sendTime = other.sendTime;
    this->delegateBR = other.delegateBR;
    this->mobileNode = other.mobileNode;
    this->seqNum = other.seqNum;
}

void DownstreamPkt::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->upstreamSendTime);
    doParsimPacking(b,this->sendTime);
    doParsimPacking(b,this->delegateBR);
    doParsimPacking(b,this->mobileNode);
    doParsimPacking(b,this->seqNum);
}

void DownstreamPkt::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->upstreamSendTime);
    doParsimUnpacking(b,this->sendTime);
    doParsimUnpacking(b,this->delegateBR);
    doParsimUnpacking(b,this->mobileNode);
    doParsimUnpacking(b,this->seqNum);
}

omnetpp::simtime_t DownstreamPkt::getUpstreamSendTime() const
{
    return this->upstreamSendTime;
}

void DownstreamPkt::setUpstreamSendTime(omnetpp::simtime_t upstreamSendTime)
{
    this->upstreamSendTime = upstreamSendTime;
}

omnetpp::simtime_t DownstreamPkt::getSendTime() const
{
    return this->sendTime;
}

void DownstreamPkt::setSendTime(omnetpp::simtime_t sendTime)
{
    this->sendTime = sendTime;
}

int DownstreamPkt::getDelegateBR() const
{
    return this->delegateBR;
}

void DownstreamPkt::setDelegateBR(int delegateBR)
{
    this->delegateBR = delegateBR;
}

int DownstreamPkt::getMobileNode() const
{
    return this->mobileNode;
}

void DownstreamPkt::setMobileNode(int mobileNode)
{
    this->mobileNode = mobileNode;
}

int DownstreamPkt::getSeqNum() const
{
    return this->seqNum;
}

void DownstreamPkt::setSeqNum(int seqNum)
{
    this->seqNum = seqNum;
}

class DownstreamPktDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_upstreamSendTime,
        FIELD_sendTime,
        FIELD_delegateBR,
        FIELD_mobileNode,
        FIELD_seqNum,
    };
  public:
    DownstreamPktDescriptor();
    virtual ~DownstreamPktDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(DownstreamPktDescriptor)

DownstreamPktDescriptor::DownstreamPktDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(DownstreamPkt)), "omnetpp::cPacket")
{
    propertyNames = nullptr;
}

DownstreamPktDescriptor::~DownstreamPktDescriptor()
{
    delete[] propertyNames;
}

bool DownstreamPktDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<DownstreamPkt *>(obj)!=nullptr;
}

const char **DownstreamPktDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *DownstreamPktDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int DownstreamPktDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 5+base->getFieldCount() : 5;
}

unsigned int DownstreamPktDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_upstreamSendTime
        FD_ISEDITABLE,    // FIELD_sendTime
        FD_ISEDITABLE,    // FIELD_delegateBR
        FD_ISEDITABLE,    // FIELD_mobileNode
        FD_ISEDITABLE,    // FIELD_seqNum
    };
    return (field >= 0 && field < 5) ? fieldTypeFlags[field] : 0;
}

const char *DownstreamPktDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "upstreamSendTime",
        "sendTime",
        "delegateBR",
        "mobileNode",
        "seqNum",
    };
    return (field >= 0 && field < 5) ? fieldNames[field] : nullptr;
}

int DownstreamPktDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "upstreamSendTime") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "sendTime") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "delegateBR") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "mobileNode") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "seqNum") == 0) return baseIndex + 4;
    return base ? base->findField(fieldName) : -1;
}

const char *DownstreamPktDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "omnetpp::simtime_t",    // FIELD_upstreamSendTime
        "omnetpp::simtime_t",    // FIELD_sendTime
        "int",    // FIELD_delegateBR
        "int",    // FIELD_mobileNode
        "int",    // FIELD_seqNum
    };
    return (field >= 0 && field < 5) ? fieldTypeStrings[field] : nullptr;
}

const char **DownstreamPktDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *DownstreamPktDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int DownstreamPktDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    DownstreamPkt *pp = omnetpp::fromAnyPtr<DownstreamPkt>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void DownstreamPktDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    DownstreamPkt *pp = omnetpp::fromAnyPtr<DownstreamPkt>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'DownstreamPkt'", field);
    }
}

const char *DownstreamPktDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    DownstreamPkt *pp = omnetpp::fromAnyPtr<DownstreamPkt>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string DownstreamPktDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    DownstreamPkt *pp = omnetpp::fromAnyPtr<DownstreamPkt>(object); (void)pp;
    switch (field) {
        case FIELD_upstreamSendTime: return simtime2string(pp->getUpstreamSendTime());
        case FIELD_sendTime: return simtime2string(pp->getSendTime());
        case FIELD_delegateBR: return long2string(pp->getDelegateBR());
        case FIELD_mobileNode: return long2string(pp->getMobileNode());
        case FIELD_seqNum: return long2string(pp->getSeqNum());
        default: return "";
    }
}

void DownstreamPktDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    DownstreamPkt *pp = omnetpp::fromAnyPtr<DownstreamPkt>(object); (void)pp;
    switch (field) {
        case FIELD_upstreamSendTime: pp->setUpstreamSendTime(string2simtime(value)); break;
        case FIELD_sendTime: pp->setSendTime(string2simtime(value)); break;
        case FIELD_delegateBR: pp->setDelegateBR(string2long(value)); break;
        case FIELD_mobileNode: pp->setMobileNode(string2long(value)); break;
        case FIELD_seqNum: pp->setSeqNum(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'DownstreamPkt'", field);
    }
}

omnetpp::cValue DownstreamPktDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    DownstreamPkt *pp = omnetpp::fromAnyPtr<DownstreamPkt>(object); (void)pp;
    switch (field) {
        case FIELD_upstreamSendTime: return pp->getUpstreamSendTime().dbl();
        case FIELD_sendTime: return pp->getSendTime().dbl();
        case FIELD_delegateBR: return pp->getDelegateBR();
        case FIELD_mobileNode: return pp->getMobileNode();
        case FIELD_seqNum: return pp->getSeqNum();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'DownstreamPkt' as cValue -- field index out of range?", field);
    }
}

void DownstreamPktDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    DownstreamPkt *pp = omnetpp::fromAnyPtr<DownstreamPkt>(object); (void)pp;
    switch (field) {
        case FIELD_upstreamSendTime: pp->setUpstreamSendTime(value.doubleValue()); break;
        case FIELD_sendTime: pp->setSendTime(value.doubleValue()); break;
        case FIELD_delegateBR: pp->setDelegateBR(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_mobileNode: pp->setMobileNode(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_seqNum: pp->setSeqNum(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'DownstreamPkt'", field);
    }
}

const char *DownstreamPktDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr DownstreamPktDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    DownstreamPkt *pp = omnetpp::fromAnyPtr<DownstreamPkt>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void DownstreamPktDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    DownstreamPkt *pp = omnetpp::fromAnyPtr<DownstreamPkt>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'DownstreamPkt'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp

