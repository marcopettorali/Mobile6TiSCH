//
// Generated file, do not edit! Created by opp_msgtool 6.0 from physicallayer/radio/base/SignalDescriptor.msg.
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
#include "SignalDescriptor_m.h"

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

Register_Class(SignalDescriptor)

SignalDescriptor::SignalDescriptor(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

SignalDescriptor::SignalDescriptor(const SignalDescriptor& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

SignalDescriptor::~SignalDescriptor()
{
}

SignalDescriptor& SignalDescriptor::operator=(const SignalDescriptor& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void SignalDescriptor::copy(const SignalDescriptor& other)
{
    this->positionX = other.positionX;
    this->positionY = other.positionY;
    this->carrierFrequency = other.carrierFrequency;
    this->bandwidth = other.bandwidth;
    this->transmitterPowerMW = other.transmitterPowerMW;
    this->transmitterGainMW = other.transmitterGainMW;
    this->rssiMW = other.rssiMW;
    this->idSrc = other.idSrc;
    this->idSeq = other.idSeq;
}

void SignalDescriptor::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->positionX);
    doParsimPacking(b,this->positionY);
    doParsimPacking(b,this->carrierFrequency);
    doParsimPacking(b,this->bandwidth);
    doParsimPacking(b,this->transmitterPowerMW);
    doParsimPacking(b,this->transmitterGainMW);
    doParsimPacking(b,this->rssiMW);
    doParsimPacking(b,this->idSrc);
    doParsimPacking(b,this->idSeq);
}

void SignalDescriptor::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->positionX);
    doParsimUnpacking(b,this->positionY);
    doParsimUnpacking(b,this->carrierFrequency);
    doParsimUnpacking(b,this->bandwidth);
    doParsimUnpacking(b,this->transmitterPowerMW);
    doParsimUnpacking(b,this->transmitterGainMW);
    doParsimUnpacking(b,this->rssiMW);
    doParsimUnpacking(b,this->idSrc);
    doParsimUnpacking(b,this->idSeq);
}

double SignalDescriptor::getPositionX() const
{
    return this->positionX;
}

void SignalDescriptor::setPositionX(double positionX)
{
    this->positionX = positionX;
}

double SignalDescriptor::getPositionY() const
{
    return this->positionY;
}

void SignalDescriptor::setPositionY(double positionY)
{
    this->positionY = positionY;
}

double SignalDescriptor::getCarrierFrequency() const
{
    return this->carrierFrequency;
}

void SignalDescriptor::setCarrierFrequency(double carrierFrequency)
{
    this->carrierFrequency = carrierFrequency;
}

double SignalDescriptor::getBandwidth() const
{
    return this->bandwidth;
}

void SignalDescriptor::setBandwidth(double bandwidth)
{
    this->bandwidth = bandwidth;
}

double SignalDescriptor::getTransmitterPowerMW() const
{
    return this->transmitterPowerMW;
}

void SignalDescriptor::setTransmitterPowerMW(double transmitterPowerMW)
{
    this->transmitterPowerMW = transmitterPowerMW;
}

double SignalDescriptor::getTransmitterGainMW() const
{
    return this->transmitterGainMW;
}

void SignalDescriptor::setTransmitterGainMW(double transmitterGainMW)
{
    this->transmitterGainMW = transmitterGainMW;
}

double SignalDescriptor::getRssiMW() const
{
    return this->rssiMW;
}

void SignalDescriptor::setRssiMW(double rssiMW)
{
    this->rssiMW = rssiMW;
}

int SignalDescriptor::getIdSrc() const
{
    return this->idSrc;
}

void SignalDescriptor::setIdSrc(int idSrc)
{
    this->idSrc = idSrc;
}

int SignalDescriptor::getIdSeq() const
{
    return this->idSeq;
}

void SignalDescriptor::setIdSeq(int idSeq)
{
    this->idSeq = idSeq;
}

class SignalDescriptorDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_positionX,
        FIELD_positionY,
        FIELD_carrierFrequency,
        FIELD_bandwidth,
        FIELD_transmitterPowerMW,
        FIELD_transmitterGainMW,
        FIELD_rssiMW,
        FIELD_idSrc,
        FIELD_idSeq,
    };
  public:
    SignalDescriptorDescriptor();
    virtual ~SignalDescriptorDescriptor();

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

Register_ClassDescriptor(SignalDescriptorDescriptor)

SignalDescriptorDescriptor::SignalDescriptorDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(SignalDescriptor)), "omnetpp::cPacket")
{
    propertyNames = nullptr;
}

SignalDescriptorDescriptor::~SignalDescriptorDescriptor()
{
    delete[] propertyNames;
}

bool SignalDescriptorDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<SignalDescriptor *>(obj)!=nullptr;
}

const char **SignalDescriptorDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *SignalDescriptorDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int SignalDescriptorDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 9+base->getFieldCount() : 9;
}

unsigned int SignalDescriptorDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_positionX
        FD_ISEDITABLE,    // FIELD_positionY
        FD_ISEDITABLE,    // FIELD_carrierFrequency
        FD_ISEDITABLE,    // FIELD_bandwidth
        FD_ISEDITABLE,    // FIELD_transmitterPowerMW
        FD_ISEDITABLE,    // FIELD_transmitterGainMW
        FD_ISEDITABLE,    // FIELD_rssiMW
        FD_ISEDITABLE,    // FIELD_idSrc
        FD_ISEDITABLE,    // FIELD_idSeq
    };
    return (field >= 0 && field < 9) ? fieldTypeFlags[field] : 0;
}

const char *SignalDescriptorDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "positionX",
        "positionY",
        "carrierFrequency",
        "bandwidth",
        "transmitterPowerMW",
        "transmitterGainMW",
        "rssiMW",
        "idSrc",
        "idSeq",
    };
    return (field >= 0 && field < 9) ? fieldNames[field] : nullptr;
}

int SignalDescriptorDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "positionX") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "positionY") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "carrierFrequency") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "bandwidth") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "transmitterPowerMW") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "transmitterGainMW") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "rssiMW") == 0) return baseIndex + 6;
    if (strcmp(fieldName, "idSrc") == 0) return baseIndex + 7;
    if (strcmp(fieldName, "idSeq") == 0) return baseIndex + 8;
    return base ? base->findField(fieldName) : -1;
}

const char *SignalDescriptorDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "double",    // FIELD_positionX
        "double",    // FIELD_positionY
        "double",    // FIELD_carrierFrequency
        "double",    // FIELD_bandwidth
        "double",    // FIELD_transmitterPowerMW
        "double",    // FIELD_transmitterGainMW
        "double",    // FIELD_rssiMW
        "int",    // FIELD_idSrc
        "int",    // FIELD_idSeq
    };
    return (field >= 0 && field < 9) ? fieldTypeStrings[field] : nullptr;
}

const char **SignalDescriptorDescriptor::getFieldPropertyNames(int field) const
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

const char *SignalDescriptorDescriptor::getFieldProperty(int field, const char *propertyName) const
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

int SignalDescriptorDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    SignalDescriptor *pp = omnetpp::fromAnyPtr<SignalDescriptor>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void SignalDescriptorDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    SignalDescriptor *pp = omnetpp::fromAnyPtr<SignalDescriptor>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'SignalDescriptor'", field);
    }
}

const char *SignalDescriptorDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    SignalDescriptor *pp = omnetpp::fromAnyPtr<SignalDescriptor>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string SignalDescriptorDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    SignalDescriptor *pp = omnetpp::fromAnyPtr<SignalDescriptor>(object); (void)pp;
    switch (field) {
        case FIELD_positionX: return double2string(pp->getPositionX());
        case FIELD_positionY: return double2string(pp->getPositionY());
        case FIELD_carrierFrequency: return double2string(pp->getCarrierFrequency());
        case FIELD_bandwidth: return double2string(pp->getBandwidth());
        case FIELD_transmitterPowerMW: return double2string(pp->getTransmitterPowerMW());
        case FIELD_transmitterGainMW: return double2string(pp->getTransmitterGainMW());
        case FIELD_rssiMW: return double2string(pp->getRssiMW());
        case FIELD_idSrc: return long2string(pp->getIdSrc());
        case FIELD_idSeq: return long2string(pp->getIdSeq());
        default: return "";
    }
}

void SignalDescriptorDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    SignalDescriptor *pp = omnetpp::fromAnyPtr<SignalDescriptor>(object); (void)pp;
    switch (field) {
        case FIELD_positionX: pp->setPositionX(string2double(value)); break;
        case FIELD_positionY: pp->setPositionY(string2double(value)); break;
        case FIELD_carrierFrequency: pp->setCarrierFrequency(string2double(value)); break;
        case FIELD_bandwidth: pp->setBandwidth(string2double(value)); break;
        case FIELD_transmitterPowerMW: pp->setTransmitterPowerMW(string2double(value)); break;
        case FIELD_transmitterGainMW: pp->setTransmitterGainMW(string2double(value)); break;
        case FIELD_rssiMW: pp->setRssiMW(string2double(value)); break;
        case FIELD_idSrc: pp->setIdSrc(string2long(value)); break;
        case FIELD_idSeq: pp->setIdSeq(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'SignalDescriptor'", field);
    }
}

omnetpp::cValue SignalDescriptorDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    SignalDescriptor *pp = omnetpp::fromAnyPtr<SignalDescriptor>(object); (void)pp;
    switch (field) {
        case FIELD_positionX: return pp->getPositionX();
        case FIELD_positionY: return pp->getPositionY();
        case FIELD_carrierFrequency: return pp->getCarrierFrequency();
        case FIELD_bandwidth: return pp->getBandwidth();
        case FIELD_transmitterPowerMW: return pp->getTransmitterPowerMW();
        case FIELD_transmitterGainMW: return pp->getTransmitterGainMW();
        case FIELD_rssiMW: return pp->getRssiMW();
        case FIELD_idSrc: return pp->getIdSrc();
        case FIELD_idSeq: return pp->getIdSeq();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'SignalDescriptor' as cValue -- field index out of range?", field);
    }
}

void SignalDescriptorDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    SignalDescriptor *pp = omnetpp::fromAnyPtr<SignalDescriptor>(object); (void)pp;
    switch (field) {
        case FIELD_positionX: pp->setPositionX(value.doubleValue()); break;
        case FIELD_positionY: pp->setPositionY(value.doubleValue()); break;
        case FIELD_carrierFrequency: pp->setCarrierFrequency(value.doubleValue()); break;
        case FIELD_bandwidth: pp->setBandwidth(value.doubleValue()); break;
        case FIELD_transmitterPowerMW: pp->setTransmitterPowerMW(value.doubleValue()); break;
        case FIELD_transmitterGainMW: pp->setTransmitterGainMW(value.doubleValue()); break;
        case FIELD_rssiMW: pp->setRssiMW(value.doubleValue()); break;
        case FIELD_idSrc: pp->setIdSrc(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_idSeq: pp->setIdSeq(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'SignalDescriptor'", field);
    }
}

const char *SignalDescriptorDescriptor::getFieldStructName(int field) const
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

omnetpp::any_ptr SignalDescriptorDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    SignalDescriptor *pp = omnetpp::fromAnyPtr<SignalDescriptor>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void SignalDescriptorDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    SignalDescriptor *pp = omnetpp::fromAnyPtr<SignalDescriptor>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'SignalDescriptor'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp

