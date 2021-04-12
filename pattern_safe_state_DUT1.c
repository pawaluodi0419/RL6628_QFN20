#include "platform.h"

void pattern_safe_state_dut1()
{
	XGpio_WriteBit(XPAR_AXI_GPIO_dut1_1_BASEADDR,0,5, 0);
	XGpio_WriteBit(XPAR_AXI_GPIO_dut1_1_BASEADDR,0,15, 0);
	//XGpio_WriteBit(XPAR_AXI_GPIO_dut1_1_BASEADDR,0,16, 0);

	//DAC output 0v
	dut1.g_dac_data_buf[0] = 0x01;	//0x01: CC1 0x03: CC2
	dut1.g_dac_data_buf[1] = 0x00;	//Data L
	dut1.g_dac_data_buf[2] = 0x00;	//Data H
	dut1.g_dac_data_buf[3] = 0x00;	//Data L
	dut1.g_dac_data_buf[4] = 0x00;	//Data H
	dut1.g_dac_data_buf[5] = 0x00;	//Data L
	dut1.g_dac_data_buf[6] = 0x00;	//Data H
	_by_mcp4822_output_control_dut1(dut1.g_dac_data_buf);
	msdelay(10);

	dut1.g_dac_data_buf[0] = 0x03;	//0x01: CC1 0x03: CC2
	dut1.g_dac_data_buf[1] = 0x00;	//Data L
	dut1.g_dac_data_buf[2] = 0x00;	//Data H
	dut1.g_dac_data_buf[3] = 0x00;	//Data L
	dut1.g_dac_data_buf[4] = 0x00;	//Data H
	dut1.g_dac_data_buf[5] = 0x00;	//Data L
	dut1.g_dac_data_buf[6] = 0x00;	//Data H
	_by_mcp4822_output_control_dut1(dut1.g_dac_data_buf);
	msdelay(10);

	//DUT IC power off
	XGpio_WriteBit(XPAR_AXI_GPIO_dut1_1_BASEADDR,0,1, 0);
	//XGpio_2_WriteBit(0,dut1_FT2_PWR_CTRL_OFFSET,0x00);
	msdelay(50);
}


