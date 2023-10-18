#ifndef CPPUTILS_H_
#define CPPUTILS_H_

// MAC ADDRESS AND IDS
#define NULL_MAC_ADDRESS -1
typedef int mac_address_t;
typedef int device_id_t;

// pkt names utility
#define NAME_INIT(name, macAddress, seqNum) (name + std::string("@") + std::to_string(macAddress) + "#" + std::to_string(seqNum)).c_str()
#define NAME_ENCAPSULATE(name, packet) std::string(name + std::string("[") + std::string(packet->getName()) + "]").c_str()

// MACROS
#define SIGN(x) (x < 0 ? -1 : 1)

#endif