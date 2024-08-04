#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "ms_os_20_request.h"
#include "usbd_request.h"
#include "usbd_core.h"
#include "DAP_config.h"
#include "DAP.h"
#include "bsp.h"

/************************************************
 * @brief Endpoint 1 PMA Addresses
 ***********************************************/

#ifndef ADDR1_TX
	#define ADDR1_TX 192
#endif
#ifndef ADDR1_RX
	#define ADDR1_RX 256
#endif
#ifndef COUNT1_RX
	#define COUNT1_RX USBD_FS_MAX_PACKET_SIZE
#endif

 /************************************************
 * @brief USBD Descriptors.
 ***********************************************/

struct __PACKED usbd_configuration_descriptor
{
	usbd_std_configuration_descriptor_type std_conf_desc;
	usbd_std_interface_descriptor_type std_interface_desc;
	usbd_std_endpoint_descriptor_type std_ep_desc[2];
};

struct __PACKED usbd_string_descriptor_zero_type
{
	usbd_std_string_descriptor_type std_string_desc;
	uint16_t wLangID;
};

struct __PACKED usbd_string_descriptor_product_type
{
	usbd_std_string_descriptor_type std_string_desc;
	uint8_t string[18];
};

struct __PACKED usbd_bos_descriptor_type
{
	usbd_std_bos_descriptor_type std_bos_desc;
	usbd_std_platform_capability_descriptor_type std_plat_cap_desc;
	usbd_ms_os20_set_information_descriptor_type ms_os20_set_info_desc;
};

struct __PACKED ms_os20_descriptor_type
{
	usbd_ms_os20_set_header_type set_header_desc;
	usbd_ms_os20_compatible_id_feature_type compatible_id_feature_type_desc;
	usbd_ms_os20_registry_property_feature_type registry_property_feature_desc;
};

/************************************************
 * @brief Custom descriptor values for CMSIS-DAP
 * STM32L412_Port
 ***********************************************/
static const usbd_std_device_descriptor_type usbd_dev_desc =
{
	USBD_LENGTH_DEVICE_DESC,
	USBD_DESC_TYPE_DEVICE,
	USBD_BCD_USB21,
	USBD_CLASS_NONE,
	0x0U,
	0x0U,
	EP0_COUNT,
	0xC251U, /*!< Keil*/
	0xF001U, /*!< DAP-Link*/
	USBD_BCD_DEVICE,
	0x0U,
	0x1U,
	0x0U,
	1
};

static const struct usbd_configuration_descriptor usbd_conf_desc =
{
	{
		USBD_LENGTH_CONFIGURATION_DESC,
		USBD_DESC_TYPE_CONFIGURATION,
		32,
		1,
		1,
		0,
		USBD_ATTRIBUTES(0,0), /*!< Bus powered, Remote wakeup is disabled.*/
		USBD_MAX_POWER(500) /*!< 500mA */
	},
	{
		USBD_LENGTH_INTERFACE_DESC,
		USBD_DESC_TYPE_INTERFACE,
		0,
		0,
		2,
		USBD_CLASS_VENDOR,
		0x0U,
		0x0U,
		0
	},
	{
		{
			USBD_LENGTH_ENDPOINT_DESC,
			USBD_DESC_TYPE_ENDPOINT,
			0x1U,
			USBD_ATTRIBUTES_TRANSFER_TYPE_BULK,
			USBD_FS_MAX_PACKET_SIZE,
			0x0U
		},
		{
			USBD_LENGTH_ENDPOINT_DESC,
			USBD_DESC_TYPE_ENDPOINT,
			0x81U,
			USBD_ATTRIBUTES_TRANSFER_TYPE_BULK,
			USBD_FS_MAX_PACKET_SIZE,
			0x0U
		}
		}
};

static const struct usbd_string_descriptor_zero_type usbd_str0_desc =
{
	{
		sizeof(usbd_str0_desc),
		USBD_DESC_TYPE_STRING
	},
	USBD_LANGID_ENGLISH_US
};

static const struct usbd_string_descriptor_product_type usbd_product_desc =
{
	{
		sizeof(usbd_product_desc),
		USBD_DESC_TYPE_STRING
	},
	{'C', 0, 'M', 0, 'S', 0, 'I', 0, 'S', 0, '-', 0, 'D', 0, 'A', 0, 'P', 0}
};

