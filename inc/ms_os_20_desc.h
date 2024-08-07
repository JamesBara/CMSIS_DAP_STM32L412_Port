#ifndef MS_OS_20_DESC_H
#define MS_OS_20_DESC_H

#include <stdint.h>
#include "cmsis_gcc.h"

#define MS_OS_20_PLATFORM 0x5

#define MS_OS_20_LENGTH_SET_HEADER 10
#define MS_OS_20_LENGTH_FEATURE_COMPATIBLE_ID 20


#define MS_OS_20_WINDOWS_VERSION_81 0x06030000UL

#define MS_OS_20_SET_HEADER_DESC 0
#define MS_OS_20_FEATURE_COMPATIBLE_ID_DESC 3
#define MS_OS_20_FEATURE_REGISTER_PROPERTY_DESC 4

#define MS_OS_20_REG_SZ 1

#define MS_OS_20_CUSTOM_REQUEST 0x1U


#define MS_OS_20_DESCRIPTOR_INDEX 0x7
#define MS_OS_20_SET_ALT_ENUMERATION 0x8


/************************************************
 * Microsoft OS 2.0 Descriptor Set Information 
 ***********************************************/
/*CapabilityData of the MS OS 2.0 descriptor.*/
struct __PACKED usbd_ms_os20_set_information_descriptor_type
{
    uint32_t dwWindowsVersion;
    uint16_t wMSOSDescriptorSetTotalLength;
    uint8_t bMS_VendorCode;
    uint8_t bAltEnumCode;
};

struct __PACKED usbd_ms_os20_set_header_type
{
	uint16_t wLength;
	uint16_t wDescriptorType;
	uint32_t dwWindowsVersion;
	uint16_t wTotalLength;
};

struct __PACKED usbd_ms_os20_compatible_id_feature_type
{
	uint16_t wLength;
	uint16_t wDescriptorType;
	uint8_t CompatibleID[8];
	uint8_t SubCompatibleID[8];
};

struct __PACKED usbd_ms_os20_registry_property_feature_type
{
	uint16_t wLength;
	uint16_t wDescriptorType;
	uint16_t wPropertyDataType;
	uint16_t wPropertyNameLength;
	uint8_t PropertyName[40];
	uint16_t wPropertyDataLength;
	uint8_t PropertyData[78];
};



#endif /*MS_OS_20_DESC_H*/