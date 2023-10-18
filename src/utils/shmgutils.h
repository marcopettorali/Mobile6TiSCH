#ifndef SHMGUTILS_H_
#define SHMGUTILS_H_

// TYPE DEFINITIONS
typedef mac_address_t mn_mac_address_t;
typedef mac_address_t br_mac_address_t;

typedef device_id_t mn_device_id_t;
typedef device_id_t br_device_id_t;

// INIT STAGE
#define INITSTAGE_SHMG_NC_BUILD_SCHEDULE INITSTAGE_EXIT_STD + 0
#define INITSTAGE_SHMG_MN_GET_SCHEDULE INITSTAGE_EXIT_STD + 1

#endif