static const struct ms_os20_descriptor_type ms_os20_desc =
{
	{
		MS_OS_20_LENGTH_SET_HEADER,
		MS_OS_20_SET_HEADER_DESC,
		MS_OS_20_WINDOWS_VERSION_81,
		sizeof(ms_os20_desc)
	},
	{
		MS_OS_20_LENGTH_FEATURE_COMPATIBLE_ID,
		MS_OS_20_FEATURE_COMPATIBLE_ID_DESC,
		{'W', 'I', 'N', 'U', 'S', 'B', 0, 0}, /*!< CompatibleID */
		{0, 0, 0, 0, 0, 0, 0, 0} /*!< SubCompatibleID */
	},
	{
		sizeof(usbd_ms_os20_registry_property_feature_type),
		MS_OS_20_FEATURE_REGISTER_PROPERTY_DESC,
		MS_OS_20_REG_SZ,
		40,
		{'D', 0, 'e', 0, 'v', 0, 'i', 0, 'c', 0, 'e', 0, 'I', 0, 'n', 0, 't', 0, 'e', 0,
		'r', 0, 'f', 0, 'a', 0, 'c', 0, 'e', 0, 'G', 0, 'U', 0, 'I', 0, 'D', 0, 0, 0},
		78,
		{'{', 0, '1', 0, 'F', 0, '0', 0, 'B', 0, '7', 0, '7', 0, '9', 0, 'C', 0, '-', 0,
		'E', 0, '6', 0, 'F', 0, '2', 0, '-', 0, '4', 0, '9', 0, '4', 0, '4', 0, '-', 0,
		'9', 0, 'B', 0, '4', 0, 'D', 0, '-', 0, '1', 0, '8', 0, '6', 0, '1', 0, '2', 0,
		'D', 0, '7', 0, '4', 0, 'A', 0, '6', 0, 'B', 0, 'B', 0, '}', 0, 0, 0 } /*!< Random GUID generated in Windows 11 (1f0b779c - e6f2 - 4944 - 9b4d - 18612d74a6bb).*/
	}
};

static const struct usbd_bos_descriptor_type usbd_bos_desc =
{
	{
		USBD_LENGTH_BOS_DESC,
		USBD_DESC_TYPE_BOS,
		33,
		1
	},
	{
		28,
		USBD_DESC_TYPE_DEVICE_CAPABILITY,
		MS_OS_20_PLATFORM,
		0x0U,
		{0xDF, 0x60, 0xDD, 0xD8, 0x89, 0x45, 0xC7, 0x4C, 0x9C, 0xD2, 0x65, 0x9D, 0x9E, 0x64, 0x8A, 0x9F} /*!< CMSIS-DAP UUID.*/
	},
	{
		MS_OS_20_WINDOWS_VERSION_81,
		sizeof(ms_os20_desc),
		MS_OS_20_CUSTOM_REQUEST,
		0x0U
	}
};

/************************************************
 * @brief Endpoint 1 buffers.
 ***********************************************/

uint8_t ep1_out_buffer[DAP_PACKET_SIZE];
uint8_t ep1_in_buffer[DAP_PACKET_SIZE];
uint16_t ep1_in_cnt;

/************************************************
 * @brief Function prototypes, used as callbacks
 * for usbd_core_config_type struct.
 ***********************************************/
static bool is_selfpowered(void);
static bool get_remote_wakeup(void);
static bool is_interface_valid(uint8_t num);
static bool is_endpoint_valid(uint8_t num, uint8_t dir);
static void clear_stall(uint8_t num, uint8_t dir);
static uint8_t* get_device_descriptor(void);
static uint8_t* get_configuration_descriptor(uint8_t index);
static uint8_t* get_string_descriptor(uint8_t index, uint16_t lang_id);
static uint8_t* get_bos_descriptor(void);
static uint8_t get_configuration(void);
static bool is_configuration_valid(uint8_t num);
static void set_configuration(uint8_t num);
static uint8_t get_interface(uint8_t num);
static void usbd_vendor_handler(usbd_setup_packet_type setup);

static void ep1_in_handler(void);
static void ep1_out_handler(void);

/**
 * @brief Device configuration.
 */
static usbd_core_config_type usbd_config =
{
	is_selfpowered,
	NULL,
	get_remote_wakeup,
	is_interface_valid,
	is_endpoint_valid,
	clear_stall,
	get_device_descriptor,
	get_configuration_descriptor,
	get_string_descriptor,
	get_bos_descriptor,
	NULL,
	get_configuration,
	is_configuration_valid,
	set_configuration,
	get_interface,
	NULL,
	NULL,
	usbd_vendor_handler,
	NULL,
	NULL,
	NULL
};

int main(void)
{
	bsp_init();
	DAP_Setup();
	usbd_core_init(&usbd_config);
	while (1)
	{

	}

	return 0;
}

/**
 * @brief Callback that handles endpoint 1 IN direction.
 * @param  
 */
static void ep1_in_handler(void)
{
	USBD_EP_SET_STAT_RX(EP1, USB_EP_STAT_RX_VALID);
}

/**
 * @brief Callback that handles endpoint 1 OUT direction.
 * @param  
 */
static void ep1_out_handler(void)
{
	uint32_t cnt = USBD_PMA_GET_RX_COUNT(EP1);
	usbd_pma_read(ADDR1_RX, ep1_out_buffer, cnt);
	ep1_in_cnt = (DAP_ExecuteCommand(ep1_out_buffer, ep1_in_buffer) & 0xFFFFFFFFUL);

	if (ep1_in_cnt)
	{
		USBD_PMA_SET_TX_COUNT(EP1, MIN(ep1_in_cnt, USBD_FS_MAX_PACKET_SIZE));
		usbd_pma_write(ADDR1_TX, ep1_in_buffer, MIN(ep1_in_cnt, USBD_FS_MAX_PACKET_SIZE));
		USBD_EP_SET_STAT_TX(EP1, USB_EP_STAT_TX_VALID);
	}
}

