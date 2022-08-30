//
// Generated file, do not edit! Created by opp_msgtool 6.0 from shmg/backboneprotocol/BackbonePkt.msg.
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
#include "BackbonePkt_m.h"

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

Register_Class(BackbonePkt)

BackbonePkt::BackbonePkt(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

BackbonePkt::BackbonePkt(const BackbonePkt& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

BackbonePkt::~BackbonePkt()
{
}

BackbonePkt& BackbonePkt::operator=(const BackbonePkt& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void BackbonePkt::copy(const BackbonePkt& other)
{
    this->mobileNode = other.mobileNode;
    this->borderRouter = other.borderRouter;
    this->seq = other.seq;
    this->rssi = other.rssi;
    this->outPort = other.outPort;
}

void BackbonePkt::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->mobileNode);
    doParsimPacking(b,this->borderRouter);
    doParsimPacking(b,this->seq);
    doParsimPacking(b,this->rssi);
    doParsimPacking(b,this->outPort);
}

void BackbonePkt::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->mobileNode);
    doParsimUnpacking(b,this->borderRouter);
    doParsimUnpacking(b,this->seq);
    doParsimUnpacking(b,this->rssi);
    doParsimUnpacking(b,this->outPort);
}

int BackbonePkt::getMobileNode() const
{
    return this->mobileNode;
}

void BackbonePkt::setMobileNode(int mobileNode)
{
    this->mobileNode = mobileNode;
}

int BackbonePkt::getBorderRouter() const
{
    return this->borderRouter;
}

void BackbonePkt::setBorderRouter(int borderRouter)
{
    this->borderRouter = borderRouter;
}

int BackbonePkt::getSeq() const
{
    return this->seq;
}

void BackbonePkt::setSeq(int seq)
{
    this->seq = seq;
}

double BackbonePkt::getRssi() const
{
    return this->rssi;
}

void BackbonePkt::setRssi(double rssi)
{
    this->rssi = rssi;
}

int BackbonePkt::getOutPort() const
{
    return this->outPort;
}

void BackbonePkt::setOutPort(int outPort)
{
    this->outPort = outPort;
}

class BackbonePktDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_mobileNode,
        FIELD_borderRouter,
        FIELD_seq,
        FIELD_rssi,
        FIELD_outPort,
    };
  public:
    BackbonePktDescriptor();
    virtual ~BackbonePktDescriptor();

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

Register_ClassDescriptor(BackbonePktDescriptor)

BackbonePktDescriptor::BackbonePktDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(BackbonePkt)), "omnetpp::cPacket")
{
    propertyNames = nullptr;
}

BackbonePktDescriptor::~BackbonePktDescriptor()
{
    delete[] propertyNames;
}

bool BackbonePktDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<BackbonePkt *>(obj)!=nullptr;
}

const char **BackbonePktDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *BackbonePktDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int BackbonePktDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 5+base->getFieldCount() : 5;
}

unsigned int BackbonePktDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_mobileNode
        FD_ISEDITABLE,    // FIELD_borderRouter
        FD_ISEDITABLE,    // FIELD_seq
        FD_ISEDITABLE,    // FIELD_rssi
        FD_ISEDITABLE,    // FIELD_outPort
    };
    return (field >= 0 && field < 5) ? fieldTypeFlags[field] : 0;
}

const char *BackbonePktDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "mobileNode",
        "borderRouter",
        "seq",
        "rssi",
        "outPort",
    };
    return (field >= 0 && field < 5) ? fieldNames[field] : nullptr;
}

int BackbonePktDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "mobileNode") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "borderRouter") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "seq") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "rssi") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "outPort") == 0) return baseIndex + 4;
    return base ? base->findField(fieldName) : -1;
}

const char *BackbonePktDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_mobileNode
        "int",    // FIELD_borderRouter
        "int",    // FIELD_seq
        "double",    // FIELD_rssi
        "int",    // FIELD_outPort
    };
    return (field >= 0 && field < 5) ? fieldTypeStrings[field] : nullptr;
}

const char **BackbonePktDescriptor::getFieldPropertyNames(int field) const
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

const char *BackbonePktDescriptor::getFieldProperty(int field, const char *propertyName) const
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

int BackbonePktDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    BackbonePkt *pp = omnetpp::fromAnyPtr<BackbonePkt>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void BackbonePktDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    BackbonePkt *pp = omnetpp::fromAnyPtr<BackbonePkt>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'BackbonePkt'", field);
    }
}

const char *BackbonePktDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    BackbonePkt *pp = omnetpp::fromAnyPtr<BackbonePkt>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string BackbonePktDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    BackbonePkt *pp = omnetpp::fromAnyPtr<BackbonePkt>(object); (void)pp;
    switch (field) {
        case FIELD_mobileNode: return long2string(pp->getMobileNode());
        case FIELD_borderRouter: return long2string(pp->getBorderRouter());
        case FIELD_seq: return long2string(pp->getSeq());
        case FIELD_rssi: return double2string(pp->getRssi());
        case FIELD_outPort: return long2string(pp->getOutPort());
        default: return "";
    }
}

void BackbonePktDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    BackbonePkt *pp = omnetpp::fromAnyPtr<BackbonePkt>(object); (void)pp;
    switch (field) {
        case FIELD_mobileNode: pp->setMobileNode(string2long(value)); break;
        case FIELD_borderRouter: pp->setBorderRouter(string2long(value)); break;
        case FIELD_seq: pp->setSeq(string2long(value)); break;
        case FIELD_rssi: pp->setRssi(string2double(value)); break;
        case FIELD_outPort: pp->setOutPort(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'BackbonePkt'", field);
    }
}

omnetpp::cValue BackbonePktDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    BackbonePkt *pp = omnetpp::fromAnyPtr<BackbonePkt>(object); (void)pp;
    switch (field) {
        case FIELD_mobileNode: return pp->getMobileNode();
        case FIELD_borderRouter: return pp->getBorderRouter();
        case FIELD_seq: return pp->getSeq();
        case FIELD_rssi: return pp->getRssi();
        case FIELD_outPort: return pp->getOutPort();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'BackbonePkt' as cValue -- field index out of range?", field);
    }
}

void BackbonePktDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    BackbonePkt *pp = omnetpp::fromAnyPtr<BackbonePkt>(object); (void)pp;
    switch (field) {
        case FIELD_mobileNode: pp->setMobileNode(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_borderRouter: pp->setBorderRouter(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_seq: pp->setSeq(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_rssi: pp->setRssi(value.doubleValue()); break;
        case FIELD_outPort: pp->setOutPort(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'BackbonePkt'", field);
    }
}

const char *BackbonePktDescriptor::getFieldStructName(int field) const
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

omnetpp::any_ptr BackbonePktDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    BackbonePkt *pp = omnetpp::fromAnyPtr<BackbonePkt>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void BackbonePktDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    BackbonePkt *pp = omnetpp::fromAnyPtr<BackbonePkt>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'BackbonePkt'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp

