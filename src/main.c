#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "usbd_core.h"
#include "usbd_dap_desc.h"
#include "usbd_dap.h"
#include "DAP.h"
#include "bsp.h"

/**
 * @brief Device configuration.
 */
static struct usbd_core_driver core_drv =
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
	usbd_core_init(&core_drv);
	while (1)
	{

	}

	return 0;
}



