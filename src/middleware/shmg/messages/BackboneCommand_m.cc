//
// Generated file, do not edit! Created by opp_msgtool 6.0 from middleware/shmg/messages/BackboneCommand.msg.
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
#include "BackboneCommand_m.h"

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

Register_Class(BackboneCommand)

BackboneCommand::BackboneCommand(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

BackboneCommand::BackboneCommand(const BackboneCommand& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

BackboneCommand::~BackboneCommand()
{
}

BackboneCommand& BackboneCommand::operator=(const BackboneCommand& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void BackboneCommand::copy(const BackboneCommand& other)
{
    this->srcEncapsulatedPkt = other.srcEncapsulatedPkt;
    this->destEncapsulatedPkt = other.destEncapsulatedPkt;
    this->seqNum = other.seqNum;
    this->useControlCellInTSCH = other.useControlCellInTSCH;
}

void BackboneCommand::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->srcEncapsulatedPkt);
    doParsimPacking(b,this->destEncapsulatedPkt);
    doParsimPacking(b,this->seqNum);
    doParsimPacking(b,this->useControlCellInTSCH);
}

void BackboneCommand::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->srcEncapsulatedPkt);
    doParsimUnpacking(b,this->destEncapsulatedPkt);
    doParsimUnpacking(b,this->seqNum);
    doParsimUnpacking(b,this->useControlCellInTSCH);
}

int BackboneCommand::getSrcEncapsulatedPkt() const
{
    return this->srcEncapsulatedPkt;
}

void BackboneCommand::setSrcEncapsulatedPkt(int srcEncapsulatedPkt)
{
    this->srcEncapsulatedPkt = srcEncapsulatedPkt;
}

int BackboneCommand::getDestEncapsulatedPkt() const
{
    return this->destEncapsulatedPkt;
}

void BackboneCommand::setDestEncapsulatedPkt(int destEncapsulatedPkt)
{
    this->destEncapsulatedPkt = destEncapsulatedPkt;
}

int BackboneCommand::getSeqNum() const
{
    return this->seqNum;
}

void BackboneCommand::setSeqNum(int seqNum)
{
    this->seqNum = seqNum;
}

bool BackboneCommand::getUseControlCellInTSCH() const
{
    return this->useControlCellInTSCH;
}

void BackboneCommand::setUseControlCellInTSCH(bool useControlCellInTSCH)
{
    this->useControlCellInTSCH = useControlCellInTSCH;
}

class BackboneCommandDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_srcEncapsulatedPkt,
        FIELD_destEncapsulatedPkt,
        FIELD_seqNum,
        FIELD_useControlCellInTSCH,
    };
  public:
    BackboneCommandDescriptor();
    virtual ~BackboneCommandDescriptor();

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

Register_ClassDescriptor(BackboneCommandDescriptor)

BackboneCommandDescriptor::BackboneCommandDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(BackboneCommand)), "omnetpp::cPacket")
{
    propertyNames = nullptr;
}

BackboneCommandDescriptor::~BackboneCommandDescriptor()
{
    delete[] propertyNames;
}

bool BackboneCommandDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<BackboneCommand *>(obj)!=nullptr;
}

const char **BackboneCommandDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *BackboneCommandDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int BackboneCommandDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 4+base->getFieldCount() : 4;
}

unsigned int BackboneCommandDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_srcEncapsulatedPkt
        FD_ISEDITABLE,    // FIELD_destEncapsulatedPkt
        FD_ISEDITABLE,    // FIELD_seqNum
        FD_ISEDITABLE,    // FIELD_useControlCellInTSCH
    };
    return (field >= 0 && field < 4) ? fieldTypeFlags[field] : 0;
}

const char *BackboneCommandDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "srcEncapsulatedPkt",
        "destEncapsulatedPkt",
        "seqNum",
        "useControlCellInTSCH",
    };
    return (field >= 0 && field < 4) ? fieldNames[field] : nullptr;
}

int BackboneCommandDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "srcEncapsulatedPkt") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "destEncapsulatedPkt") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "seqNum") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "useControlCellInTSCH") == 0) return baseIndex + 3;
    return base ? base->findField(fieldName) : -1;
}

const char *BackboneCommandDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_srcEncapsulatedPkt
        "int",    // FIELD_destEncapsulatedPkt
        "int",    // FIELD_seqNum
        "bool",    // FIELD_useControlCellInTSCH
    };
    return (field >= 0 && field < 4) ? fieldTypeStrings[field] : nullptr;
}

const char **BackboneCommandDescriptor::getFieldPropertyNames(int field) const
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

const char *BackboneCommandDescriptor::getFieldProperty(int field, const char *propertyName) const
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

int BackboneCommandDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    BackboneCommand *pp = omnetpp::fromAnyPtr<BackboneCommand>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void BackboneCommandDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    BackboneCommand *pp = omnetpp::fromAnyPtr<BackboneCommand>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'BackboneCommand'", field);
    }
}

const char *BackboneCommandDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    BackboneCommand *pp = omnetpp::fromAnyPtr<BackboneCommand>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string BackboneCommandDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    BackboneCommand *pp = omnetpp::fromAnyPtr<BackboneCommand>(object); (void)pp;
    switch (field) {
        case FIELD_srcEncapsulatedPkt: return long2string(pp->getSrcEncapsulatedPkt());
        case FIELD_destEncapsulatedPkt: return long2string(pp->getDestEncapsulatedPkt());
        case FIELD_seqNum: return long2string(pp->getSeqNum());
        case FIELD_useControlCellInTSCH: return bool2string(pp->getUseControlCellInTSCH());
        default: return "";
    }
}

void BackboneCommandDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    BackboneCommand *pp = omnetpp::fromAnyPtr<BackboneCommand>(object); (void)pp;
    switch (field) {
        case FIELD_srcEncapsulatedPkt: pp->setSrcEncapsulatedPkt(string2long(value)); break;
        case FIELD_destEncapsulatedPkt: pp->setDestEncapsulatedPkt(string2long(value)); break;
        case FIELD_seqNum: pp->setSeqNum(string2long(value)); break;
        case FIELD_useControlCellInTSCH: pp->setUseControlCellInTSCH(string2bool(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'BackboneCommand'", field);
    }
}

omnetpp::cValue BackboneCommandDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    BackboneCommand *pp = omnetpp::fromAnyPtr<BackboneCommand>(object); (void)pp;
    switch (field) {
        case FIELD_srcEncapsulatedPkt: return pp->getSrcEncapsulatedPkt();
        case FIELD_destEncapsulatedPkt: return pp->getDestEncapsulatedPkt();
        case FIELD_seqNum: return pp->getSeqNum();
        case FIELD_useControlCellInTSCH: return pp->getUseControlCellInTSCH();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'BackboneCommand' as cValue -- field index out of range?", field);
    }
}

void BackboneCommandDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    BackboneCommand *pp = omnetpp::fromAnyPtr<BackboneCommand>(object); (void)pp;
    switch (field) {
        case FIELD_srcEncapsulatedPkt: pp->setSrcEncapsulatedPkt(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_destEncapsulatedPkt: pp->setDestEncapsulatedPkt(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_seqNum: pp->setSeqNum(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_useControlCellInTSCH: pp->setUseControlCellInTSCH(value.boolValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'BackboneCommand'", field);
    }
}

const char *BackboneCommandDescriptor::getFieldStructName(int field) const
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

omnetpp::any_ptr BackboneCommandDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    BackboneCommand *pp = omnetpp::fromAnyPtr<BackboneCommand>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void BackboneCommandDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    BackboneCommand *pp = omnetpp::fromAnyPtr<BackboneCommand>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'BackboneCommand'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp

