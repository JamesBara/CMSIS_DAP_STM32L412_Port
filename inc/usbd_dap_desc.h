#ifndef USBD_DAP_DESC_H
#define USBD_DAP_DESC_H

#include <stdbool.h>
#include "usbd_desc.h"
#include "ms_os_20_desc.h"


/************************************************
* USBD DAP Specific Descriptors.
***********************************************/

struct __PACKED usbd_configuration_descriptor
{
	struct usbd_std_configuration_descriptor_type std_conf_desc;
	struct usbd_std_interface_descriptor_type std_interface_desc;
	struct usbd_std_endpoint_descriptor_type std_ep_desc[2];
};

struct __PACKED usbd_string_descriptor_zero_type
{
	struct usbd_std_string_descriptor_type std_string_desc;
	uint16_t wLangID;
};

struct __PACKED usbd_string_descriptor_product_type
{
	struct usbd_std_string_descriptor_type std_string_desc;
	uint8_t string[18];
};

struct __PACKED usbd_bos_descriptor_type
{
	struct usbd_std_bos_descriptor_type std_bos_desc;
	struct usbd_std_platform_capability_descriptor_type std_plat_cap_desc;
	struct usbd_ms_os20_set_information_descriptor_type ms_os20_set_info_desc;
};

struct __PACKED ms_os20_descriptor_type
{
	struct usbd_ms_os20_set_header_type set_header_desc;
	struct usbd_ms_os20_compatible_id_feature_type compatible_id_feature_type_desc;
	struct usbd_ms_os20_registry_property_feature_type registry_property_feature_desc;
};

/************************************************
 *  usbd_core_driver struct callback functions.
 ***********************************************/

bool is_selfpowered(void);
bool get_remote_wakeup(void);
bool is_interface_valid(uint8_t num);
bool is_endpoint_valid(uint8_t num, uint8_t dir);
uint8_t* get_device_descriptor(void);
uint8_t* get_configuration_descriptor(uint8_t index);
uint8_t* get_string_descriptor(uint8_t index, uint16_t lang_id);
uint8_t* get_bos_descriptor(void);
uint8_t get_configuration(void);
bool is_configuration_valid(uint8_t num);
uint8_t get_interface(uint8_t num);
void usbd_vendor_handler(struct usbd_setup_packet_type setup);

#endif /*USBD_DAP_DESC_H*/