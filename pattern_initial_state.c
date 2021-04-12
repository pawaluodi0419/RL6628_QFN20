#include "platform.h"

void pattern_initial_state()
{
	XGpio_2_WriteBit(0,dut0_FT2_PWR_CTRL_OFFSET,0x00);	//S0-PWR_EN: 控制S0-ATX_5V是否开出S0-5V（未经过采样电阻）
	XGpio_2_WriteBit(0,dut1_FT2_PWR_CTRL_OFFSET,0x00);
	XGpio_2_WriteBit(0,dut2_FT2_PWR_CTRL_OFFSET,0x00);
	XGpio_2_WriteBit(0,dut3_FT2_PWR_CTRL_OFFSET,0x00);

	XGpio_WriteBit(XPAR_AXI_GPIO_dut0_1_BASEADDR,0,1, 0);	//S0-PWR_EN_2: 控制S0-5V是否开出S0-5V_IN/S0-VCON_IN
	XGpio_WriteBit(XPAR_AXI_GPIO_dut1_1_BASEADDR,0,1, 0);
	XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,1, 0);
	XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,1, 0);

	XGpio_WriteBit(XPAR_AXI_GPIO_dut0_1_BASEADDR,0,5, 0);	//S0-VMON_TRIM_EN
	XGpio_WriteBit(XPAR_AXI_GPIO_dut1_1_BASEADDR,0,5, 0);
	XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,5, 0);
	XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,5, 0);

	XGpio_WriteBit(XPAR_AXI_GPIO_dut0_1_BASEADDR,0,13, 0);	//S0_FPGA_RELAY8
	XGpio_WriteBit(XPAR_AXI_GPIO_dut1_1_BASEADDR,0,13, 0);
	XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,13, 0);
	XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,13, 0);

	XGpio_WriteBit(XPAR_AXI_GPIO_dut0_1_BASEADDR,0,14, 0);
	XGpio_WriteBit(XPAR_AXI_GPIO_dut1_1_BASEADDR,0,14, 0);
	XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,14, 0);
	XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,14, 0);

	XGpio_WriteBit(XPAR_AXI_GPIO_dut0_1_BASEADDR,0,15, 0);
	XGpio_WriteBit(XPAR_AXI_GPIO_dut1_1_BASEADDR,0,15, 0);
	XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,15, 0);
	XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,15, 0);

	XGpio_WriteBit(XPAR_AXI_GPIO_dut0_1_BASEADDR,0,16, 0);	//S0_FPGA_RELAY9
	XGpio_WriteBit(XPAR_AXI_GPIO_dut1_1_BASEADDR,0,16, 0);
	XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,16, 0);
	XGpio_WriteBit(XPAR_AXI_GPIO_dut3_1_BASEADDR,0,16, 0);
}


