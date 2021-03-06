#include "platform.h"
#include "Pattern6_CCDet.h"
#include "ccdet_calibration_dut.h"

//dut2 Pattern 6??LDO3V3 Calibration and test
u8 _by_Pattern6_CCDet_dut2()
{
	u8 i;

	u8 Buff_dut2_XGPIO_0[8];

	switch(dut2.g_ccdet_step)
	{
	case 0x0000:
	{
		//gpio17: low bit, gpio20: high bit
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,17, 0);
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,18, 1);
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,19, 1);
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,20, 0);
		msdelay(5);

		//enable ADG1604
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,13, 1);	//S0-CC1_TEST_EN
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,14, 1);	//S0-CC2_TEST_EN
		msdelay(5);
		//select ADG1604 S1 channel
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,9, 0);	//S0-CC1_TEST_SEL0
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,10, 1);	//S0-CC1_TEST_SEL1

		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,24, 0);	//S0-CC2_TEST_SEL0
		XGpio_WriteBit(XPAR_AXI_GPIO_dut2_1_BASEADDR,0,27, 1);	//S0-CC2_TEST_SEL1
		msdelay(5);

		dut2.g_ccdet_step++;
	}

	//cc1 0.2v
	case 0x0001:
	{
		dut2.g_dac_data_buf[0] = 0x01;
		dut2.g_dac_data_buf[1] = 0x90;
		dut2.g_dac_data_buf[2] = 0x01;
		dut2.g_dac_data_buf[3] = 0x68;
		dut2.g_dac_data_buf[4] = 0x01;
		dut2.g_dac_data_buf[5] = 0xb8;
		dut2.g_dac_data_buf[6] = 0x01;

		dut2.g_ccdet_phy_calibration_addr_buf[0] = 0x0f;
		dut2.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut2.g_ccdet_voltage_sel = 0x55;
		dut2.g_ccdet_calibration_data_buf[0]     = dut2.g_ccdet_calibration_data_buf[4];
		dut2.g_ccdet_calibration_data_buf[1]     = dut2.g_ccdet_calibration_data_buf[5];
		dut2.g_ccdet_phy_calibration_bit         = 3;
		dut2.g_ccdet_ana_result_bit              = 2;

		_ccdet_calibration_dut2();
		if(dut2.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut2.g_ccdet_calibration_data_buf[4] = dut2.g_ccdet_calibration_data_buf[2];
			dut2.g_ccdet_calibration_data_buf[5] = dut2.g_ccdet_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut2.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut2.g_ccdet_process_bit = ccdet_process_waiting;
			dut2.g_ccdet_retest_signbit = 0;
			dut2.g_ccdet_step++;
			dut2.g_pattern_step = 0;
			xil_printf("dut2.g_ccdet_step = %x\r\n",dut2.g_ccdet_step);
		}
			////////xil_printf("dut2_pattern6_CC1_0.2V_Calibration\r\n");
		    break;
	}

	//cc2 0.2v
	case 0x0002:
	{
		dut2.g_dac_data_buf[0] = 0x03;
		dut2.g_dac_data_buf[1] = 0x90;
		dut2.g_dac_data_buf[2] = 0x01;
		dut2.g_dac_data_buf[3] = 0x68;
		dut2.g_dac_data_buf[4] = 0x01;
		dut2.g_dac_data_buf[5] = 0xb8;
		dut2.g_dac_data_buf[6] = 0x01;

		dut2.g_ccdet_phy_calibration_addr_buf[0] = 0x0f;
		dut2.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut2.g_ccdet_voltage_sel = 0x55;
		dut2.g_ccdet_calibration_data_buf[0]     = dut2.g_ccdet_calibration_data_buf[4];
		dut2.g_ccdet_calibration_data_buf[1]     = dut2.g_ccdet_calibration_data_buf[5];
		dut2.g_ccdet_phy_calibration_bit         = 2;
		dut2.g_ccdet_ana_result_bit              = 5;

		_ccdet_calibration_dut2();
		if(dut2.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut2.g_ccdet_calibration_data_buf[4] = dut2.g_ccdet_calibration_data_buf[2];
			dut2.g_ccdet_calibration_data_buf[5] = dut2.g_ccdet_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut2.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut2.g_ccdet_process_bit = ccdet_process_waiting;
			dut2.g_ccdet_retest_signbit = 0;
			dut2.g_ccdet_step++;
			dut2.g_pattern_step = 0;
			//xil_printf("dut2.g_ccdet_step = %x\r\n",dut2.g_ccdet_step);
		}
			////////xil_printf("dut2_pattern6_CC2_0.2V_Calibration\r\n");
		    break;
	}

	//cc1 0.4v
	case 0x0003:
	{
		dut2.g_dac_data_buf[0] = 0x01;
		dut2.g_dac_data_buf[1] = 0x20;
		dut2.g_dac_data_buf[2] = 0x03;
		dut2.g_dac_data_buf[3] = 0xe4;
		dut2.g_dac_data_buf[4] = 0x02;
		dut2.g_dac_data_buf[5] = 0x5c;
		dut2.g_dac_data_buf[6] = 0x03;

		dut2.g_ccdet_phy_calibration_addr_buf[0] = 0x0f;
		dut2.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut2.g_ccdet_voltage_sel = 0xaa;
		dut2.g_ccdet_calibration_data_buf[0]     = dut2.g_ccdet_calibration_data_buf[4];
		dut2.g_ccdet_calibration_data_buf[1]     = dut2.g_ccdet_calibration_data_buf[5];
		dut2.g_ccdet_phy_calibration_bit         = 1;
		dut2.g_ccdet_ana_result_bit              = 3;

		_ccdet_calibration_dut2();
		if(dut2.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut2.g_ccdet_calibration_data_buf[4] = dut2.g_ccdet_calibration_data_buf[2];
			dut2.g_ccdet_calibration_data_buf[5] = dut2.g_ccdet_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut2.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut2.g_ccdet_process_bit = ccdet_process_waiting;
			dut2.g_ccdet_retest_signbit = 0;

			dut2.g_ccdet_step++;
			dut2.g_pattern_step = 0;
			xil_printf("dut2.g_ccdet_step = %x\r\n",dut2.g_ccdet_step);
		}
			////////xil_printf("dut2_pattern6_CC1_0.4V_Calibration\r\n");
		    break;
	}

	//cc2 0.4v
	case 0x0004:
	{
		dut2.g_dac_data_buf[0] = 0x03;
		dut2.g_dac_data_buf[1] = 0x20;
		dut2.g_dac_data_buf[2] = 0x03;
		dut2.g_dac_data_buf[3] = 0xe4;
		dut2.g_dac_data_buf[4] = 0x02;
		dut2.g_dac_data_buf[5] = 0x5c;
		dut2.g_dac_data_buf[6] = 0x03;

		dut2.g_ccdet_phy_calibration_addr_buf[0] = 0x0f;
		dut2.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut2.g_ccdet_voltage_sel = 0xaa;
		dut2.g_ccdet_calibration_data_buf[0]     = dut2.g_ccdet_calibration_data_buf[4];
		dut2.g_ccdet_calibration_data_buf[1]     = dut2.g_ccdet_calibration_data_buf[5];
		dut2.g_ccdet_phy_calibration_bit         = 0;
		dut2.g_ccdet_ana_result_bit              = 6;

		_ccdet_calibration_dut2();
		if(dut2.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut2.g_ccdet_calibration_data_buf[4] = dut2.g_ccdet_calibration_data_buf[2];
			dut2.g_ccdet_calibration_data_buf[5] = dut2.g_ccdet_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut2.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut2.g_ccdet_process_bit = ccdet_process_waiting;
			dut2.g_ccdet_retest_signbit = 0;
			dut2.g_ccdet_step++;
			dut2.g_pattern_step = 0;
			//xil_printf("dut2.g_ccdet_step = %x\r\n",dut2.g_ccdet_step);
		}
			////////xil_printf("dut2_pattern6_CC2_0.4V_Calibration\r\n");
		    break;
	}

	//cc1 0.8v
	case 0x0005:
	{
		dut2.g_dac_data_buf[0] = 0x01;
		dut2.g_dac_data_buf[1] = 0x40;
		dut2.g_dac_data_buf[2] = 0x06;
		dut2.g_dac_data_buf[3] = 0x04;
		dut2.g_dac_data_buf[4] = 0x06;
		dut2.g_dac_data_buf[5] = 0x08;
		dut2.g_dac_data_buf[6] = 0x07;

		dut2.g_ccdet_phy_calibration_addr_buf[0] = 0x10;
		dut2.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut2.g_ccdet_voltage_sel = 0xff;
		dut2.g_ccdet_calibration_data_buf[0]     = dut2.g_ccdet_calibration_data_buf[6];
		dut2.g_ccdet_calibration_data_buf[1]     = dut2.g_ccdet_calibration_data_buf[7];
		dut2.g_ccdet_phy_calibration_bit         = 1;
		dut2.g_ccdet_ana_result_bit              = 3;

		_ccdet_calibration_dut2();
		if(dut2.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut2.g_ccdet_calibration_data_buf[6] = dut2.g_ccdet_calibration_data_buf[2];
			dut2.g_ccdet_calibration_data_buf[7] = dut2.g_ccdet_calibration_data_buf[3];
			////xil_printf("dut2.g_ccdet_calibration_data_buf[2] = %x\r\n",dut2.g_ccdet_calibration_data_buf[2]);
			////xil_printf("dut2.g_ccdet_calibration_data_buf[3] = %x\r\n",dut2.g_ccdet_calibration_data_buf[3]);
			////xil_printf("dut2.g_ccdet_calibration_data_buf[6] = %x\r\n",dut2.g_ccdet_calibration_data_buf[6]);
			////xil_printf("dut2.g_ccdet_calibration_data_buf[7] = %x\r\n",dut2.g_ccdet_calibration_data_buf[7]);
			for(i=0;i<4;i++)
			{
				dut2.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut2.g_ccdet_process_bit = ccdet_process_waiting;
			dut2.g_ccdet_retest_signbit = 0;
			dut2.g_ccdet_step++;
			dut2.g_pattern_step = 0;
			xil_printf("dut2.g_ccdet_step = %x\r\n",dut2.g_ccdet_step);
			////xil_printf("dut2_pattern6_CC1_0.8V_Calibration\r\n");
			////xil_printf("dut2.g_ccdet_calibration_data_buf[6] = %x\r\n",dut2.g_ccdet_calibration_data_buf[6]);
			////xil_printf("dut2.g_ccdet_calibration_data_buf[7] = %x\r\n",dut2.g_ccdet_calibration_data_buf[7]);
		}
			////////xil_printf("dut2_pattern6_CC1_0.8V_Calibration\r\n");
		    break;
	}

	//cc2 0.8v
	case 0x0006:
	{
		dut2.g_dac_data_buf[0] = 0x03;
		dut2.g_dac_data_buf[1] = 0x40;
		dut2.g_dac_data_buf[2] = 0x06;
		dut2.g_dac_data_buf[3] = 0x04;
		dut2.g_dac_data_buf[4] = 0x06;
		dut2.g_dac_data_buf[5] = 0x08;
		dut2.g_dac_data_buf[6] = 0x07;

		dut2.g_ccdet_phy_calibration_addr_buf[0] = 0x10;
		dut2.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut2.g_ccdet_voltage_sel = 0xff;
		dut2.g_ccdet_calibration_data_buf[0]     = dut2.g_ccdet_calibration_data_buf[6];
		dut2.g_ccdet_calibration_data_buf[1]     = dut2.g_ccdet_calibration_data_buf[7];
		dut2.g_ccdet_phy_calibration_bit         = 0;
		dut2.g_ccdet_ana_result_bit              = 6;

		_ccdet_calibration_dut2();
		if(dut2.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut2.g_ccdet_calibration_data_buf[6] = dut2.g_ccdet_calibration_data_buf[2];
			dut2.g_ccdet_calibration_data_buf[7] = dut2.g_ccdet_calibration_data_buf[3];
			////xil_printf("dut2.g_ccdet_calibration_data_buf[2] = %x\r\n",dut2.g_ccdet_calibration_data_buf[2]);
			////xil_printf("dut2.g_ccdet_calibration_data_buf[3] = %x\r\n",dut2.g_ccdet_calibration_data_buf[3]);
			////xil_printf("dut2.g_ccdet_calibration_data_buf[6] = %x\r\n",dut2.g_ccdet_calibration_data_buf[6]);
			////xil_printf("dut2.g_ccdet_calibration_data_buf[7] = %x\r\n",dut2.g_ccdet_calibration_data_buf[7]);
			for(i=0;i<4;i++)
			{
				dut2.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut2.g_ccdet_process_bit = ccdet_process_waiting;
			dut2.g_ccdet_retest_signbit = 0;
			dut2.g_ccdet_step++;
			dut2.g_pattern_step = 0;
			//xil_printf("dut2.g_ccdet_step = %x\r\n",dut2.g_ccdet_step);
			////xil_printf("dut2_pattern6_CC2_0.8V_Calibration\r\n");
			////xil_printf("dut2.g_ccdet_calibration_data_buf[6] = %x\r\n",dut2.g_ccdet_calibration_data_buf[6]);
			////xil_printf("dut2.g_ccdet_calibration_data_buf[7] = %x\r\n\r\n",dut2.g_ccdet_calibration_data_buf[7]);
		}
			////////xil_printf("dut2_pattern6_CC2_0.8V_Calibration\r\n");
		    break;
	}

	//cc1 0.66v
	case 0x0007:
	{
		dut2.g_dac_data_buf[0] = 0x01;
		dut2.g_dac_data_buf[1] = 0x28;
		dut2.g_dac_data_buf[2] = 0x05;
		dut2.g_dac_data_buf[3] = 0x00;
		dut2.g_dac_data_buf[4] = 0x05;
		dut2.g_dac_data_buf[5] = 0x50;
		dut2.g_dac_data_buf[6] = 0x05;

		dut2.g_ccdet_phy_calibration_addr_buf[0] = 0x10;
		dut2.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut2.g_ccdet_voltage_sel = 0x00;
		dut2.g_ccdet_calibration_data_buf[0]     = dut2.g_ccdet_calibration_data_buf[6];
		dut2.g_ccdet_calibration_data_buf[1]     = dut2.g_ccdet_calibration_data_buf[7];
		dut2.g_ccdet_phy_calibration_bit         = 3;
		dut2.g_ccdet_ana_result_bit              = 3;
		////xil_printf("dut2.g_ccdet_calibration_data_buf[0] = %x\r\n",dut2.g_ccdet_calibration_data_buf[0]);
		////xil_printf("dut2.g_ccdet_calibration_data_buf[1] = %x\r\n",dut2.g_ccdet_calibration_data_buf[1]);
		////xil_printf("dut2.g_ccdet_calibration_data_buf[6] = %x\r\n",dut2.g_ccdet_calibration_data_buf[6]);
		////xil_printf("dut2.g_ccdet_calibration_data_buf[7] = %x\r\n\r\n",dut2.g_ccdet_calibration_data_buf[7]);
		////xil_printf("dut2.g_calibration_offset =%x\r\n", dut2.g_calibration_offset);
		////xil_printf("dut2.g_ccdet_retest_signbit =%x\r\n", dut2.g_ccdet_retest_signbit);
		_ccdet_calibration_dut2();
		////xil_printf("dut2.g_ccdet_step = %x\r\n",dut2.g_ccdet_step);
		//xil_printf("dut2.g_pattern_step = %x\r\n",dut2.g_pattern_step);
		if(dut2.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut2.g_ccdet_calibration_data_buf[6] = dut2.g_ccdet_calibration_data_buf[2];
			dut2.g_ccdet_calibration_data_buf[7] = dut2.g_ccdet_calibration_data_buf[3];

			//xil_printf("dut2.g_ccdet_calibration_data_buf[2] = %x\r\n",dut2.g_ccdet_calibration_data_buf[2]);
			//xil_printf("dut2.g_ccdet_calibration_data_buf[3] = %x\r\n",dut2.g_ccdet_calibration_data_buf[3]);
			//xil_printf("dut2.g_ccdet_calibration_data_buf[6] = %x\r\n",dut2.g_ccdet_calibration_data_buf[6]);
			//xil_printf("dut2.g_ccdet_calibration_data_buf[7] = %x\r\n\r\n",dut2.g_ccdet_calibration_data_buf[7]);
			for(i=0;i<4;i++)
			{
				dut2.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut2.g_ccdet_process_bit = ccdet_process_waiting;
			dut2.g_ccdet_retest_signbit = 0;
			dut2.g_ccdet_step++;
			dut2.g_pattern_step = 0;
			xil_printf("dut2.g_ccdet_step = %x\r\n",dut2.g_ccdet_step);
			////xil_printf("dut2_pattern6_CC1_0.66V_Calibration\r\n");
			////xil_printf("dut2.g_ccdet_calibration_data_buf[6] = %x\r\n",dut2.g_ccdet_calibration_data_buf[6]);

		}
			////////xil_printf("dut2_pattern6_CC1_0.66V_Calibration\r\n");
		    break;
	}

	//cc2 0.66v
	case 0x0008:
	{
		dut2.g_dac_data_buf[0] = 0x03;
		dut2.g_dac_data_buf[1] = 0x28;
		dut2.g_dac_data_buf[2] = 0x05;
		dut2.g_dac_data_buf[3] = 0x00;
		dut2.g_dac_data_buf[4] = 0x05;
		dut2.g_dac_data_buf[5] = 0x50;
		dut2.g_dac_data_buf[6] = 0x05;

		dut2.g_ccdet_phy_calibration_addr_buf[0] = 0x10;
		dut2.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut2.g_ccdet_voltage_sel = 0x00;
		dut2.g_ccdet_calibration_data_buf[0]     = dut2.g_ccdet_calibration_data_buf[6];
		dut2.g_ccdet_calibration_data_buf[1]     = dut2.g_ccdet_calibration_data_buf[7];
		dut2.g_ccdet_phy_calibration_bit         = 2;
		dut2.g_ccdet_ana_result_bit              = 6;

		_ccdet_calibration_dut2();
		if(dut2.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut2.g_ccdet_calibration_data_buf[6] = dut2.g_ccdet_calibration_data_buf[2];
			dut2.g_ccdet_calibration_data_buf[7] = dut2.g_ccdet_calibration_data_buf[3];
			////xil_printf("dut2.g_ccdet_calibration_data_buf[2] = %x\r\n",dut2.g_ccdet_calibration_data_buf[2]);
			////xil_printf("dut2.g_ccdet_calibration_data_buf[3] = %x\r\n",dut2.g_ccdet_calibration_data_buf[3]);
			////xil_printf("dut2.g_ccdet_calibration_data_buf[6] = %x\r\n",dut2.g_ccdet_calibration_data_buf[6]);
			////xil_printf("dut2.g_ccdet_calibration_data_buf[7] = %x\r\n",dut2.g_ccdet_calibration_data_buf[7]);
			for(i=0;i<4;i++)
			{
				dut2.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut2.g_ccdet_process_bit = ccdet_process_waiting;
			dut2.g_ccdet_retest_signbit = 0;
			dut2.g_ccdet_step++;
			dut2.g_pattern_step = 0;
			//xil_printf("dut2.g_ccdet_step = %x\r\n",dut2.g_ccdet_step);
			////xil_printf("dut2_pattern6_CC2_0.66V_Calibration\r\n");
			////xil_printf("dut2.g_ccdet_calibration_data_buf[6] = %x\r\n",dut2.g_ccdet_calibration_data_buf[6]);
			////xil_printf("dut2.g_ccdet_calibration_data_buf[7] = %x\r\n",dut2.g_ccdet_calibration_data_buf[7]);
		}
			////////xil_printf("dut2_pattern6_CC2_0.66V_Calibration\r\n");
		    break;
	}

	//cc1 1.23v
	case 0x0009:
	{
		dut2.g_dac_data_buf[0] = 0x01;
		dut2.g_dac_data_buf[1] = 0x9c;
		dut2.g_dac_data_buf[2] = 0x09;
		dut2.g_dac_data_buf[3] = 0x60;
		dut2.g_dac_data_buf[4] = 0x09;
		dut2.g_dac_data_buf[5] = 0xd8;
		dut2.g_dac_data_buf[6] = 0x09;

		dut2.g_ccdet_phy_calibration_addr_buf[0] = 0x11;
		dut2.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut2.g_ccdet_voltage_sel = 0x00;
		dut2.g_ccdet_calibration_data_buf[0]     = dut2.g_ccdet_calibration_data_buf[8];
		dut2.g_ccdet_calibration_data_buf[1]     = dut2.g_ccdet_calibration_data_buf[9];
		dut2.g_ccdet_phy_calibration_bit         = 3;
		dut2.g_ccdet_ana_result_bit              = 4;
		////////xil_printf("dut2_pattern6_CC1_1.23V_Calibration_before\r\n");
		_ccdet_calibration_dut2();
		if(dut2.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut2.g_ccdet_calibration_data_buf[8] = dut2.g_ccdet_calibration_data_buf[2];
			dut2.g_ccdet_calibration_data_buf[9] = dut2.g_ccdet_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut2.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut2.g_ccdet_process_bit = ccdet_process_waiting;
			dut2.g_ccdet_retest_signbit = 0;
			dut2.g_ccdet_step++;
			dut2.g_pattern_step = 0;
			xil_printf("dut2.g_ccdet_step = %x\r\n",dut2.g_ccdet_step);
		}
			////////xil_printf("dut2_pattern6_CC1_1.23V_Calibration\r\n");
		    break;
	}

	//cc2 1.23v
	case 0x000a:
	{
		dut2.g_dac_data_buf[0] = 0x03;
		dut2.g_dac_data_buf[1] = 0x9c;
		dut2.g_dac_data_buf[2] = 0x09;
		dut2.g_dac_data_buf[3] = 0x60;
		dut2.g_dac_data_buf[4] = 0x09;
		dut2.g_dac_data_buf[5] = 0xd8;
		dut2.g_dac_data_buf[6] = 0x09;

		dut2.g_ccdet_phy_calibration_addr_buf[0] = 0x11;
		dut2.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut2.g_ccdet_voltage_sel = 0x00;
		dut2.g_ccdet_calibration_data_buf[0]     = dut2.g_ccdet_calibration_data_buf[8];
		dut2.g_ccdet_calibration_data_buf[1]     = dut2.g_ccdet_calibration_data_buf[9];
		dut2.g_ccdet_phy_calibration_bit         = 2;
		dut2.g_ccdet_ana_result_bit              = 7;

		_ccdet_calibration_dut2();
		if(dut2.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut2.g_ccdet_calibration_data_buf[8] = dut2.g_ccdet_calibration_data_buf[2];
			dut2.g_ccdet_calibration_data_buf[9] = dut2.g_ccdet_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut2.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut2.g_ccdet_process_bit = ccdet_process_waiting;
			dut2.g_ccdet_retest_signbit = 0;
			dut2.g_ccdet_step++;
			dut2.g_pattern_step = 0;
			//xil_printf("dut2.g_ccdet_step = %x\r\n",dut2.g_ccdet_step);
		}
			////////xil_printf("dut2_pattern6_CC2_1.23V_Calibration\r\n");
		    break;
	}

	//cc1 1.6v
	case 0x000b:
	{
		dut2.g_dac_data_buf[0] = 0x01;
		dut2.g_dac_data_buf[1] = 0x80;
		dut2.g_dac_data_buf[2] = 0x0c;
		dut2.g_dac_data_buf[3] = 0xf4;
		dut2.g_dac_data_buf[4] = 0x0b;
		dut2.g_dac_data_buf[5] = 0xbc;
		dut2.g_dac_data_buf[6] = 0x0c;

		dut2.g_ccdet_phy_calibration_addr_buf[0] = 0x11;
		dut2.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut2.g_ccdet_voltage_sel = 0x55;
		dut2.g_ccdet_calibration_data_buf[0]     = dut2.g_ccdet_calibration_data_buf[8];
		dut2.g_ccdet_calibration_data_buf[1]     = dut2.g_ccdet_calibration_data_buf[9];
		dut2.g_ccdet_phy_calibration_bit         = 1;
		dut2.g_ccdet_ana_result_bit              = 4;

		_ccdet_calibration_dut2();
		if(dut2.g_ccdet_process_bit != ccdet_process_done_pass)
		{
			break;
		}
		else
		{
			dut2.g_ccdet_calibration_data_buf[8] = dut2.g_ccdet_calibration_data_buf[2];
			dut2.g_ccdet_calibration_data_buf[9] = dut2.g_ccdet_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut2.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut2.g_ccdet_process_bit = ccdet_process_waiting;
			dut2.g_ccdet_retest_signbit = 0;
			dut2.g_ccdet_step++;
			dut2.g_pattern_step = 0;
			xil_printf("dut2.g_ccdet_step = %x\r\n",dut2.g_ccdet_step);
		}
			////////xil_printf("dut2_pattern6_CC1_1.6V_Calibration\r\n");
		    break;
	}

	//cc2 1.6v
	case 0x000c:
	{
		dut2.g_dac_data_buf[0] = 0x03;
		dut2.g_dac_data_buf[1] = 0x80;
		dut2.g_dac_data_buf[2] = 0x0c;
		dut2.g_dac_data_buf[3] = 0xf4;
		dut2.g_dac_data_buf[4] = 0x0b;
		dut2.g_dac_data_buf[5] = 0xbc;
		dut2.g_dac_data_buf[6] = 0x0c;

		dut2.g_ccdet_phy_calibration_addr_buf[0] = 0x11;
		dut2.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut2.g_ccdet_voltage_sel = 0x55;
		dut2.g_ccdet_calibration_data_buf[0]     = dut2.g_ccdet_calibration_data_buf[8];
		dut2.g_ccdet_calibration_data_buf[1]     = dut2.g_ccdet_calibration_data_buf[9];
		dut2.g_ccdet_phy_calibration_bit         = 0;
		dut2.g_ccdet_ana_result_bit              = 7;

		_ccdet_calibration_dut2();
		if(dut2.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut2.g_ccdet_calibration_data_buf[8] = dut2.g_ccdet_calibration_data_buf[2];
			dut2.g_ccdet_calibration_data_buf[9] = dut2.g_ccdet_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut2.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut2.g_ccdet_process_bit = ccdet_process_waiting;
			dut2.g_ccdet_retest_signbit = 0;
			dut2.g_ccdet_step++;
			dut2.g_pattern_step = 0;
			//xil_printf("dut2.g_ccdet_step = %x\r\n",dut2.g_ccdet_step);
		}
			////////xil_printf("dut2_pattern6_CC2_1.6V_Calibration\r\n");
		    break;
	}

	//cc1 2.6v
	case 0x000d:
	{
		dut2.g_dac_data_buf[0] = 0x05;
		dut2.g_dac_data_buf[1] = 0x28;
		dut2.g_dac_data_buf[2] = 0x0a;
		dut2.g_dac_data_buf[3] = 0xa6;
		dut2.g_dac_data_buf[4] = 0x09;
		dut2.g_dac_data_buf[5] = 0xaa;
		dut2.g_dac_data_buf[6] = 0x0a;

		dut2.g_ccdet_phy_calibration_addr_buf[0] = 0x12;
		dut2.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut2.g_ccdet_voltage_sel = 0xff;
		dut2.g_ccdet_calibration_data_buf[0]     = dut2.g_ccdet_calibration_data_buf[10];
		dut2.g_ccdet_calibration_data_buf[1]     = dut2.g_ccdet_calibration_data_buf[11];
		dut2.g_ccdet_phy_calibration_bit         = 1;
		dut2.g_ccdet_ana_result_bit              = 4;

		_ccdet_calibration_dut2();
		if(dut2.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut2.g_ccdet_calibration_data_buf[10] = dut2.g_ccdet_calibration_data_buf[2];
			dut2.g_ccdet_calibration_data_buf[11] = dut2.g_ccdet_calibration_data_buf[3];
			for(i=0;i<4;i++)
			{
				dut2.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut2.g_ccdet_process_bit = ccdet_process_waiting;
			dut2.g_ccdet_retest_signbit = 0;
			dut2.g_ccdet_step++;
			dut2.g_pattern_step = 0;
			xil_printf("dut2.g_ccdet_step = %x\r\n",dut2.g_ccdet_step);
		}
			////////xil_printf("dut2_pattern6_CC1_2.6V_Calibration\r\n");
		    break;
	}

	//cc2 2.6v
	case 0x000e:
	{
		dut2.g_dac_data_buf[0] = 0x07;
		dut2.g_dac_data_buf[1] = 0x28;
		dut2.g_dac_data_buf[2] = 0x0a;
		dut2.g_dac_data_buf[3] = 0xa6;
		dut2.g_dac_data_buf[4] = 0x09;
		dut2.g_dac_data_buf[5] = 0xaa;
		dut2.g_dac_data_buf[6] = 0x0a;

		dut2.g_ccdet_phy_calibration_addr_buf[0] = 0x12;
		dut2.g_ccdet_phy_calibration_addr_buf[1] = 0x01;
		dut2.g_ccdet_voltage_sel = 0xff;
		dut2.g_ccdet_calibration_data_buf[0]     = dut2.g_ccdet_calibration_data_buf[10];
		dut2.g_ccdet_calibration_data_buf[1]     = dut2.g_ccdet_calibration_data_buf[11];
		dut2.g_ccdet_phy_calibration_bit         = 0;
		dut2.g_ccdet_ana_result_bit              = 7;

		_ccdet_calibration_dut2();

		if(dut2.g_ccdet_process_bit != ccdet_process_done_pass)
		{

		}
		else
		{
			dut2.g_ccdet_calibration_data_buf[10] = dut2.g_ccdet_calibration_data_buf[2];
			dut2.g_ccdet_calibration_data_buf[11] = dut2.g_ccdet_calibration_data_buf[3];

			for(i=0;i<4;i++)
			{
				dut2.g_ccdet_calibration_data_buf[i] = 0;
			}
			dut2.g_ccdet_process_bit = ccdet_process_waiting;
			dut2.g_ccdet_retest_signbit = 0;
			dut2.g_ccdet_step++;
			dut2.g_pattern_step = 0;

      		//xil_printf("dut2.g_ccdet_calibration_data_buf[4] =%x\r\n", dut2.g_ccdet_calibration_data_buf[4]);
      		//xil_printf("dut2.g_ccdet_calibration_data_buf[5] =%x\r\n", dut2.g_ccdet_calibration_data_buf[5]);
      		//xil_printf("dut2.g_ccdet_calibration_data_buf[6] =%x\r\n", dut2.g_ccdet_calibration_data_buf[6]);
      		//xil_printf("dut2.g_ccdet_calibration_data_buf[7] =%x\r\n", dut2.g_ccdet_calibration_data_buf[7]);
      		//xil_printf("dut2.g_ccdet_calibration_data_buf[8] =%x\r\n", dut2.g_ccdet_calibration_data_buf[8]);
      		//xil_printf("dut2.g_ccdet_calibration_data_buf[9] =%x\r\n", dut2.g_ccdet_calibration_data_buf[9]);
      		//xil_printf("dut2.g_ccdet_calibration_data_buf[10] =%x\r\n", dut2.g_ccdet_calibration_data_buf[10]);
      		//xil_printf("dut2.g_ccdet_calibration_data_buf[11] =%x\r\n\r\n", dut2.g_ccdet_calibration_data_buf[11]);
		}
		break;
	}

	case 0x000f:
	{
		if(((dut2.g_ccdet_calibration_data_buf[4]&0x0f)>ccdet_writeefuse_min)&&((dut2.g_ccdet_calibration_data_buf[4]&0x0f)<ccdet_writeefuse_max))
		{
			dut2.g_ccdet_step++;
		}
		else
		{
			dut2.g_result_fail = 0x01;
			//xil_printf("dut2_g_ccdet_writeefuse fail, not fit spec!\r\n");
		}
		break;
	}

	case 0x0010:
	{
		if((((dut2.g_ccdet_calibration_data_buf[4]>>4)&0x0f)>ccdet_writeefuse_min)&&(((dut2.g_ccdet_calibration_data_buf[4]>>4)&0x0f)<ccdet_writeefuse_max))
		{
			dut2.g_ccdet_step++;
		}
		else
		{
			dut2.g_result_fail = 0x01;
			//xil_printf("dut2_g_ccdet_writeefuse fail, not fit spec!\r\n");
		}
		break;
	}

	case 0x0011:
	{
		if(((dut2.g_ccdet_calibration_data_buf[5]&0x0f)>ccdet_writeefuse_min)&&((dut2.g_ccdet_calibration_data_buf[5]&0x0f)<ccdet_writeefuse_max))
		{
			dut2.g_ccdet_step++;
		}
		else
		{
			dut2.g_result_fail = 0x01;
			//xil_printf("dut2_g_ccdet_writeefuse fail, not fit spec!\r\n");
		}
		break;
	}

	case 0x0012:
	{
		if((((dut2.g_ccdet_calibration_data_buf[5]>>4)&0x0f)>ccdet_writeefuse_min)&&(((dut2.g_ccdet_calibration_data_buf[5]>>4)&0x0f)<ccdet_writeefuse_max))
		{
			dut2.g_ccdet_step++;
		}
		else
		{
			dut2.g_result_fail = 0x01;
			//xil_printf("dut2_g_ccdet_writeefuse fail, not fit spec!\r\n");
		}
		break;
	}

	case 0x0013:
	{
		if(((dut2.g_ccdet_calibration_data_buf[6]&0x0f)>ccdet_writeefuse_min)&&((dut2.g_ccdet_calibration_data_buf[6]&0x0f)<ccdet_writeefuse_max))
		{
			dut2.g_ccdet_step++;
		}
		else
		{
			dut2.g_result_fail = 0x01;
			//xil_printf("dut2_g_ccdet_writeefuse fail, not fit spec!\r\n");
		}
		break;
	}

	case 0x0014:
	{
		if((((dut2.g_ccdet_calibration_data_buf[6]>>4)&0x0f)>ccdet_writeefuse_min)&&(((dut2.g_ccdet_calibration_data_buf[6]>>4)&0x0f)<ccdet_writeefuse_max))
		{
			dut2.g_ccdet_step++;
		}
		else
		{
			dut2.g_result_fail = 0x01;
			//xil_printf("dut2_g_ccdet_writeefuse fail, not fit spec!\r\n");
		}
		break;
	}

	case 0x0015:
	{
		if(((dut2.g_ccdet_calibration_data_buf[7]&0x0f)>ccdet_writeefuse_min)&&((dut2.g_ccdet_calibration_data_buf[7]&0x0f)<ccdet_writeefuse_max))
		{
			dut2.g_ccdet_step++;
		}
		else
		{
			dut2.g_result_fail = 0x01;
			//xil_printf("dut2_g_ccdet_writeefuse fail, not fit spec!\r\n");
		}
		break;
	}

	case 0x0016:
	{
		if((((dut2.g_ccdet_calibration_data_buf[7]>>4)&0x0f)>ccdet_writeefuse_min)&&(((dut2.g_ccdet_calibration_data_buf[7]>>4)&0x0f)<ccdet_writeefuse_max))
		{
			dut2.g_ccdet_step++;
		}
		else
		{
			dut2.g_result_fail = 0x01;
			//xil_printf("dut2_g_ccdet_writeefuse fail, not fit spec!\r\n");
		}
		break;
	}

	case 0x0017:
	{
		if(((dut2.g_ccdet_calibration_data_buf[8]&0x0f)>ccdet_writeefuse_min)&&((dut2.g_ccdet_calibration_data_buf[8]&0x0f)<ccdet_writeefuse_max))
		{
			dut2.g_ccdet_step++;
		}
		else
		{
			dut2.g_result_fail = 0x01;
			//xil_printf("dut2_g_ccdet_writeefuse fail, not fit spec!\r\n");
		}
		break;
	}

	case 0x0018:
	{
		if((((dut2.g_ccdet_calibration_data_buf[8]>>4)&0x0f)>ccdet_writeefuse_min)&&(((dut2.g_ccdet_calibration_data_buf[8]>>4)&0x0f)<ccdet_writeefuse_max))
		{
			dut2.g_ccdet_step++;
		}
		else
		{
			dut2.g_result_fail = 0x01;
			//xil_printf("dut2_g_ccdet_writeefuse fail, not fit spec!\r\n");
		}
		break;
	}

	case 0x0019:
	{
		if(((dut2.g_ccdet_calibration_data_buf[9]&0x0f)>ccdet_writeefuse_min)&&((dut2.g_ccdet_calibration_data_buf[9]&0x0f)<ccdet_writeefuse_max))
		{
			dut2.g_ccdet_step++;
		}
		else
		{
			dut2.g_result_fail = 0x01;
			//xil_printf("dut2_g_ccdet_writeefuse fail, not fit spec!\r\n");
		}
		break;
	}

	case 0x001a:
	{
		if((((dut2.g_ccdet_calibration_data_buf[9]>>4)&0x0f)>ccdet_writeefuse_min)&&(((dut2.g_ccdet_calibration_data_buf[9]>>4)&0x0f)<ccdet_writeefuse_max))
		{
			dut2.g_ccdet_step++;
		}
		else
		{
			dut2.g_result_fail = 0x01;
			//xil_printf("dut2_g_ccdet_writeefuse fail, not fit spec!\r\n");
		}
		break;
	}

	case 0x001b:
	{
		if(((dut2.g_ccdet_calibration_data_buf[10]&0x0f)>ccdet_writeefuse_min)&&((dut2.g_ccdet_calibration_data_buf[10]&0x0f)<ccdet_writeefuse_max))
		{
			dut2.g_ccdet_step++;
		}
		else
		{
			dut2.g_result_fail = 0x01;
			//xil_printf("dut2_g_ccdet_writeefuse fail, not fit spec!\r\n");
		}
		break;
	}

	case 0x001c:
	{
		if((((dut2.g_ccdet_calibration_data_buf[10]>>4)&0x0f)>ccdet_writeefuse_min)&&(((dut2.g_ccdet_calibration_data_buf[10]>>4)&0x0f)<ccdet_writeefuse_max))
		{
			dut2.g_ccdet_step++;
		}
		else
		{
			dut2.g_result_fail = 0x01;
			//xil_printf("dut2_g_ccdet_writeefuse fail, not fit spec!\r\n");
		}
		break;
	}

	case 0x001d:
	{
		dut2.g_dac_data_buf[0] = 0;
		dut2.g_dac_data_buf[1] = 0;
		dut2.g_dac_data_buf[2] = 0;
		dut2.g_dac_data_buf[3] = 0;
		dut2.g_dac_data_buf[4] = 0;
		dut2.g_dac_data_buf[5] = 0;
		dut2.g_dac_data_buf[6] = 0;

		for(i=1; i<31; i++)
		{
			dut2.g_pattern_smbus_control_buf[i] = CLEAR_;
		}
		dut2.g_pattern_timer = 0xfff;
		dut2.g_pattern_smbus_control_buf[0] = smbus_road_waiting;
		dut2.g_ccdet_step = 0;
		dut2.g_pattern_step = 0;
		dut2.g_dut_pattern_status_buf[2]++;
		dut2.g_uartPatternEnable = 0x00;
		dut2.g_uartPatternNum = (dut2.g_uartPatternNum++ & 0xff);
		result_output_for_v50(XPAR_AXI_GPIO_dut2_1_BASEADDR,dut2.g_uartPatternNum);
		xil_printf("dut2_pattern6_CC_Calibration_pass!\r\n\r\n");

	    break;
	}
	}

	if(dut2.g_result_fail == 0x01)
	{
		dut2.g_ft2_test_done = 0x00;
		pattern_safe_state_dut2();	//add safe state
		xil_printf("dut2_pattern6_fail! fail_step = %x\r\n\r\n",dut2.g_ccdet_step);
	}

	//////xil_printf("dut2.g_pattern_step = %x\r\n",dut2.g_pattern_step);
	////xil_printf("dut2_pattern6_ccdet_step = %x\r\n\r\n",dut2.g_ccdet_step);
	   return 0;
}

















