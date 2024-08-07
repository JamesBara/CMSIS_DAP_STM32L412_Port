#include "usbd_dap.h"
#include "usbd_core.h"
#include "DAP_config.h"
#include "DAP.h"

/************************************************
 *  Endpoint 1 buffers.
 ***********************************************/

uint8_t ep1_out_buffer[DAP_PACKET_SIZE];
uint8_t ep1_in_buffer[DAP_PACKET_SIZE];
__IO uint16_t ep1_in_cnt;

/************************************************
 *  Endpoint 1 callback function prototypes.
 ***********************************************/

static void ep1_in_handler(void);
static void ep1_out_handler(void);

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
 * @brief Callback that resumes endpoint work after clearing a stall.
 * @param num Endpoint number that got cleared.
 * @param dir Direction of endpoint that got cleared.
 */
void clear_stall(uint8_t num, uint8_t dir)
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
 * @brief Setup stage callback for SET_CONFIGURATION request. Registers the endpoints for selected configuration.
 * @param num Configuration number sent by host.
 */
void set_configuration(uint8_t num)
{
	UNUSED(num);
	usbd_register_ep(EP1, USB_EP_TYPE_BULK, ADDR1_TX, ADDR1_RX, COUNT1_RX, ep1_in_handler, ep1_out_handler);
}