/**
 * @brief Setup stage callback for GET_STATUS request, with device recipient.
 * @param  
 * @return true if device is selfpowered, false if it is buspowered.
 */
static bool is_selfpowered(void)
{
	return false;
}

/**
 * @brief Setup stage callback for GET_STATUS request, with device recipient.
 * @param  
 * @return true if remote wakeup is enabled, false if it isn't.
 */
static bool get_remote_wakeup(void)
{
	return false;
}

/**
 * @brief Callback that checks if an interface is valid.
 * @param num Interface number to check.
 * @return 
 */
static bool is_interface_valid(uint8_t num)
{
	switch (num)
	{
		case 0:
		{
			return true;
			break;
		}
		default:
		{
			return false;
			break;
		}
	}
}

/**
 * @brief Callback that checks if an endpoint is valid.
 * @param num Endpoint number to check.
 * @param dir Direction of endpoint to check.
 * @return 
 */
static bool is_endpoint_valid(uint8_t num, uint8_t dir)
{
	UNUSED(dir);
	switch (num)
	{
		case 1:
		{
			return true;
			break;
		}
		default:
		{
			return false;
			break;
		}
	}
}

/**
 * @brief Callback that resumes endpoint work after clearing a stall.
 * @param num Endpoint number that got cleared.
 * @param dir Direction of endpoint that got cleared.
 */
static void clear_stall(uint8_t num, uint8_t dir)
{
	if (num == 1)
	{
		if (dir)
		{
			ep1_in_handler();
		}
		else
		{
			ep1_out_handler();
		}
	}
}

/**
 * @brief Callback that provides the device descriptor.
 * @param  
 * @return uint8_t pointer to the device descriptor.
 */
static uint8_t* get_device_descriptor(void)
{
	return (uint8_t*)&usbd_dev_desc;
}

/**
 * @brief Callback that provides a configuration descriptor.
 * @param index Configuration index sent by host.
 * @return uint8_t pointer to a configuration descriptor.
 */
static uint8_t* get_configuration_descriptor(uint8_t index)
{
	UNUSED(index);
	return (uint8_t*)&usbd_conf_desc;
}

/**
 * @brief Callback that provides a string descriptor.
 * @param index String index sent by host.
 * @param lang_id Language ID for string 0 descriptor.
 * @return uint8_t pointer to a string descriptor.
 */
static uint8_t* get_string_descriptor(uint8_t index, uint16_t lang_id)
{
	UNUSED(lang_id);
	switch (index)
	{
		case 0:
		{
			return (uint8_t*)&usbd_str0_desc;
			break;
		}
		case 1:
		{
			return (uint8_t*)&usbd_product_desc;
			break;
		}
		default:
		{
			return NULL;
			break;
		}
	}
}

/**
 * @brief Callback that provides the BOS descriptor.
 * @param  
 * @return uint8_t pointer to the BOS descriptor.
 */
static uint8_t* get_bos_descriptor(void)
{
	return (uint8_t*)&usbd_bos_desc;
}

/**
 * @brief Setup stage callback for GET_CONFIGURATION request.
 * @param  
 * @return The number of the current configuration.
 */
static uint8_t get_configuration(void)
{
	return 1;
}

/**
 * @brief Callback that checks if a configuration is valid.
 * @param num Configuration number sent by host.
 * @return 
 */
static bool is_configuration_valid(uint8_t num)
{
	switch (num)
	{
		case 1:
		{
			return true;
			break;
		}
		default:
		{
			return false;
			break;
		}
	}
}

/**
 * @brief Setup stage callback for SET_CONFIGURATION request. Registers the endpoints for selected configuration.
 * @param num Configuration number sent by host.
 */
static void set_configuration(uint8_t num)
{
	UNUSED(num);
	usbd_register_ep(EP1, USB_EP_TYPE_BULK, ADDR1_TX, ADDR1_RX, COUNT1_RX, ep1_in_handler, ep1_out_handler);
}

/**
 * @brief Setup stage callback for GET_INTERFACE request.
 * @param num Interface number sent by host.
 * @return Alternative interface of the selected interface.
 */
static uint8_t get_interface(uint8_t num)
{
	UNUSED(num);
	return 0;
}

/**
 * @brief Setup stage callback for vendor recipient request. Handles Microsoft OS 2.0 descriptor.
 * @param setup USB setup packet.
 */
static void usbd_vendor_handler(usbd_setup_packet_type setup)
{
	switch(setup.bRequest)
	{
		case MS_OS_20_CUSTOM_REQUEST:
		{
			switch (setup.wIndex)
			{
				case MS_OS_20_DESCRIPTOR_INDEX:
				{
					usbd_prepare_data_in_stage((uint8_t*)&ms_os20_desc, sizeof(ms_os20_desc));
					break;
				}
				default:
				{
					USBD_EP0_SET_STALL();
					break;
				}
			}
			break;
		}
		default:
		{
			USBD_EP0_SET_STALL();
			break;
		}
	}
}