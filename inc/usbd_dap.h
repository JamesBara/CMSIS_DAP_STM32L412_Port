#ifndef USBD_DAP_H
#define USBD_DAP_H

#include "usbd_desc.h"

/************************************************
 *  Endpoint 1 PMA Addresses
 ***********************************************/

#define ADDR1_TX 192
#define ADDR1_RX 256
#define COUNT1_RX USBD_FS_MAX_PACKET_SIZE

/************************************************
 *  usbd_core_driver struct callback functions.
 ***********************************************/

void clear_stall(uint8_t num, uint8_t dir);
void set_configuration(uint8_t num);



#endif /*USBD_DAP_H*/