#include "usbd_dap_desc.h"
#include "usbd_core.h"

/************************************************
 *  Custom descriptor values for 
 *  CMSIS-DAP STM32L412_Port.
 ***********************************************/
static const struct usbd_std_device_descriptor_type usbd_dev_desc =
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
		USBD_ATTRIBUTES_CONFIGURATION(0,0),
		USBD_MAX_POWER(500)
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
		sizeof(struct usbd_ms_os20_registry_property_feature_type),
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

/**
 * @brief Setup stage callback for GET_STATUS request, with device recipient.
 * @param
 * @return true if device is selfpowered, false if it is buspowered.
 */
bool is_selfpowered(void)
{
	return false;
}

/**
 * @brief Setup stage callback for GET_STATUS request, with device recipient.
 * @param
 * @return true if remote wakeup is enabled, false if it isn't.
 */
bool get_remote_wakeup(void)
{
	return false;
}

/**
 * @brief Callback that checks if an interface is valid.
 * @param num Interface number to check.
 * @return
 */
bool is_interface_valid(uint8_t num)
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
bool is_endpoint_valid(uint8_t num, uint8_t dir)
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
 * @brief Callback that provides the device descriptor.
 * @param
 * @return uint8_t pointer to the device descriptor.
 */
uint8_t* get_device_descriptor(void)
{
	return (uint8_t*)&usbd_dev_desc;
}

/**
 * @brief Callback that provides a configuration descriptor.
 * @param index Configuration index sent by host.
 * @return uint8_t pointer to a configuration descriptor.
 */
uint8_t* get_configuration_descriptor(uint8_t index)
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
uint8_t* get_string_descriptor(uint8_t index, uint16_t lang_id)
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
uint8_t* get_bos_descriptor(void)
{
	return (uint8_t*)&usbd_bos_desc;
}

/**
 * @brief Setup stage callback for GET_CONFIGURATION request.
 * @param
 * @return The number of the current configuration.
 */
uint8_t get_configuration(void)
{
	return 1;
}

/**
 * @brief Callback that checks if a configuration is valid.
 * @param num Configuration number sent by host.
 * @return
 */
bool is_configuration_valid(uint8_t num)
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
 * @brief Setup stage callback for GET_INTERFACE request.
 * @param num Interface number sent by host.
 * @return Alternative interface of the selected interface.
 */
uint8_t get_interface(uint8_t num)
{
	UNUSED(num);
	return 0;
}

/**
 * @brief Setup stage callback for vendor recipient request. Handles Microsoft OS 2.0 descriptor.
 * @param setup USB setup packet.
 */
void usbd_vendor_handler(struct usbd_setup_packet_type setup)
{
	switch (setup.bRequest)
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