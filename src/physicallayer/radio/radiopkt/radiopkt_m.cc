//
// Generated file, do not edit! Created by opp_msgtool 6.0 from physicallayer/radio/radiopkt/radiopkt.msg.
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
#include "radiopkt_m.h"

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

Register_Class(RadioPkt)

RadioPkt::RadioPkt(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

RadioPkt::RadioPkt(const RadioPkt& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

RadioPkt::~RadioPkt()
{
}

RadioPkt& RadioPkt::operator=(const RadioPkt& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void RadioPkt::copy(const RadioPkt& other)
{
    this->src = other.src;
    this->dest = other.dest;
    this->positionX = other.positionX;
    this->positionY = other.positionY;
    this->carrierFrequency = other.carrierFrequency;
    this->bandwidth = other.bandwidth;
    this->transmitterPowerMW = other.transmitterPowerMW;
    this->transmitterGainMW = other.transmitterGainMW;
    this->size = other.size;
}

void RadioPkt::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->src);
    doParsimPacking(b,this->dest);
    doParsimPacking(b,this->positionX);
    doParsimPacking(b,this->positionY);
    doParsimPacking(b,this->carrierFrequency);
    doParsimPacking(b,this->bandwidth);
    doParsimPacking(b,this->transmitterPowerMW);
    doParsimPacking(b,this->transmitterGainMW);
    doParsimPacking(b,this->size);
}

void RadioPkt::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->src);
    doParsimUnpacking(b,this->dest);
    doParsimUnpacking(b,this->positionX);
    doParsimUnpacking(b,this->positionY);
    doParsimUnpacking(b,this->carrierFrequency);
    doParsimUnpacking(b,this->bandwidth);
    doParsimUnpacking(b,this->transmitterPowerMW);
    doParsimUnpacking(b,this->transmitterGainMW);
    doParsimUnpacking(b,this->size);
}

int RadioPkt::getSrc() const
{
    return this->src;
}

void RadioPkt::setSrc(int src)
{
    this->src = src;
}

int RadioPkt::getDest() const
{
    return this->dest;
}

void RadioPkt::setDest(int dest)
{
    this->dest = dest;
}

double RadioPkt::getPositionX() const
{
    return this->positionX;
}

void RadioPkt::setPositionX(double positionX)
{
    this->positionX = positionX;
}

double RadioPkt::getPositionY() const
{
    return this->positionY;
}

void RadioPkt::setPositionY(double positionY)
{
    this->positionY = positionY;
}

double RadioPkt::getCarrierFrequency() const
{
    return this->carrierFrequency;
}

void RadioPkt::setCarrierFrequency(double carrierFrequency)
{
    this->carrierFrequency = carrierFrequency;
}

double RadioPkt::getBandwidth() const
{
    return this->bandwidth;
}

void RadioPkt::setBandwidth(double bandwidth)
{
    this->bandwidth = bandwidth;
}

double RadioPkt::getTransmitterPowerMW() const
{
    return this->transmitterPowerMW;
}

void RadioPkt::setTransmitterPowerMW(double transmitterPowerMW)
{
    this->transmitterPowerMW = transmitterPowerMW;
}

double RadioPkt::getTransmitterGainMW() const
{
    return this->transmitterGainMW;
}

void RadioPkt::setTransmitterGainMW(double transmitterGainMW)
{
    this->transmitterGainMW = transmitterGainMW;
}

int RadioPkt::getSize() const
{
    return this->size;
}

void RadioPkt::setSize(int size)
{
    this->size = size;
}

class RadioPktDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_src,
        FIELD_dest,
        FIELD_positionX,
        FIELD_positionY,
        FIELD_carrierFrequency,
        FIELD_bandwidth,
        FIELD_transmitterPowerMW,
        FIELD_transmitterGainMW,
        FIELD_size,
    };
  public:
    RadioPktDescriptor();
    virtual ~RadioPktDescriptor();

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

Register_ClassDescriptor(RadioPktDescriptor)

RadioPktDescriptor::RadioPktDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(RadioPkt)), "omnetpp::cPacket")
{
    propertyNames = nullptr;
}

RadioPktDescriptor::~RadioPktDescriptor()
{
    delete[] propertyNames;
}

bool RadioPktDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<RadioPkt *>(obj)!=nullptr;
}

const char **RadioPktDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *RadioPktDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int RadioPktDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 9+base->getFieldCount() : 9;
}

unsigned int RadioPktDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_src
        FD_ISEDITABLE,    // FIELD_dest
        FD_ISEDITABLE,    // FIELD_positionX
        FD_ISEDITABLE,    // FIELD_positionY
        FD_ISEDITABLE,    // FIELD_carrierFrequency
        FD_ISEDITABLE,    // FIELD_bandwidth
        FD_ISEDITABLE,    // FIELD_transmitterPowerMW
        FD_ISEDITABLE,    // FIELD_transmitterGainMW
        FD_ISEDITABLE,    // FIELD_size
    };
    return (field >= 0 && field < 9) ? fieldTypeFlags[field] : 0;
}

const char *RadioPktDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "src",
        "dest",
        "positionX",
        "positionY",
        "carrierFrequency",
        "bandwidth",
        "transmitterPowerMW",
        "transmitterGainMW",
        "size",
    };
    return (field >= 0 && field < 9) ? fieldNames[field] : nullptr;
}

int RadioPktDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "src") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "dest") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "positionX") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "positionY") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "carrierFrequency") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "bandwidth") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "transmitterPowerMW") == 0) return baseIndex + 6;
    if (strcmp(fieldName, "transmitterGainMW") == 0) return baseIndex + 7;
    if (strcmp(fieldName, "size") == 0) return baseIndex + 8;
    return base ? base->findField(fieldName) : -1;
}

const char *RadioPktDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_src
        "int",    // FIELD_dest
        "double",    // FIELD_positionX
        "double",    // FIELD_positionY
        "double",    // FIELD_carrierFrequency
        "double",    // FIELD_bandwidth
        "double",    // FIELD_transmitterPowerMW
        "double",    // FIELD_transmitterGainMW
        "int",    // FIELD_size
    };
    return (field >= 0 && field < 9) ? fieldTypeStrings[field] : nullptr;
}

const char **RadioPktDescriptor::getFieldPropertyNames(int field) const
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

const char *RadioPktDescriptor::getFieldProperty(int field, const char *propertyName) const
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

int RadioPktDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    RadioPkt *pp = omnetpp::fromAnyPtr<RadioPkt>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void RadioPktDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    RadioPkt *pp = omnetpp::fromAnyPtr<RadioPkt>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'RadioPkt'", field);
    }
}

const char *RadioPktDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    RadioPkt *pp = omnetpp::fromAnyPtr<RadioPkt>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string RadioPktDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    RadioPkt *pp = omnetpp::fromAnyPtr<RadioPkt>(object); (void)pp;
    switch (field) {
        case FIELD_src: return long2string(pp->getSrc());
        case FIELD_dest: return long2string(pp->getDest());
        case FIELD_positionX: return double2string(pp->getPositionX());
        case FIELD_positionY: return double2string(pp->getPositionY());
        case FIELD_carrierFrequency: return double2string(pp->getCarrierFrequency());
        case FIELD_bandwidth: return double2string(pp->getBandwidth());
        case FIELD_transmitterPowerMW: return double2string(pp->getTransmitterPowerMW());
        case FIELD_transmitterGainMW: return double2string(pp->getTransmitterGainMW());
        case FIELD_size: return long2string(pp->getSize());
        default: return "";
    }
}

void RadioPktDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    RadioPkt *pp = omnetpp::fromAnyPtr<RadioPkt>(object); (void)pp;
    switch (field) {
        case FIELD_src: pp->setSrc(string2long(value)); break;
        case FIELD_dest: pp->setDest(string2long(value)); break;
        case FIELD_positionX: pp->setPositionX(string2double(value)); break;
        case FIELD_positionY: pp->setPositionY(string2double(value)); break;
        case FIELD_carrierFrequency: pp->setCarrierFrequency(string2double(value)); break;
        case FIELD_bandwidth: pp->setBandwidth(string2double(value)); break;
        case FIELD_transmitterPowerMW: pp->setTransmitterPowerMW(string2double(value)); break;
        case FIELD_transmitterGainMW: pp->setTransmitterGainMW(string2double(value)); break;
        case FIELD_size: pp->setSize(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'RadioPkt'", field);
    }
}

omnetpp::cValue RadioPktDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    RadioPkt *pp = omnetpp::fromAnyPtr<RadioPkt>(object); (void)pp;
    switch (field) {
        case FIELD_src: return pp->getSrc();
        case FIELD_dest: return pp->getDest();
        case FIELD_positionX: return pp->getPositionX();
        case FIELD_positionY: return pp->getPositionY();
        case FIELD_carrierFrequency: return pp->getCarrierFrequency();
        case FIELD_bandwidth: return pp->getBandwidth();
        case FIELD_transmitterPowerMW: return pp->getTransmitterPowerMW();
        case FIELD_transmitterGainMW: return pp->getTransmitterGainMW();
        case FIELD_size: return pp->getSize();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'RadioPkt' as cValue -- field index out of range?", field);
    }
}

void RadioPktDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    RadioPkt *pp = omnetpp::fromAnyPtr<RadioPkt>(object); (void)pp;
    switch (field) {
        case FIELD_src: pp->setSrc(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_dest: pp->setDest(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_positionX: pp->setPositionX(value.doubleValue()); break;
        case FIELD_positionY: pp->setPositionY(value.doubleValue()); break;
        case FIELD_carrierFrequency: pp->setCarrierFrequency(value.doubleValue()); break;
        case FIELD_bandwidth: pp->setBandwidth(value.doubleValue()); break;
        case FIELD_transmitterPowerMW: pp->setTransmitterPowerMW(value.doubleValue()); break;
        case FIELD_transmitterGainMW: pp->setTransmitterGainMW(value.doubleValue()); break;
        case FIELD_size: pp->setSize(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'RadioPkt'", field);
    }
}

const char *RadioPktDescriptor::getFieldStructName(int field) const
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

omnetpp::any_ptr RadioPktDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    RadioPkt *pp = omnetpp::fromAnyPtr<RadioPkt>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void RadioPktDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    RadioPkt *pp = omnetpp::fromAnyPtr<RadioPkt>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'RadioPkt'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp

