//
// Generated file, do not edit! Created by opp_msgtool 6.0 from physicallayer/medium/intarray/IntArray.msg.
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
#include "IntArray_m.h"

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

Register_Class(IntArray)

IntArray::IntArray(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

IntArray::IntArray(const IntArray& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

IntArray::~IntArray()
{
    delete [] this->data;
}

IntArray& IntArray::operator=(const IntArray& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void IntArray::copy(const IntArray& other)
{
    delete [] this->data;
    this->data = (other.data_arraysize==0) ? nullptr : new int[other.data_arraysize];
    data_arraysize = other.data_arraysize;
    for (size_t i = 0; i < data_arraysize; i++) {
        this->data[i] = other.data[i];
    }
}

void IntArray::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    b->pack(data_arraysize);
    doParsimArrayPacking(b,this->data,data_arraysize);
}

void IntArray::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    delete [] this->data;
    b->unpack(data_arraysize);
    if (data_arraysize == 0) {
        this->data = nullptr;
    } else {
        this->data = new int[data_arraysize];
        doParsimArrayUnpacking(b,this->data,data_arraysize);
    }
}

size_t IntArray::getDataArraySize() const
{
    return data_arraysize;
}

int IntArray::getData(size_t k) const
{
    if (k >= data_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)data_arraysize, (unsigned long)k);
    return this->data[k];
}

void IntArray::setDataArraySize(size_t newSize)
{
    int *data2 = (newSize==0) ? nullptr : new int[newSize];
    size_t minSize = data_arraysize < newSize ? data_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        data2[i] = this->data[i];
    for (size_t i = minSize; i < newSize; i++)
        data2[i] = 0;
    delete [] this->data;
    this->data = data2;
    data_arraysize = newSize;
}

void IntArray::setData(size_t k, int data)
{
    if (k >= data_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)data_arraysize, (unsigned long)k);
    this->data[k] = data;
}

void IntArray::insertData(size_t k, int data)
{
    if (k > data_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)data_arraysize, (unsigned long)k);
    size_t newSize = data_arraysize + 1;
    int *data2 = new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        data2[i] = this->data[i];
    data2[k] = data;
    for (i = k + 1; i < newSize; i++)
        data2[i] = this->data[i-1];
    delete [] this->data;
    this->data = data2;
    data_arraysize = newSize;
}

void IntArray::appendData(int data)
{
    insertData(data_arraysize, data);
}

void IntArray::eraseData(size_t k)
{
    if (k >= data_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)data_arraysize, (unsigned long)k);
    size_t newSize = data_arraysize - 1;
    int *data2 = (newSize == 0) ? nullptr : new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        data2[i] = this->data[i];
    for (i = k; i < newSize; i++)
        data2[i] = this->data[i+1];
    delete [] this->data;
    this->data = data2;
    data_arraysize = newSize;
}

class IntArrayDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_data,
    };
  public:
    IntArrayDescriptor();
    virtual ~IntArrayDescriptor();

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

Register_ClassDescriptor(IntArrayDescriptor)

IntArrayDescriptor::IntArrayDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(IntArray)), "omnetpp::cPacket")
{
    propertyNames = nullptr;
}

IntArrayDescriptor::~IntArrayDescriptor()
{
    delete[] propertyNames;
}

bool IntArrayDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<IntArray *>(obj)!=nullptr;
}

const char **IntArrayDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *IntArrayDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int IntArrayDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 1+base->getFieldCount() : 1;
}

unsigned int IntArrayDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISARRAY | FD_ISEDITABLE | FD_ISRESIZABLE,    // FIELD_data
    };
    return (field >= 0 && field < 1) ? fieldTypeFlags[field] : 0;
}

const char *IntArrayDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "data",
    };
    return (field >= 0 && field < 1) ? fieldNames[field] : nullptr;
}

int IntArrayDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "data") == 0) return baseIndex + 0;
    return base ? base->findField(fieldName) : -1;
}

const char *IntArrayDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_data
    };
    return (field >= 0 && field < 1) ? fieldTypeStrings[field] : nullptr;
}

const char **IntArrayDescriptor::getFieldPropertyNames(int field) const
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

const char *IntArrayDescriptor::getFieldProperty(int field, const char *propertyName) const
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

int IntArrayDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    IntArray *pp = omnetpp::fromAnyPtr<IntArray>(object); (void)pp;
    switch (field) {
        case FIELD_data: return pp->getDataArraySize();
        default: return 0;
    }
}

void IntArrayDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    IntArray *pp = omnetpp::fromAnyPtr<IntArray>(object); (void)pp;
    switch (field) {
        case FIELD_data: pp->setDataArraySize(size); break;
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'IntArray'", field);
    }
}

const char *IntArrayDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    IntArray *pp = omnetpp::fromAnyPtr<IntArray>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string IntArrayDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    IntArray *pp = omnetpp::fromAnyPtr<IntArray>(object); (void)pp;
    switch (field) {
        case FIELD_data: return long2string(pp->getData(i));
        default: return "";
    }
}

void IntArrayDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    IntArray *pp = omnetpp::fromAnyPtr<IntArray>(object); (void)pp;
    switch (field) {
        case FIELD_data: pp->setData(i,string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'IntArray'", field);
    }
}

omnetpp::cValue IntArrayDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    IntArray *pp = omnetpp::fromAnyPtr<IntArray>(object); (void)pp;
    switch (field) {
        case FIELD_data: return pp->getData(i);
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'IntArray' as cValue -- field index out of range?", field);
    }
}

void IntArrayDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    IntArray *pp = omnetpp::fromAnyPtr<IntArray>(object); (void)pp;
    switch (field) {
        case FIELD_data: pp->setData(i,omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'IntArray'", field);
    }
}

const char *IntArrayDescriptor::getFieldStructName(int field) const
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

omnetpp::any_ptr IntArrayDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    IntArray *pp = omnetpp::fromAnyPtr<IntArray>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void IntArrayDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    IntArray *pp = omnetpp::fromAnyPtr<IntArray>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'IntArray'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp

